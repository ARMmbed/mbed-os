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

const ID_SLAVE "Slave"
const ID_MASTER "Master"

const ARG_IDX_MODE 0
const ARG_IDX_DATA_RATE_HZ 1
const ARG_IDX_SCB_CLK_HZ   2

const LOW_OVERSAMPLE  "lowOversample"
const HIGH_OVERSAMPLE "highOversample"
const DIGITAL_FILTER  "digitalFilter"

const DIV_MHZ 1000000
const DIV_KHZ 1000

# Supported data rates ranges
const I2C_DATA_RATE_STD   100000
const I2C_DATA_RATE_FST   400000
const I2C_DATA_RATE_FSTP 1000000

# Master clock (Hz) and duty cycle limits for standard mode
const I2C_MASTER_STD_CLK_MIN_MHZ 1.55
const I2C_MASTER_STD_CLK_MAX_MHZ 3.2
const I2C_MASTER_STD_CLK_MIN_HZ 1550000
const I2C_MASTER_STD_CLK_MAX_HZ 3200000
const I2C_MASTER_STD_SCL_LOW  5000 ;# tLOW  + tF 4700 + 300
const I2C_MASTER_STD_SCL_HIGH 5000 ;# tHIGH + tR 4000 + 1000

# Master clock (Hz) and duty cycle limits for fast mode
const I2C_MASTER_FST_CLK_MIN_MHZ  7.82
const I2C_MASTER_FST_CLK_MAX_MHZ 10.0
const I2C_MASTER_FST_CLK_MIN_HZ  7820000
const I2C_MASTER_FST_CLK_MAX_HZ 10000000
const I2C_MASTER_FST_SCL_LOW  1600 ;# tLOW  + tF 1300 + 300
const I2C_MASTER_FST_SCL_HIGH 900  ;# tHIGH + tR 600 + 300

# Master clock (Hz) and duty cycle limits for fast plus mode
const I2C_MASTER_FSTP_CLK_MIN_MHZ 14.32
const I2C_MASTER_FSTP_CLK_MAX_MHZ 25.8
const I2C_MASTER_FSTP_CLK_MIN_HZ  14320000
const I2C_MASTER_FSTP_CLK_MAX_HZ  25800000
const I2C_MASTER_FSTP_SCL_LOW  620 ;# tLOW  + tF 500 + 120
const I2C_MASTER_FSTP_SCL_HIGH 380 ;# tHIGH + tR 260 + 120

const I2C_LOW_PHASE_MAX  16
const I2C_HIGH_PHASE_MAX 16
const I2C_DUTY_CYCLE_MAX 32

const SUCCESS 0
const ERROR_ARG_COUNT 1
const ERROR_ID 2
const ERROR_ARG_VALUE 3
const ERROR_I2C_FREQ_RANGE 4
const ERROR_DATA_RATE_RANGE 5

set channelName stdout

if {[chan names ModusToolbox] eq "ModusToolbox"} {
    set channelName ModusToolbox
}

proc solve_i2c {} {
    if {$::argc != $::ARG_IDX_SCB_CLK_HZ + 1} {
        error "I2C data rate solver requires 3 arguments:
\tstring id - Master or Slave.
\tinteger dataRateHz - Desired I2C data rate in Hz.
\tinteger scbClockHz - SCB source clock frequency in Hz."
        return $::ERROR_ARG_COUNT
    }
    set mode [lindex $::argv $::ARG_IDX_MODE]
    if {[string compare -nocase $mode $::ID_SLAVE] == 0} {
        return [output_slave_values]
    }
    if {[string compare -nocase $mode $::ID_MASTER] != 0} {
        error "Unhandled solver: $mode"
        return $::ERROR_ID
    }
    set dataRateHz [lindex $::argv $::ARG_IDX_DATA_RATE_HZ]
    set scbClockHz [lindex $::argv $::ARG_IDX_SCB_CLK_HZ]
    if {![string is integer $dataRateHz] || ![string is integer $scbClockHz]} {
        error "Unable to parse argument values: either $dataRateHz or $scbClockHz is not an integer."
        return $::ERROR_ARG_VALUE
    }
    return [master_set_data_rate $dataRateHz $scbClockHz]
}

proc output_results {lowPhase highPhase enableMedian} {
    set digitalFilterStr [expr {$enableMedian ? "true" : "false"}]
    puts $::channelName "param:$::LOW_OVERSAMPLE=$lowPhase"
    puts $::channelName "param:$::HIGH_OVERSAMPLE=$highPhase"
    puts $::channelName "param:$::DIGITAL_FILTER=$digitalFilterStr"
}

