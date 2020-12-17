# Copyright 2020 Cypress Semiconductor Corporation
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# From https://wiki.tcl-lang.org/page/constants
proc const {name value} {
    uplevel 1 [list set $name $value]
    uplevel 1 [list trace var $name w {error constant} ]
}

const ARG_MODE_IDX 0

const ARG_AUTO_IDX_SRC_FREQ 1
const ARG_AUTO_IDX_TARGET_FREQ 2
const ARG_AUTO_IDX_WCO_SRC 3
const ARG_AUTO_IDX_SRC_ACCURACY 4

const ARG_MANUAL_IDX_SRC_FREQ 1
const ARG_MANUAL_IDX_MULT 2
const ARG_MANUAL_IDX_DIV 3
const ARG_MANUAL_IDX_LOCK_TOLERANCE 4
const ARG_MANUAL_IDX_WCO_SRC 5
const ARG_MANUAL_IDX_SRC_ACCURACY 6

const MIN_CCO_OUTPUT_FREQ 48
const MIN_OUTPUT_FREQ [expr {$::MIN_CCO_OUTPUT_FREQ / 2}]
const MAX_OUTPUT_FREQ 100
const SRC_CCO_FREQ_RATIO 2.2

const OUTPUT_DIV_ENABLED 1

const CCO_RANGE [list "CY_SYSCLK_FLL_CCO_RANGE0" "CY_SYSCLK_FLL_CCO_RANGE1" "CY_SYSCLK_FLL_CCO_RANGE2" \
                 "CY_SYSCLK_FLL_CCO_RANGE3" "CY_SYSCLK_FLL_CCO_RANGE4"]
const TRIM_STEPS [list 0.0011034 0.001102 0.0011 0.0011 0.00117062]
const F_MARGIN [list 43600000.0 58100000.0 77200000.0 103000000.0 132000000.0]

const CONFIG_OUTPUT_MODE "CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT"

# igain and pgain bitfield values correspond to: 1/256, 1/128, ..., 4, 8
const IGAINS_PGAINS [list 0.00390625  0.0078125  0.015625  0.03125  0.0625  0.125 \
                          0.25        0.5        1.0       2.0      4.0     8.0]

const SUCCESS 0
const ERROR_ARG_COUNT 1
const ERROR_ID 2
const ERROR_ARG_VALUE 3

set channelName stdout

if {[chan names ModusToolbox] eq "ModusToolbox"} {
    set channelName ModusToolbox
}

proc solve_fll {} {
    if {$::argc < $::ARG_MODE_IDX + 1} {
        print_arg_count_error
        return $::ERROR_ARG_COUNT
    }
    if {[string compare -nocase [lindex $::argv $::ARG_MODE_IDX] "Auto"] == 0} {
        return [parse_and_run_auto]
    }
    if {[string compare -nocase [lindex $::argv $::ARG_MODE_IDX] "Manual"] == 0} {
        return [parse_and_run_manual]
    }
    return $::ERROR_ARG_VALUE
}

proc parse_and_run_auto {} {
    if {$::argc != $::ARG_AUTO_IDX_SRC_ACCURACY + 1} {
        print_arg_count_error
        return $::ERROR_ARG_COUNT
    }
    set srcFreqMHz [lindex $::argv $::ARG_AUTO_IDX_SRC_FREQ]
    set targetFreqMHz [lindex $::argv $::ARG_AUTO_IDX_TARGET_FREQ]
    set wcoIsSource [string is true [lindex $::argv $::ARG_AUTO_IDX_WCO_SRC]]
    set srcAccuracyPercent [lindex $::argv $::ARG_AUTO_IDX_SRC_ACCURACY]
    if {![string is double $srcFreqMHz] || ![string is double $targetFreqMHz] || ![string is double $srcAccuracyPercent]} {
        error "Unable to parse argument values: either $srcFreqMHz or $targetFreqMHz or $srcAccuracyPercent is not a floating-point number."
        return $::ERROR_ARG_VALUE
    }
    set srcFreqMHz [expr {double($srcFreqMHz)}]
    set targetFreqMHz [expr {double($targetFreqMHz)}]
    set srcAccuracyPercent [expr {double($srcAccuracyPercent)}]
    set retVal [verify_desired_frequency $srcFreqMHz $targetFreqMHz $::OUTPUT_DIV_ENABLED]
    if {$retVal != $::SUCCESS} {
        return $retVal
    }
    return [solve_auto $srcFreqMHz $targetFreqMHz $wcoIsSource [expr {abs($srcAccuracyPercent / 100.0)}]]
}

