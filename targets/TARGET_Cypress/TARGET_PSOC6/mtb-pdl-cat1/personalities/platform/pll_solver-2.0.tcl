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
    uplevel 1 [list trace var $name w {error constant;} ]
}

const ARG_IDX_SRC_FREQ 0
const ARG_IDX_TARGET_FREQ 1
const ARG_IDX_LF_MODE 2
const ARG_IDX_MIN_POWER 3

const FEEDBACK_DIV_KEY "feedbackDiv"
const REFERENCE_DIV_KEY "referenceDiv"
const OUTPUT_DIV_KEY "outputDiv"

const MIN_IN_FREQ 4
const MAX_IN_FREQ 64
const PFD_MIN 4
const PFD_MAX 8

# PLL OUTPUT_DIV bitfield allowable range
const OUT_MIN 2
const OUT_MAX 16

# PLL REFERENCE_DIV bitfield allowable range
const REF_MIN 1
const REF_MAX 18

# PLL FEEDBACK_DIV bitfield allowable ranges, LF and normal modes
const FB_MAX_LF 56
const FB_MIN 22
const FB_MAX_NORM 112

# PLL Fvco range allowable ranges, LF and normal modes
const VCO_MIN_LF 170
const VCO_MAX_LF 200
const VCO_MIN_NORM 200
const VCO_MAX_NORM 400

const DOUBLE_COMPARE_EPSILON 0.00001
const MAX_DOUBLE 1.0e308

const SUCCESS 0
const ERROR_ARG_COUNT 1
const ERROR_ID 2
const ERROR_ARG_VALUE 3

set channelName stdout

if {[chan names ModusToolbox] eq "ModusToolbox"} {
    set channelName ModusToolbox
}

proc solve_pll {} {
    if {$::argc != $::ARG_IDX_MIN_POWER + 1} {
        error "PLL Solver requires 4 arguments:
\tdouble srcFreqMHz - Source clock frequency for the PLL in MHz.
\tdouble targetFreqMHz - Output frequency of the PLL in MHz.
\tboolean lfMode - CLK_PLL_CONFIG register, PLL_LF_MODE bit.
\tboolean minPower - Optimize for min power rather min jitter."
        return $::ERROR_ARG_COUNT
    }
    set srcFreqMHz [lindex $::argv $::ARG_IDX_SRC_FREQ]
    set targetFreqMHz [lindex $::argv $::ARG_IDX_TARGET_FREQ]
    set lfMode [string is true [lindex $::argv $::ARG_IDX_LF_MODE]]
    set minPower [string is true [lindex $::argv $::ARG_IDX_MIN_POWER]]
    if {![string is double $srcFreqMHz] || ![string is double $targetFreqMHz]} {
        error "Unable to parse argument values: either $srcFreqMHz or $targetFreqMHz is not a floating-point number."
        return $::ERROR_ARG_VALUE
    }
    set srcFreqMHz [expr {double($srcFreqMHz)}]
    set targetFreqMHz [expr {double($targetFreqMHz)}]
    if {$srcFreqMHz < $::MIN_IN_FREQ || $::MAX_IN_FREQ < $srcFreqMHz} {
        error "Invalid PLL input frequency '$srcFreqMHz'. Must be within the range $::MIN_IN_FREQ-$::MAX_IN_FREQ."
        return $::ERROR_ARG_VALUE
    }
    return [solve_pll_internal $srcFreqMHz $targetFreqMHz $lfMode $minPower]
}

proc solve_pll_internal {srcFreqMHz targetFreqMHz lfMode minPower} {
    set divFb $::FB_MIN
    set divRef $::REF_MIN
    set divOut $::OUT_MIN

    set freqRatio [expr {$srcFreqMHz / $targetFreqMHz}]

    set leastErrorFound $::MAX_DOUBLE
    set fbMax [expr {$lfMode ? $::FB_MAX_LF : $::FB_MAX_NORM}]
    for {set p $::FB_MIN} {$p <= $fbMax} {incr p} {
        for {set q $::REF_MIN} {$q <= $::REF_MAX} {incr q} {
            set pfdRefFreq [expr {$srcFreqMHz / $q}]
            set vcoFreq [expr {$pfdRefFreq * $p}]

            if {![is_pfdref_in_range $pfdRefFreq] || ![is_vco_in_range $vcoFreq $lfMode]} {
                continue
            }
            for {set o $::OUT_MIN} {$o <= $::OUT_MAX} {incr o} {
                set error [expr {abs(1 - (($freqRatio * $p) / ($q * $o)))}]
                # If quality is equal, then minPower means minimize divOut, and not minPower means max divOut
                if {$error < $leastErrorFound ||
                    ([double_equals $error $leastErrorFound] &&
                     (($divOut > $o) == $minPower))} {
                    set leastErrorFound $error
                    set divFb $p
                    set divRef $q
                    set divOut $o
                }
            }
        }
    }
    if {$leastErrorFound == $::MAX_DOUBLE} {
        return $::ERROR_ID
    }
    puts $::channelName "param:$::FEEDBACK_DIV_KEY=$divFb"
    puts $::channelName "param:$::REFERENCE_DIV_KEY=$divRef"
    puts $::channelName "param:$::OUTPUT_DIV_KEY=$divOut"
    return $::SUCCESS
}

proc is_vco_in_range {vcoFreq lfMode} {
    return [expr {$lfMode ? ($::VCO_MIN_LF <= $vcoFreq && $vcoFreq <= $::VCO_MAX_LF)
                  : ($::VCO_MIN_NORM <= $vcoFreq && $vcoFreq <= $::VCO_MAX_NORM)}]
}

proc is_pfdref_in_range {pfdRefFrequency} {
    return [expr {$::PFD_MIN <= $pfdRefFrequency && $pfdRefFrequency <= $::PFD_MAX}]
}

proc double_equals {a b} {
    return [expr {abs($a - $b) < $::DOUBLE_COMPARE_EPSILON}]
}

solve_pll
