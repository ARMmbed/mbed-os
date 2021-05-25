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

const DEFAULT_RESOLUTION 12
const MIN_SAMPLE_TIME_NS 83.0
const OTHER_ADC_CLOCKS 2

# Actual sample time is 1/2 clock less than specified
# for PSoC4 and 1 clock less for PSoC 6.
# See SAR_v2 SAMPLE_TIME01 register description.
const ADC_CLOCKS_NOT_SAMPLING 1

const APERTURE_TIMER_COUNT 4     ;# There are four timers in the SAR
const APERTURE_TIMER_MIN 2       ;# The minimum value for each timer is 2 clocks
const APERTURE_TIMER_MAX 1023

# ADC clock constants. These are the outer hardware limits.
# Actual limits depend on the configuration and may be tighter.
# See AdcClockFreqMin_Hz and AdcClockFreqMax_Hz properties.
const ADC_CLOCK_MAX_HZ 60000000
const ADC_CLOCK_MIN_HZ 1000000

const MIN_ADC_CLOCK_DIV 2

const MAX_NUM_CHANNELS 16

const ARG_IDX_HF_CLK_RATE 0
const ARG_IDX_IS_FIXED_CLOCK 1
const ARG_IDX_CLK_RATE 2
const ARG_IDX_MIN_CLK_RATE 3
const ARG_IDX_MAX_CLK_RATE 4
const ARG_IDX_IS_SINGLE_SHOT 5
const ARG_IDX_SAMPLE_RATE 6
const ARG_IDX_NUM_CHANNELS 7
const ARG_IDX_CHANNEL_INFO 8

const NUM_ARGS_PER_CHANNEL 2

const SUCCESS 0
const ERROR_ARG_COUNT 1
const ERROR_ID 2
const ERROR_ARG_VALUE 3
const ERROR_FAIL_SCHEDULING 4

array set SAMPLE_PER_SCAN_MAP {
    "CY_SAR_AVG_CNT_2" {2}
    "CY_SAR_AVG_CNT_4" {4}
    "CY_SAR_AVG_CNT_8" {8}
    "CY_SAR_AVG_CNT_16" {16}
    "CY_SAR_AVG_CNT_32" {32}
    "CY_SAR_AVG_CNT_64" {64}
    "CY_SAR_AVG_CNT_128" {128}
    "CY_SAR_AVG_CNT_256" {256}
}

# Indices for the list-based implementation of the CyTimingInfo structure
const TIMING_IDEAL_SAMPLE_ADC_CLOCK 0
const TIMING_SAMPLE_COUNT 1
const TIMING_IS_ENABLED 2
const TIMING_CHANNEL_NUM 3

proc construct_timing_info {minAcqAdcClocksNeeded samplesPerScan enabled chanNum} {
    # Note: the order of this list must match the above indices
    return [list $minAcqAdcClocksNeeded $samplesPerScan $enabled $chanNum]
}

# Indices for the list-based implementation of the CySchedChan structure
# Input properties
const CHAN_MIN_ACQ_TIME_NS 0
const CHAN_SAMPLES_PER_SCAN 1
const CHAN_IS_ENABLED 2
const CHAN_RESOLUTION 3
# Output properties
const CHAN_TIMER 4
const CHAN_ACHIEVED_ACQ_TIME_NS 5
const CHAN_ACHIEVED_SAMPLE_TIME_NS 6
const CHAN_MIN_ACQ_ADC_CLOCKS_NEEDED 7    ;# Minimum number of ADC clocks to acquire the channel
const CHAN_MIN_CONV_ADC_CLOCKS_NEEDED 8   ;# Minimum number of ADC clocks needed for a conversion
const CHAN_MIN_TOTAL_ADC_CLOCKS_NEEDED 9  ;# Minimum number of ADC clocks needed for sampling (acquisition + conversion)
const CHAN_MAPPED_ACQ_ADC_CLOCKS 10       ;# Actual number of ADC clocks to acquire the channel
const CHAN_MAPPED_TOTAL_ADC_CLOCKS 11     ;# Actual number of ADC clocks needed for sampling (acquisition + conversion)