proc parse_and_run_manual {} {
    if {$::argc != $::ARG_MANUAL_IDX_SRC_ACCURACY + 1} {
        print_arg_count_error
        return $::ERROR_ARG_COUNT
    }
    set srcFreqMHz [lindex $::argv $::ARG_MANUAL_IDX_SRC_FREQ]
    set mult [lindex $::argv $::ARG_MANUAL_IDX_MULT]
    set div [lindex $::argv $::ARG_MANUAL_IDX_DIV]
    set lockTol [lindex $::argv $::ARG_MANUAL_IDX_LOCK_TOLERANCE]
    set wcoIsSource [string is true [lindex $::argv $::ARG_MANUAL_IDX_WCO_SRC]]
    set srcAccuracyPercent [lindex $::argv $::ARG_MANUAL_IDX_SRC_ACCURACY]
    if {![string is double $srcFreqMHz] || ![string is double $srcAccuracyPercent]} {
        error "Unable to parse argument values: either $srcFreqMHz or $srcAccuracyPercent is not a floating-point number."
        return $::ERROR_ARG_VALUE
    }
    if {![string is integer $mult] || ![string is integer $div] || ![string is integer $lockTol]} {
        error "Unable to parse argument values: either $mult or $div or $lockTol is not an integer."
        return $::ERROR_ARG_VALUE
    }
    set srcFreqMHz [expr {double($srcFreqMHz)}]
    set srcAccuracyPercent [expr {double($srcAccuracyPercent)}]

    set expectedFreq [expr {$srcFreqMHz * $mult / $div / ($::OUTPUT_DIV_ENABLED ? 2 : 1)}]
    set retVal [verify_desired_frequency $srcFreqMHz $expectedFreq $::OUTPUT_DIV_ENABLED]
    if {$retVal != $::SUCCESS} {
        return $retVal
    }
    return [solve_manual $srcFreqMHz $mult $div $lockTol $wcoIsSource [expr {abs($srcAccuracyPercent / 100.0)}]]
}

proc solve_auto {srcFreqMHz targetFreqMHz wcoIsSource srcAccuracy} {
    # constants indexed by ccoIdx
    # 1. Output division by 2 is always required.
    set configEnableOutputDiv $::OUTPUT_DIV_ENABLED
    # 2. Compute the target CCO frequency from the target output frequency and output division.
    set ccoFreqMHz [expr {$targetFreqMHz * ($configEnableOutputDiv ? 2 : 1)}]
    # 3. Compute the CCO range value from the CCO frequency.
    set ccoIdx [get_cco_range_idx $ccoFreqMHz]
    set configCcoRange [lindex $::CCO_RANGE $ccoIdx]
    # 4. Compute the FLL reference divider value. refDiv is a constant if the WCO is the FLL source.
    set configRefDiv [expr {$wcoIsSource ? 19 : int(ceil(($srcFreqMHz / $targetFreqMHz) * 250.0))}]
    # 5. Compute the FLL multiplier value.
    set configFllMult [expr {int(ceil($ccoFreqMHz / ($srcFreqMHz / $configRefDiv)))}]
    # 6. Compute the lock tolerance.  We assume CCO accuracy is 0.25%.
    set CCO_ACCURACY 0.0025
    set configLockTolerance [expr {int(ceil($configFllMult * (1.5 * ((1.0 + $CCO_ACCURACY) / (1.0 - $srcAccuracy) - 1.0))))}]
    # 7. Compute the CCO igain and pgain.
    set kcco [expr {([lindex $::TRIM_STEPS $ccoIdx] * [lindex $::F_MARGIN $ccoIdx]) / 1000.0}]
    set ki_p [expr {850.0 / ($kcco * ($configRefDiv / $srcFreqMHz))}]
    # 7a. Find the largest IGAIN value that is less than or equal to ki_p.
    set configIgain [expr {[llength $::IGAINS_PGAINS] - 1}]
    while {([lindex $::IGAINS_PGAINS $configIgain] > $ki_p) && ($configIgain != 0)} {
        incr configIgain -1
    }
    # 7b. Decrement igain if the WCO is the FLL source.
    if {$wcoIsSource && ($configIgain > 0)} {
        incr configIgain -1
    }
    # 7c. Find the largest PGAIN value that is less than or equal to ki_p - IGAINS_PGAINS[igain]
    set configPgain [expr {[llength $::IGAINS_PGAINS] - 1}]
    while {([lindex $::IGAINS_PGAINS $configPgain] > ($ki_p - [lindex $::IGAINS_PGAINS $configIgain]) && ($configPgain != 0))} {
        incr configPgain -1
    }
    # 7d. Decrement pgain if the WCO is the FLL source.
    if {$wcoIsSource && ($configPgain > 0)} {
        incr configPgain -1
    }
    # 8. Compute the CCO_FREQ bits in CLK_FLL_CONFIG4 register.
    set configCcoFreq [expr {int(floor(log($ccoFreqMHz * 1000000 / [lindex $::F_MARGIN $ccoIdx]) / log(1.0 + [lindex $::TRIM_STEPS $ccoIdx])))}]
    # 9. Compute the settling count, using a 1-usec settling time.  Use a constant if the WCO is the FLL source.
    set configSettlingCount [get_settling_count $wcoIsSource $srcFreqMHz $ccoFreqMHz $configRefDiv]
    set accuracy [calculate_fll_accuracy $ccoFreqMHz $srcFreqMHz [lindex $::TRIM_STEPS $ccoIdx] $configRefDiv $configLockTolerance]
    set accuracyString [format "%.04f" [expr {$accuracy * 100.0}]]
    set enableOutputDiv [expr {$configEnableOutputDiv ? "true" : "false"}]

    puts $::channelName "param:fllMult=$configFllMult"
    puts $::channelName "param:refDiv=$configRefDiv"
    puts $::channelName "param:ccoRange=$configCcoRange"
    puts $::channelName "param:enableOutputDiv=$enableOutputDiv"
    puts $::channelName "param:lockTolerance=$configLockTolerance"
    puts $::channelName "param:igain=$configIgain"
    puts $::channelName "param:pgain=$configPgain"
    puts $::channelName "param:settlingCount=$configSettlingCount"
    puts $::channelName "param:outputMode=$::CONFIG_OUTPUT_MODE"
    puts $::channelName "param:cco_Freq=$configCcoFreq"
    puts $::channelName "param:accuracy=$accuracyString"
    return $::SUCCESS
}

