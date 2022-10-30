/****
 * @file     InSlide.cpp
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
#include <Arduino.h>
#include <InSlide.h>

InSlide::InSlide(uint8_t clk, uint8_t plBar, uint8_t sIn, uint8_t sOut) {
    clkPin = clk;
    plBarPin = plBar;
    sInPin = sIn;
    sOutPin = sOut;

    nDigits = -1;           // Device not initialized
}

int8_t InSlide::begin(){
    // Initialize the GPIO pins
    pinMode(clkPin, OUTPUT);        // The InSlide clock line we drive as needed
    digitalWrite(clkPin, LOW);
    pinMode(plBarPin, OUTPUT);      // The InSlide device inverse Parallel Load line we drive as needed.
    digitalWrite(plBarPin, HIGH);
    pinMode(sInPin, OUTPUT);        // The InSlide serial input line we drive as needed.
    digitalWrite(sInPin, LOW);
    pinMode(sOutPin, INPUT);        // The Inslide serial output line we read as needed.

    // Determine the number of digits there are in the device. 

    // Fill the device with zeroes.
    for (uint16_t bitCount = 0; bitCount < INSL_MAX_DIGITS * INSL_S_OUT_BITS_PER_DIGIT; bitCount++) {
        digitalWrite(clkPin, HIGH);
        digitalWrite(clkPin, LOW);
    }
    // Put ones in and see how many clk cycles until the first one comes out sOut
    digitalWrite(sInPin, HIGH);
    uint8_t bitOut = LOW;
    uint16_t bitCount;
    for (bitCount = 0; bitCount < INSL_MAX_DIGITS * INSL_S_OUT_BITS_PER_DIGIT; bitCount++) {
        digitalWrite(clkPin, HIGH);
        digitalWrite(clkPin, LOW);
        bitOut = digitalRead(sOutPin);
        if (bitOut == HIGH) {
            break;
        }
    }
    bitCount++;
    if (bitOut == LOW) {
        #ifdef INSL_DEBUG
        Serial.println(F("InSlide: Unable to detect how many digits in device."));
        #endif
        nDigits = INSL_TOO_MANY_DIGITS;
    } else if (bitCount % INSL_S_OUT_BITS_PER_DIGIT != 0) {
        #ifdef INSL_DEBUG
        Serial.print(F("InSlide: Got a 1-bit at bit count "));
        Serial.print(bitCount);
        Serial.println(F(". That's not evenly divisible by the number of bits per digit."));
        #endif
        nDigits = INSL_ERROR;
    } else {
        nDigits = bitCount / INSL_S_OUT_BITS_PER_DIGIT;
        #ifdef INSL_DEBUG
        Serial.print(F("InSlide: Initialized device with "));
        if (nDigits == 1) {
            Serial.println(F("one digit."));
        } else {
            Serial.print(nDigits);
            Serial.println(F(" digits."));
        }
        #endif
    }
    return nDigits;
}

int8_t InSlide::digitCount() {
    return nDigits;
}

int8_t InSlide::digitValue(uint8_t dNo) {
    if (dNo < 0 || dNo >= nDigits) {
        return INSL_ERROR;
    }
    switch (sensorData[dNo]) {
        case INSL_DIGIT_0:
            return 0;
        case INSL_DIGIT_1:
            return 1;
        case INSL_DIGIT_2:
            return 2;
        case INSL_DIGIT_3:
            return 3;
        case INSL_DIGIT_4:
            return 4;
        case INSL_DIGIT_5:
            return 5;
        case INSL_DIGIT_6:
            return 6;
        case INSL_DIGIT_7:
            return 7;
        case INSL_DIGIT_8:
            return 8;
        case INSL_DIGIT_9:
            return 9;
        case INSL_DIGIT_A:
            return 0x0A;
        case INSL_DIGIT_B:
            return 0x0B;
        case INSL_DIGIT_C:
            return 0x0C;
        case INSL_DIGIT_D:
            return 0x0D;
        case INSL_DIGIT_E:
            return 0x0E;
        case INSL_DIGIT_F:
            return 0x0F;
        default:
            #ifdef INSL_DEBUG
            Serial.print(F("inSlide: digit number "));
            Serial.print(dNo);
            Serial.print(F(" is not a well formed digit. Sensor data: "));
            Serial.print(sensorData[dNo] < 0x10 ? F("0x0") : F("0x"));
            Serial.println(sensorData[dNo], HEX);
            #endif
            return INSL_ERROR;
    }
}

int8_t InSlide::sensorState(uint8_t dNo) {
    if (nDigits < 1 || dNo >= nDigits)  {
        return INSL_ERROR;
    }
    return sensorData[dNo];
}

void InSlide::update(){
    if (nDigits < 1) {
        return;
    }
    digitalWrite(plBarPin, LOW);        // Enable parallel load
    digitalWrite(clkPin, HIGH);         // Strobe clk to load
    digitalWrite(clkPin, LOW);
    digitalWrite(plBarPin, HIGH);       // Disable parallel load

    #ifdef INSL_DEBUG
    Serial.print(F("InSlide: Raw sensor data "));
    #endif
    // Cycle through the digits from highest to lowest
    for (int8_t dCount = nDigits - 1; dCount >= 0; dCount--) {
        sensorData[dCount] = 0;
        // Read the bits for this digit from bit H to bit A
        for (int8_t bCount = INSL_S_OUT_BITS_PER_DIGIT - 1; bCount >= 0; bCount--) {
            uint8_t bit = digitalRead(sOutPin) == LOW ? 1 : 0;
            sensorData[dCount] |= bit << bCount;
            digitalWrite(clkPin, HIGH);
            digitalWrite(clkPin, LOW);
        }
        // Trim off unused bit(s)
        sensorData[dCount] &= 0xFF >> (INSL_S_OUT_BITS_PER_DIGIT - INSL_SENSORS_PER_DIGIT);
        #ifdef INSL_DEBUG
        Serial.print(sensorData[dCount] < 0x10 ? F("0x0") : F("0x"));
        Serial.print(sensorData[dCount], HEX);
        Serial.print(F(" "));
        #endif
    }
    #ifdef INSL_DEBUG
    Serial.println(F(""));
    #endif
}