proc construct_sched_chan {acqTime samples} {
    # Note: the order of this list must match the above indices
    return [list [expr {max2($acqTime, $::MIN_SAMPLE_TIME_NS)}] $samples [expr {$samples != 0}] $::DEFAULT_RESOLUTION \
                0 0.0 0.0 0 [expr {$::DEFAULT_RESOLUTION + $::OTHER_ADC_CLOCKS}] 0 0 0]
}

set hfClockRate 0.0
set isFixedClock false
set fixedAdcClockRate 0.0
set minClockRate 0.0
set maxClockRate 0.0
set isSingleShot False
set targetSampleRate 0.0
set numChannels 0
set channels {}
set adcClockDivider 0.0

set aperturesAdcClock [lrepeat $::APERTURE_TIMER_COUNT $::APERTURE_TIMER_MIN]
set achievedSampleRate 0
set achievedScanPeriod_us 0.0
set cost 0

set min_aperture $::APERTURE_TIMER_MAX
set min_sample_time_sel 0
set inj_sample_time_sel ""
set injMinAcqTimeNs 0
set injSamplesPerScan 0
set inj_achieved_acq_time 0.0
set inj_achieved_sample_time 0.0

set channelName stdout

if {[chan names ModusToolbox] eq "ModusToolbox"} {
    set channelName ModusToolbox
}

namespace eval tcl {
    namespace eval mathfunc {
        proc min2 {arg1 arg2} {
            if {$arg1 < $arg2} {
                return $arg1
            } else {
                return $arg2
            }
        }

        proc max2 {arg1 arg2} {
            if {$arg1 > $arg2} {
                return $arg1
            } else {
                return $arg2
            }
        }
    }
}