proc solve_manual {srcFreqMHz configFllMult configRefDiv configLockTolerance wcoIsSource srcAccuracy} {
    # constants indexed by ccoIdx
    # 1. Output division by 2 is always required.
    set configEnableOutputDiv $::OUTPUT_DIV_ENABLED
    set outputDiv [expr {$configEnableOutputDiv ? 2 : 1}]
    set actualFreq [expr {(($srcFreqMHz * $configFllMult) / $configRefDiv) / $outputDiv}]
    # 2. Compute the target CCO frequency from the target output frequency and output division.
    set ccoFreqMHz [expr {$actualFreq * $outputDiv}]
    # 3. Compute the CCO range value from the CCO frequency.
    set ccoIdx [get_cco_range_idx $ccoFreqMHz]
    set configCcoRange [lindex $::CCO_RANGE $ccoIdx]
    # 7. Compute the CCO igain and pgain.
    set kcco [expr {([lindex $::TRIM_STEPS $ccoIdx] * [lindex $::F_MARGIN $ccoIdx]) / 1000.0}]
    set ki_p [expr {850.0 / ($kcco * ($configRefDiv / $srcFreqMHz))}]
    # 7a. Find the largest IGAIN value that is less than or equal to ki_p.
    set configIgain [expr {[llength $::IGAINS_PGAINS] - 1}]
    while {([lindex $::IGAINS_PGAINS $configIgain] > $ki_p) && ($configIgain != 0)} {
        incr configIgain -1
    }
    # 7b. Decrement igain if the WCO is the FLL source.
    if {$wcoIsSource && ($configIgain > 0)} {
        incr configIgain -1
    }
    # 7c. Find the largest PGAIN value that is less than or equal to ki_p - IGAINS_PGAINS[igain]
    set configPgain [expr {[llength $::IGAINS_PGAINS] - 1}]
    while {([lindex $::IGAINS_PGAINS $configPgain] > ($ki_p - [lindex $::IGAINS_PGAINS $configIgain]) && ($configPgain != 0))} {
        incr configPgain -1
    }
    # 7d. Decrement pgain if the WCO is the FLL source.
    if {$wcoIsSource && ($configPgain > 0)} {
        incr configPgain -1
    }
    # 8. Compute the CCO_FREQ bits in CLK_FLL_CONFIG4 register.
    set configCcoFreq [expr {int(floor(log($ccoFreqMHz * 1000000 / [lindex $::F_MARGIN $ccoIdx]) / log(1.0 + [lindex $::TRIM_STEPS $ccoIdx])))}]
    # 9. Compute the settling count, using a 1-usec settling time.  Use a constant if the WCO is the FLL source.
    set configSettlingCount [get_settling_count $wcoIsSource $srcFreqMHz $ccoFreqMHz $configRefDiv]
    set accuracy [calculate_fll_accuracy $ccoFreqMHz $srcFreqMHz [lindex $::TRIM_STEPS $ccoIdx] $configRefDiv $configLockTolerance]
    set accuracyString [format "%.04f" [expr {$accuracy * 100.0}]]
    set enableOutputDiv [expr {$configEnableOutputDiv ? "true" : "false"}]

    puts $::channelName "param:fllMult=$configFllMult"
    puts $::channelName "param:refDiv=$configRefDiv"
    puts $::channelName "param:ccoRange=$configCcoRange"
    puts $::channelName "param:enableOutputDiv=$enableOutputDiv"
    puts $::channelName "param:lockTolerance=$configLockTolerance"
    puts $::channelName "param:igain=$configIgain"
    puts $::channelName "param:pgain=$configPgain"
    puts $::channelName "param:settlingCount=$configSettlingCount"
    puts $::channelName "param:outputMode=$::CONFIG_OUTPUT_MODE"
    puts $::channelName "param:cco_Freq=$configCcoFreq"
    puts $::channelName "param:accuracy=$accuracyString"
    return $::SUCCESS
}