proc master_set_data_rate {dataRateHz scbClockHz} {
    set sclLow 0
    set sclHigh 0

    # Calculated values
    set lowPhase 0
    set highPhase 0
    set enableMedian false

    # Get duration of SCL low and high for the selected data rate
    if {$dataRateHz == 0} {
        return $::ERROR_DATA_RATE_RANGE
    }
    if {$dataRateHz <= $::I2C_DATA_RATE_STD} {
        if {$scbClockHz < $::I2C_MASTER_STD_CLK_MIN_HZ || $scbClockHz > $::I2C_MASTER_STD_CLK_MAX_HZ} {
            output_results $lowPhase $highPhase $enableMedian
            return $::ERROR_I2C_FREQ_RANGE
        }
        set sclLow $::I2C_MASTER_STD_SCL_LOW
        set sclHigh $::I2C_MASTER_STD_SCL_HIGH
    } elseif {$dataRateHz <= $::I2C_DATA_RATE_FST} {
        if {$scbClockHz < $::I2C_MASTER_FST_CLK_MIN_HZ || $scbClockHz > $::I2C_MASTER_FST_CLK_MAX_HZ} {
            output_results $lowPhase $highPhase $enableMedian
            return $::ERROR_I2C_FREQ_RANGE
        }
        set sclLow $::I2C_MASTER_FST_SCL_LOW
        set sclHigh $::I2C_MASTER_FST_SCL_HIGH
    } elseif {$dataRateHz <= $::I2C_DATA_RATE_FSTP} {
        if {$scbClockHz < $::I2C_MASTER_FSTP_CLK_MIN_HZ || $scbClockHz > $::I2C_MASTER_FSTP_CLK_MAX_HZ} {
            output_results $lowPhase $highPhase $enableMedian
            return $::ERROR_I2C_FREQ_RANGE
        }
        set sclLow $::I2C_MASTER_FSTP_SCL_LOW
        set sclHigh $::I2C_MASTER_FSTP_SCL_HIGH
        set enableMedian true
    } else {
        output_results $lowPhase $highPhase $enableMedian
        return $::ERROR_I2C_FREQ_RANGE
    }
    set updateLowPhase false
    set actualDataRateHz 0

    # Get period of the SCB clock in ns
    set period [expr {1000000000 / $scbClockHz}]

    # Get low phase minimum value in SCB clocks
    set lowPhase [expr {$sclLow / $period}]

    if {($period * $lowPhase) < $sclLow} {
        incr lowPhase
    }
    if {$lowPhase > $::I2C_LOW_PHASE_MAX} {
        set lowPhase $::I2C_LOW_PHASE_MAX
    }

    # Define if updateLowPhase low phase
    set updateLowPhase [expr {$lowPhase < $::I2C_LOW_PHASE_MAX}]

    # Get high phase minimum value in SCB clocks
    set highPhase [expr {$sclHigh / $period}]

    if {($period * $highPhase) < $sclHigh} {
        incr highPhase
    }
    if {$highPhase > $::I2C_HIGH_PHASE_MAX} {
        set highPhase $::I2C_HIGH_PHASE_MAX
    }

    # Get actual data rate
    set actualDataRateHz [expr {$scbClockHz / ($lowPhase + $highPhase)}]

    # Find desired data rate
    while {($actualDataRateHz > $dataRateHz) && (($lowPhase + $highPhase) < $::I2C_DUTY_CYCLE_MAX)} {
        # Increase low and high phase to reach desired data rate
        if {$updateLowPhase} {
            if {$lowPhase < $::I2C_LOW_PHASE_MAX} {
                # Update low phase
                incr lowPhase
                set updateLowPhase false
            }
        } elseif {$highPhase < $::I2C_HIGH_PHASE_MAX} {
            # Update high phase
            incr highPhase
            set updateLowPhase [expr {$lowPhase < $::I2C_LOW_PHASE_MAX}]
        }

        # Update actual data rate
        set actualDataRateHz [expr {$scbClockHz / ($lowPhase + $highPhase)}]
    }

    output_results $lowPhase $highPhase $enableMedian
    return $::SUCCESS
}

proc output_slave_values {} {
    output_results 0 0 false
    return $::SUCCESS
}

solve_i2c