proc schedule_sar {} {
    if {$::argc < $::ARG_IDX_NUM_CHANNELS + 1} {
        error "Not enough base arguments."
        return $::ERROR_ARG_COUNT
    }
    set ::hfClockRate [lindex $::argv $::ARG_IDX_HF_CLK_RATE]
    set ::isFixedClock [string is true [lindex $::argv $::ARG_IDX_IS_FIXED_CLOCK]]
    set ::fixedAdcClockRate [lindex $::argv $::ARG_IDX_CLK_RATE]
    set ::minClockRate [lindex $::argv $::ARG_IDX_MIN_CLK_RATE]
    set ::maxClockRate [lindex $::argv $::ARG_IDX_MAX_CLK_RATE]
    set ::isSingleShot [string is true [lindex $::argv $::ARG_IDX_IS_SINGLE_SHOT]]
    set ::targetSampleRate [lindex $::argv $::ARG_IDX_SAMPLE_RATE]
    set ::numChannels [lindex $::argv $::ARG_IDX_NUM_CHANNELS]
    if {![string is double $::hfClockRate] || ![string is double $::fixedAdcClockRate] ||
        ![string is double $::minClockRate] || ![string is double $::maxClockRate] ||
        ![string is double $::targetSampleRate]} {
        error "Unable to parse argument values: One of $::hfClockRate, $::fixedAdcClockRate, $::minClockRate, $::maxClockRate, $::targetSampleRate
is not a floating-point number."
        return $::ERROR_ARG_VALUE
    }
    set ::hfClockRate [expr {double($::hfClockRate)}]
    set ::fixedAdcClockRate [expr {double($::fixedAdcClockRate)}]
    set ::minClockRate [expr {double($::minClockRate)}]
    set ::maxClockRate [expr {double($::maxClockRate)}]
    set ::targetSampleRate [expr {double($::targetSampleRate)}]
    if {![string is integer $::numChannels]} {
        error "Unable to parse argument values: $::numChannels is not an integer."
        return $::ERROR_ARG_VALUE
    }
    if {$::argc < $::ARG_IDX_CHANNEL_INFO + $::numChannels * $::NUM_ARGS_PER_CHANNEL + $::NUM_ARGS_PER_CHANNEL} {
        error "Not enough channel information."
        return $::ERROR_ARG_COUNT
    }
    for {set i 0} {$i < $::numChannels} {incr i} {
        set minAcqTimeNs [lindex $::argv [expr {$::ARG_IDX_CHANNEL_INFO + $i * $::NUM_ARGS_PER_CHANNEL}]]
        set samplesPerScan [get_samples_per_scan [lindex $::argv [expr {$::ARG_IDX_CHANNEL_INFO + $i * $::NUM_ARGS_PER_CHANNEL + 1}]]]
        if {![string is double $minAcqTimeNs]} {
            error "Unable to parse argument values: $minAcqTimeNs is not a floating-point number."
            return $::ERROR_ARG_VALUE
        }
        set minAcqTimeNs [expr {double($minAcqTimeNs)}]
        lappend ::channels [construct_sched_chan $minAcqTimeNs $samplesPerScan]
    }

    set ::injMinAcqTimeNs [lindex $::argv [expr {$::ARG_IDX_CHANNEL_INFO + 16 * $::NUM_ARGS_PER_CHANNEL}]]
    set ::injSamplesPerScan [get_samples_per_scan [lindex $::argv [expr {$::ARG_IDX_CHANNEL_INFO + 16 * $::NUM_ARGS_PER_CHANNEL + 1}]]]

    set adcClockRate $::fixedAdcClockRate
    if {!$::isFixedClock} {
        set ::adcClockDivider [select_adc_clock_divider]
        set adcClockRate [expr {$::hfClockRate / $::adcClockDivider}]
    }
    if {$::isSingleShot} {
        set ::solution [schedule_adc_one_shot $adcClockRate]
    } else {
        set ::solution [schedule_adc_only_with_fixed_adc_clock $adcClockRate]
    }

    # If scan period <= 0, scheduling did not succeed.
    set scanPeriodAdcClock [find_mapped_scan_adc_clocks]
    if {$scanPeriodAdcClock <= 0} {
        error "Scheduling failed."
        return $::ERROR_FAIL_SCHEDULING
    }
    set ::achievedSampleRate [expr {int(round($adcClockRate / $scanPeriodAdcClock))}]
    set ::achievedScanPeriod_us [expr {1e6 / $::achievedSampleRate}]
    for {set chanNum 0} {$chanNum < [llength $::channels]} {incr chanNum} {
        set adcClocks [expr {[lindex $::channels $chanNum $::CHAN_MAPPED_ACQ_ADC_CLOCKS] - $::ADC_CLOCKS_NOT_SAMPLING}]
        lset ::channels $chanNum $::CHAN_ACHIEVED_ACQ_TIME_NS [expr {round(1e9 * $adcClocks / $adcClockRate)}]
        lset ::channels $chanNum $::CHAN_ACHIEVED_SAMPLE_TIME_NS [expr {round(1e9 * [lindex $::channels $chanNum $::CHAN_MAPPED_TOTAL_ADC_CLOCKS] / $adcClockRate)}]
    }
    
    for {set sampleTime 0} {$sampleTime < [llength $::aperturesAdcClock]} {incr sampleTime} {
        set aperture [lindex $::aperturesAdcClock $sampleTime]
        if {$aperture >= $::APERTURE_TIMER_MIN} {
            puts $::channelName [format "param:sample_time_%d=%d" $sampleTime $aperture]
            if {$aperture < $::min_aperture} {
                set ::min_aperture $aperture
                set ::min_sample_time_sel $sampleTime
            }
        } else {
        puts $::channelName [format "param:sample_time_%d=%d" $sampleTime $::APERTURE_TIMER_MIN]
        }
    }

    puts $::channelName [format "param:inj_aperture=%d" $::min_aperture]

    set injMappedTotalAdcClocks [expr {($::min_aperture + [expr {$::DEFAULT_RESOLUTION + $::OTHER_ADC_CLOCKS}]) * $::injSamplesPerScan}]
    set ::inj_achieved_acq_time [expr {round(1e9 * [expr {$::min_aperture - $::ADC_CLOCKS_NOT_SAMPLING}] / $adcClockRate)}]
    set ::inj_achieved_sample_time [expr {round(1e9 * $injMappedTotalAdcClocks / $adcClockRate)}]

    write_output
    return $::SUCCESS
}

proc get_samples_per_scan {arg} {
    if {[info exists ::SAMPLE_PER_SCAN_MAP($arg)]} {
        return $::SAMPLE_PER_SCAN_MAP($arg)
    } elseif {[string is integer $arg]} {
        return $arg
    } else {
        error "Unable to parse argument values: $arg is not an integer."
        exit $::ERROR_ARG_VALUE
    }
}