proc get_settling_count {isWco srcFreqMHz ccoFreqMHz configRefDiv} {
    set ttref [expr {$configRefDiv / ($srcFreqMHz * 1000.0)}]
    set testval [expr {6.0 / ($ccoFreqMHz * 1000.0)}]
    set divval [expr {ceil($srcFreqMHz)}]
    set altval [expr {ceil(($divval / $ttref) + 1.0)}]
    return [expr {int($isWco ? 200 : (($ttref > $testval) ? $divval : (($divval > $altval) ? $divval : $altval)))}]
}

proc get_cco_range_idx {ccoFreqMHz} {
    if {$ccoFreqMHz >= 150.3392} {
        return 4
    } elseif {$ccoFreqMHz >= 113.00938} {
        return 3
    } elseif {$ccoFreqMHz >= 94.9487} {
        return 2
    } elseif {$ccoFreqMHz >= 63.8556} {
        return 1
    } else {
        return 0
    }
}

proc calculate_fll_accuracy {targetCcoFreq sourceRefFreq trimStep refDiv lockTolerance} {
    set measure [expr {(1.0 / $refDiv) * ($sourceRefFreq / $targetCcoFreq)}]
    set ss_2 [expr {$trimStep / 2.0}]
    set fllPrecision [expr {($measure > $ss_2) ? $measure : $ss_2}]
    return [expr {$fllPrecision * ($lockTolerance + 2)}]
}

proc verify_desired_frequency {sourceFreq targetFreq outputDiv} {
    set ratio [expr {$outputDiv ? ($::SRC_CCO_FREQ_RATIO / 2) : $::SRC_CCO_FREQ_RATIO}]
    if {$targetFreq < $::MIN_OUTPUT_FREQ || $::MAX_OUTPUT_FREQ < $targetFreq} {
        error "Invalid FLL target frequency '$targetFreq'. Must be within the range $::MIN_OUTPUT_FREQ-$::MAX_OUTPUT_FREQ."
        return $::ERROR_ARG_VALUE
    }
    if {($sourceFreq * $ratio) >= $targetFreq} {
        error "Invalid FLL source/target frequency ratio.  Target must be at least $ratio faster than the source."
        return $::ERROR_ARG_VALUE
    }
    return $::SUCCESS
}

proc print_arg_count_error {} {
    error "FLL Solver requires 5 arguments in automatic mode:
\t\"auto\" automatic - Use automatic solver based on desired frequency
\tdouble srcFreqMHz - Source clock frequency for the FLL in MHz.
\tdouble targetFreqMHz - Output frequency of the FLL in MHz.
\tboolean wcoIsSource - Is the WCO sourcing the FLL?
\tdouble sourceAccuracy - FLL reference clock accuracy (%)?
FLL Solver requires 7 arguments in manual mode:
\t\"manual\" automatic - Use automatic solver based on desired frequency
\tdouble srcFreqMHz - Source clock frequency for the FLL in MHz.
\tint Mult - Clock frequency multiplier.
\tint Div - Clock frequency divider.
\tint lockTolerance - FLL lock tolerance.
\tboolean wcoIsSource - Is the WCO sourcing the FLL?
\tdouble sourceAccuracy - FLL reference clock accuracy (%)?"
}

solve_fll
