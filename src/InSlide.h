/****
 * @file     InSlide.h
 * 
 * This file is a part of a library for a strange and nearly useless input device that
 * consists of mechanical seven-segment displays whose content the user sets by operating
 * three sliders to form digits in the display. After setting the desired input value in
 * this way, clicking a button reads the set value.
 * 
 * See README.md for details.
 * 
 * @version  Version 1.0.0, October 2022
 *  
 * @author   D. L. Ehnebuske
 * 
 * @section  license
 *
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2022 by D. L. Ehnebuke All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the copyright holders nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 ****/
#pragma once
#ifndef PCINT_VERSION
#include <InSlide.h>
#endif

// Adjustable compile-time parameters
//#define INSL_DEBUG                            // Uncomment to turn on debug printing
#define INSL_MAX_DIGITS           (4)         // The maximum number of digits a device may have.

// Fixed compile-time constants
#define INSL_SENSORS_PER_DIGIT    (7)         // The number of A3144 sensors there are per digit
#define INSL_S_OUT_BITS_PER_DIGIT (8)         // The number of bits in the sOut line per digit
#define INSL_ERROR                (-1)        // Something went wrong; see member function description
#define INSL_TOO_MANY_DIGITS      (-2)        // The device has too many digits to be initialized
#define INSL_INVALID_DIGIT        (-3)        // The digit sliders are not set to show a valid digit 0 ... F

// sensorData values of the digits
//                                   EFGABCD
#define INSL_DIGIT_0              (0b1011010)
#define INSL_DIGIT_1              (0b0001011)
#define INSL_DIGIT_2              (0b1110001)
#define INSL_DIGIT_3              (0b1111011)
#define INSL_DIGIT_4              (0b0101000)
#define INSL_DIGIT_5              (0b1110100)
#define INSL_DIGIT_6              (0b1110110)
#define INSL_DIGIT_7              (0b0011011)
#define INSL_DIGIT_8              (0b1111010)
#define INSL_DIGIT_9              (0b1111000)
#define INSL_DIGIT_A              (0b0111010)
#define INSL_DIGIT_B              (0b1100110)
#define INSL_DIGIT_C              (0b1011110)
#define INSL_DIGIT_D              (0b1101001)
#define INSL_DIGIT_E              (0b1111110)
#define INSL_DIGIT_F              (0b0111110)

class InSlide {
  public:
    /****
     * 
     * InSlide(clk, plBar, sIn, sOut, read)
     *    Constructor: Instantite a new InSlide device whose clock, PL Bar (Parallel Load), 
     *    serial-in, and serial-out lines are connected to the passed GPIO pins.
     * 
     ****/
    InSlide(uint8_t clk, uint8_t plBar, uint8_t sIn, uint8_t sOut);

    /****
     * 
     * begin()
     *   Initialize operation of this InSlide device. Typically called once in the sketch's setup().
     * 
     * @returns number of digits found in the input slider device.
     *    == INSL_ERROR ==> Unable to intitialize device
     *    == INSL_TOO_MANY_DIGITS ==> Unable to detect number of digits during initialization
     * 
     ****/
    int8_t begin();

    /****
     * 
     * digitCount()
     *  Get the number of digits in the display. 
     * 
     * @returns number of digits in the input slider device.
     *    == INSL_ERROR ==> Unable to intitialize device
     *    == INSL_TOO_MANY_DIGITS ==> Unable to detect number of digits during initialization
     * 
     ****/
    int8_t digitCount();

    /****
     * 
     * digitValue(dNo)
     *  Return the decoded value of digit dNo in the device (digit 0, if no dNo passed). The 
     *  recognized values are the hexadecimal digits 0, 1, 2, ... A, b, C, d, E and F. 
     *  Returns INSL_ERROR if the digit is not present in the device (i.e., dNo >= digitCount) or 
     *  that digit is currently set to an invalid configuration.
     * 
     * @returns digit value or error indicator.
     *  If device is not initialized or dNo is out of range, returns INSL_ERROR
     * 
     ****/
    int8_t digitValue(uint8_t dNo = 0);

    /****
     * 
     * sensorState(dNo)
     *  Get the current raw sensor data for digit dNo in the device (digit 0, if no dNo passed). 
     *  Intended as debugging aid.
     * 
     * @returns The raw sensor data for digit dNo. Sensor A is bit 0 (LSB) ... Sensor G is bit 7
     *  If device is not initialized or dNo is out of range, returns INSL_ERROR
     ****/
    int8_t sensorState(uint8_t dNo = 0);

    /****
     * 
     * update()
     *  Read the current state of the device.
     * 
     ****/
    void update();

  private:
    /****
     * 
     * Member variables
     * 
     ****/
    uint8_t clkPin;
    uint8_t plBarPin;
    uint8_t sInPin;
    uint8_t sOutPin;

    int8_t nDigits;                     // Actual number of digits in the InSlide device. < 1 ==> Error
    int8_t sensorData[INSL_MAX_DIGITS]; // Latest raw sensor data for each digit in display.
};