proc write_output {} {
    puts $::channelName "param:achieved_sample_rate=$::achievedSampleRate"
    puts $::channelName [format "param:achieved_sample_period=%.2f us" $::achievedScanPeriod_us]
    puts $::channelName [format "param:adc_clock_divider=%.1f" $::adcClockDivider]

    for {set chanNum 0} {$chanNum < $::MAX_NUM_CHANNELS} {incr chanNum} {
        set ch_achieved_acq_time 0
        set ch_achieved_sample_time 0
        set ch_sample_time_sel ""
        if {$chanNum < [llength $::channels]} {
            set ch_achieved_acq_time [expr {int([lindex $::channels $chanNum $::CHAN_ACHIEVED_ACQ_TIME_NS])}]
            set ch_achieved_sample_time [expr {int([lindex $::channels $chanNum $::CHAN_ACHIEVED_SAMPLE_TIME_NS])}]
            set ch_sample_time_sel [format "CY_SAR_CHAN_SAMPLE_TIME_%d" [lindex $::channels $chanNum $::CHAN_TIMER]]
        }
        puts $::channelName [format "param:ch%d_achieved_acq_time=%d ns" $chanNum $ch_achieved_acq_time]
        puts $::channelName [format "param:ch%d_achieved_sample_time=%d ns" $chanNum $ch_achieved_sample_time]
        puts $::channelName [format "param:ch%d_sample_time_sel=%s" $chanNum $ch_sample_time_sel]
    }

    puts $::channelName [format "param:sample_time_min=%d" $::min_aperture]
    set inj_sample_time_sel [format "CY_SAR_CHAN_SAMPLE_TIME_%d" $::min_sample_time_sel]
    puts $::channelName [format "param:inj_sample_time_sel=%s" $inj_sample_time_sel]
    puts $::channelName [format "param:inj_achieved_acq_time=%d ns" $::inj_achieved_acq_time]
    puts $::channelName [format "param:inj_achieved_sample_time=%d ns" $::inj_achieved_sample_time]
}

proc schedule_adc_only_with_fixed_adc_clock {adcClockRate} {
    # This method must be called to initialize channel sample and conversion ADC clock counts.
    find_min_scan_adc_clocks $adcClockRate

    # Set up data structures for optimization without padding.
    set chanTimesNoPad [lsort -dictionary [get_channel_timing_info]]

    set apertureClocksNoPad $::aperturesAdcClock

    # Set up data structures for optimization with padding.
    set chanTimesPad $chanTimesNoPad
    set apertureClocksPad $apertureClocksNoPad
    set padIndex [find_padding_channel $chanTimesPad]
    if {$padIndex != 0} {
        set padChan [lindex $::chanTimesPad $padIndex]
        set ::chanTimesPad [lreplace $::chanTimesPad $padIndex $padIndex]
        set ::chanTimesPad [linsert $::chanTimesPad 0 $padChan]
    }

    # Find solution with minimum ADC clocks without padding.
    optimize_apertures "chanTimesNoPad" 0 [llength $chanTimesNoPad] "apertureClocksNoPad" 0 [llength $apertureClocksNoPad]

    set ::aperturesAdcClock $apertureClocksNoPad
    set_channel_timers 0 [llength $::aperturesAdcClock]
    set mappedMinScanAdcClocksNoPad [find_mapped_scan_adc_clocks]

    # Find solution with minimum ADC clocks, reserving one aperture timer for padding.
    optimize_apertures "chanTimesPad" 1 [llength $chanTimesPad] "apertureClocksPad" 1 [llength $apertureClocksPad]

    lset apertureClocksPad 0 [lindex $chanTimesPad 0 $::TIMING_IDEAL_SAMPLE_ADC_CLOCK]

    set ::aperturesAdcClock $apertureClocksPad
    set_channel_timers 1 [llength $::aperturesAdcClock]
    lset ::channels [lindex $chanTimesPad 0 $::TIMING_CHANNEL_NUM] $::CHAN_TIMER 0
    set mappedMinScanAdcClocksPad [find_mapped_scan_adc_clocks]

    set targetScanAdcClocks [adc_clocks $::targetSampleRate $adcClockRate]

    if {$mappedMinScanAdcClocksPad <= $targetScanAdcClocks} {
        # What if more than one channel needs to be padded?
        set padAmount [expr {$targetScanAdcClocks - $mappedMinScanAdcClocksPad}]
        set samplesPerScan [lindex $::channels [lindex $chanTimesPad 0 $::TIMING_CHANNEL_NUM] $::CHAN_SAMPLES_PER_SCAN]
        if {$samplesPerScan > 1} {
            set padAmount [expr {($padAmount + ($samplesPerScan / 2)) / $samplesPerScan}]
        }
        lset ::aperturesAdcClock 0 [expr {min2([lindex $::aperturesAdcClock 0] + $padAmount, $::APERTURE_TIMER_MAX)}]
    } elseif {abs($mappedMinScanAdcClocksNoPad - $targetScanAdcClocks) < abs($mappedMinScanAdcClocksPad - $targetScanAdcClocks)} {
        # If unpadded timers overshoot less than padded timers, switch to unpadded timers.
        set ::aperturesAdcClock $apertureClocksNoPad
        set_channel_timers 0 [llength $::aperturesAdcClock]
    }
    # else, stick with padded timers without adding padding.
}

