/****
 * @file     SimpleRead.cpp
 * 
 * This file is an example for the InSLide library.
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

#define BANNER            F("Seven-segment input device test program V1.0.0.")

// GPIO pins to which the various InSlide signals are attached
#define READ_SW_PIN       (8)
#define CLK_PIN           (4)
#define PL_BAR_PIN        (7)
#define S_IN_PIN          (6)
#define S_OUT_PIN         (5)

#define PAUSE_MS          (500)

InSlide device = InSlide(CLK_PIN, PL_BAR_PIN, S_IN_PIN, S_OUT_PIN);

void setup() {
    Serial.begin(9600);
    Serial.println(BANNER);

    if (device.begin() <= 0) {
      Serial.print(F("InSlide device initialization failed with RC = "));
      Serial.println(device.digitCount());
      while (true) {
          // Spin; can't continue.
      }
    }
    Serial.print(F("InSlide device with "));
    Serial.print(device.digitCount());
    Serial.println(F(" digits found."));

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
    device.update();
    for (uint8_t dNo = 0; dNo < device.digitCount(); dNo++) {
        uint8_t sensorState = device.sensorState(dNo);
        if (sensorState >= 0) {
            Serial.print(F("Module "));
            Serial.print(dNo);
            Serial.print(F(" sensor state: 0b"));
            Serial.print(sensorState, BIN);
            int8_t digitValue = device.digitValue(dNo);
            if (digitValue >= 0) {
                Serial.print(F(" (digit: "));
                Serial.print(digitValue, HEX);
                Serial.println(F(")."));
            } else {
                Serial.println(F(" (not a valid digit)."));
            }
        } else {
            Serial.println("Error reading sensor state.");
        }
    }
    delay(PAUSE_MS);
}