proc schedule_adc_one_shot {adcClockRate} {
    # This method must be called to initialize channel sample and conversion ADC clock counts.
    find_min_scan_adc_clocks $adcClockRate

    # Set up data structures for optimization without padding.
    set chanTimesNoPad [lsort -dictionary [get_channel_timing_info]]

    # Find solution with minimum ADC clocks without padding.
    optimize_apertures "chanTimesNoPad" 0 [llength $chanTimesNoPad] "::aperturesAdcClock" 0 [llength $::aperturesAdcClock]

    set_channel_timers 0 [llength $::aperturesAdcClock]
}

# Find the minimum number of ADC clocks required to scan all channels.
# Also stores the minimum number of ADC clocks in the channel table.
# Assumes each channel has its own aperture timer.
proc find_min_scan_adc_clocks {adcClockRate} {
    set minScanAdcClocks 0
    set newChannels {}

    foreach chan $::channels {
        lset chan $::CHAN_MIN_ACQ_ADC_CLOCKS_NEEDED [aperture_adc_clocks [lindex $chan $::CHAN_MIN_ACQ_TIME_NS] $adcClockRate]
        if {[lindex $chan $::CHAN_IS_ENABLED]} {
            lset chan $::CHAN_MIN_TOTAL_ADC_CLOCKS_NEEDED [expr {[lindex $chan $::CHAN_MIN_ACQ_ADC_CLOCKS_NEEDED] + [lindex $chan $::CHAN_MIN_CONV_ADC_CLOCKS_NEEDED] * [lindex $chan $::CHAN_SAMPLES_PER_SCAN]}]
        } else {
            lset chan $::CHAN_MIN_TOTAL_ADC_CLOCKS_NEEDED 0
        }
        incr minScanAdcClocks [lindex $chan $::CHAN_MIN_TOTAL_ADC_CLOCKS_NEEDED]
        lappend newChannels $chan
    }
    set ::channels $newChannels
    return $minScanAdcClocks
}

# Calculate number of ADC clocks required for specified sample time.
# Number of ADC clocks required to cover sample time
proc aperture_adc_clocks {apertureNs adcClockRate} {
    # Convert to ns.
    set adcClockNs [expr {1.0e9 / $adcClockRate}]

    # Find number of clocks required to cover aperture period.
    # Down 1/2 ns for rounding (the ceiling function below will adjust this)
    set apertureClocks [expr {$apertureNs / $adcClockNs + $::ADC_CLOCKS_NOT_SAMPLING - (0.5 / $adcClockNs)}]

    # Convert to integral number of clocks and enforce limits
    set clocks [expr {int(ceil($apertureClocks))}]

    # Enforce limits.
    set clocks [expr {min2($clocks, $::APERTURE_TIMER_MAX)}]
    set clocks [expr {max2($clocks, $::APERTURE_TIMER_MIN)}]

    return $clocks
}

# Extract timing info for optimization from channel data.
proc get_channel_timing_info {} {
    set chanTimings {}

    for {set chanNum 0} {$chanNum < [llength $::channels]} {incr chanNum} {
        set chan [lindex $::channels $chanNum]
        lappend chanTimings [construct_timing_info [lindex $chan $::CHAN_MIN_ACQ_ADC_CLOCKS_NEEDED] [lindex $chan $::CHAN_SAMPLES_PER_SCAN] [lindex $chan $::CHAN_IS_ENABLED] $chanNum]
    }
    return $chanTimings
}

# This function (in the original Java code) always returns 0.
# TODO: Make it do something useful.
proc compare_padding_merit {chanTimes $bestPadIndex $padIndex} {
    return 0
}

# Find best channel in range to use for ADC clock padding.
# Prefer channels with no averaging or minimum averaging and lowest ideal ADC clock count.
# The chanTimes list must be sorted.
proc find_padding_channel {chanTimes} {
    set bestPadIndex 0
    for {set padIndex 1} {$padIndex < [llength $chanTimes]} {incr padIndex} {
        if {[compare_padding_merit $chanTimes $bestPadIndex $padIndex] > 0} {
            set bestPadIndex $padIndex
        }
    }
    return $bestPadIndex
}

# Recursive min cost optimizer.
#
# "Cost" is the difference between the number of ADC clocks used
# by aperture timers to cover the given channels and the number of ADC
# clocks that would be required if each channel had its own timer.
#
# The channel timing info list must be sorted.
#
# Each recursive call consumes one aperture timer and zero or more
# channels. The recursion terminates when there are either no timers
# or no channels left.
#
# Returns min cost timer settings in apertureClocks list.
proc optimize_apertures {chanTimesName chanBase chanTop apertureClocksName apertureBase apertureTop} {
    upvar $chanTimesName chanTimes            ;# "passed by reference", tcl style
    upvar $apertureClocksName apertureClocks  ;# "passed by reference", tcl style
    if {$chanTop == $chanBase} {
        # All channels have been processed. Zero out remainder of timer list.
        for {set index $apertureBase} {$index < $apertureTop} {incr index} {
            lset apertureClocks $index 0
        }
        return 0
    }
    if {$apertureBase + 1 == $apertureTop} {
        # There is only one timer left.  Use it to cover all the remaining channels.
        set apertureTime [lindex $chanTimes [expr {$chanTop - 1}] $::TIMING_IDEAL_SAMPLE_ADC_CLOCK]
        lset apertureClocks $apertureBase $apertureTime
        return [calc_aperture_cost $chanTimes $chanBase $chanTop $apertureTime]
    }

    # Loop over possible timer values making recursive calls.  Save the best found.
    set minCost 9876543210
    set minClocks $apertureClocks

    set chanNum $chanBase
    while {$chanNum < $chanTop} {
        set apertureTime [lindex $chanTimes $chanNum $::TIMING_IDEAL_SAMPLE_ADC_CLOCK]
        set nextChanNum $chanNum
        while {$nextChanNum < $chanTop && [lindex $chanTimes $nextChanNum $::TIMING_IDEAL_SAMPLE_ADC_CLOCK] == $apertureTime} {
            incr nextChanNum
        }

        set branchClocks $apertureClocks
        set branchCost [expr {[calc_aperture_cost $chanTimes $chanBase $nextChanNum $apertureTime]
                              + [optimize_apertures "chanTimes" $nextChanNum $chanTop "branchClocks" [expr {$apertureBase + 1}] $apertureTop]}]
        if {$minCost > $branchCost} {
            set minCost $branchCost
            set minClocks [lreplace $branchClocks $apertureBase $apertureBase $apertureTime]
        }

        set chanNum $nextChanNum
    }

    for {set index $apertureBase} {$index < $apertureTop} {incr index} {
        lset apertureClocks $index [lindex $minClocks $index]
    }
    return $minCost
}

# Calculate cost of covering the specified channels with one timer setting.
# Requires channel ideal sample times to have been calculated.
proc calc_aperture_cost {chanTimes chanBase chanTop aperture} {
    set cost 0

    for {set chanNum $chanBase} {$chanNum < $chanTop} {incr chanNum} {
        set chan [lindex $chanTimes $chanNum]
        set chanExcess [expr {($aperture - [lindex $chan $::TIMING_IDEAL_SAMPLE_ADC_CLOCK]) * [lindex $chan $::TIMING_SAMPLE_COUNT]}]
        incr cost $chanExcess
    }
    return $cost
}

# Assign best fit aperture timers to specified channels.
proc set_channel_timers {timerBase timerTop} {
    for {set chanNum 0} {$chanNum < [llength $::channels]} {incr chanNum} {
        for {set timer $timerBase} {$timer < $timerTop} {incr timer} {
            if {[lindex $::channels $chanNum $::CHAN_MIN_ACQ_ADC_CLOCKS_NEEDED] <= [lindex $::aperturesAdcClock $timer]} {
                lset ::channels $chanNum $::CHAN_TIMER $timer
                break
            }
        }
    }
}

# Find number of ADC clocks required for scan with specified aperture timers.
proc find_mapped_scan_adc_clocks {} {
    set mappedScanAdcClocks 0

    for {set chanNum 0} {$chanNum < [llength $::channels]} {incr chanNum} {
        set mappedAcqAdcClocks [lindex $::aperturesAdcClock [lindex $::channels $chanNum $::CHAN_TIMER]]
        lset ::channels $chanNum $::CHAN_MAPPED_ACQ_ADC_CLOCKS $mappedAcqAdcClocks
        lset ::channels $chanNum $::CHAN_MAPPED_TOTAL_ADC_CLOCKS \
            [expr {($mappedAcqAdcClocks + [lindex $::channels $chanNum $::CHAN_MIN_CONV_ADC_CLOCKS_NEEDED]) * [lindex $::channels $chanNum $::CHAN_SAMPLES_PER_SCAN]}]

        incr mappedScanAdcClocks [lindex $::channels $chanNum $::CHAN_MAPPED_TOTAL_ADC_CLOCKS]
    }
    return $mappedScanAdcClocks
}

# Find number of ADC clocks required for target scan frequency.
proc adc_clocks {targetFrequency adcClockRate} {
    if {$targetFrequency == 0} {
        return 2147483647   ;# This is the expected behavior of casting Infinity to an int, according to the Java version
    }
    return [expr {int(ceil($adcClockRate / $targetFrequency))}]
}

# Calculate ADC clock divider for internal ADC clocks.
# Results are stored in the adcClockDivider property.
proc select_adc_clock_divider {} {
    if {$::isSingleShot} {
        set ::adcClockDivider $::MIN_ADC_CLOCK_DIV
        return
    }
    # Find minimum sample time and ADC conversion clock count for scan.
    set minScan [find_adc_clock_parameters]
    set minScanSample_ns [lindex $minScan 0]
    set minScanConvAdcClock [lindex $minScan 1]

    set minScanSampleTime_s [expr {$minScanSample_ns / 1.0e9}]

    # Select a clock divider that will use about 512 padding clock periods to hit the target clock rate.
    set targetScanPeriod_s [expr {1.0 / $::targetSampleRate}]
    set allowedConversionTime_s [expr {$targetScanPeriod_s - $minScanSampleTime_s}]
    set adcClockHz [expr {($minScanConvAdcClock + 512) / $allowedConversionTime_s}]
    set ::adcClockDivider [expr {int(round($::hfClockRate / $adcClockHz))}]
    set maxDivider [expr {int(floor($::hfClockRate / $::minClockRate))}]
    set minDivider [expr {int(ceil($::hfClockRate / $::maxClockRate))}]
    set ::adcClockDivider [expr {min2($::adcClockDivider, $maxDivider)}]
    set ::adcClockDivider [expr {max2($::adcClockDivider, $minDivider)}]
    set ::adcClockDivider [expr {max2($::adcClockDivider, $::MIN_ADC_CLOCK_DIV)}]
}

proc find_adc_clock_parameters {} {
    set minScanSampleNs 0
    set minScanConvAdcClocks 0

    foreach chan $::channels {
        set minSampleNs [lindex $chan $::CHAN_MIN_ACQ_TIME_NS]
        set minConvAdcClocks [lindex $chan $::CHAN_MIN_CONV_ADC_CLOCKS_NEEDED]
        if {[lindex $chan $::CHAN_SAMPLES_PER_SCAN] > 1} {
            set minSampleNs [expr {$minSampleNs * [lindex $chan $::CHAN_SAMPLES_PER_SCAN]}]
            set minConvAdcClocks [expr {$minConvAdcClocks * [lindex $chan $::CHAN_SAMPLES_PER_SCAN]}]
            incr minScanSampleNs $minSampleNs
            incr minScanConvAdcClocks $minConvAdcClocks
        }
    }
    return [list $minScanSampleNs $minScanConvAdcClocks]
}

schedule_sar
