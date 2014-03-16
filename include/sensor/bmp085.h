/*
*
* Copyright (c) 2013, Michal Potrzebicz <michal@elevendroids.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
* conditions are met:
*
*    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*      disclaimer.
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided with the distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#ifndef BMP085_H
#define BMP085_H

#include <stdint.h>
#include <stdbool.h>
#include "bus/i2c.h"

#define BMP085_ADDRESS	0x77

#define BMP085_REG_AC1	0xAA
#define BMP085_REG_AC2	0xAC
#define BMP085_REG_AC3	0xAE
#define BMP085_REG_AC4	0xB0
#define BMP085_REG_AC5	0xB2
#define BMP085_REG_AC6	0xB4
#define BMP085_REG_B1	0xB6
#define BMP085_REG_B2	0xB8
#define BMP085_REG_MB	0xBA
#define BMP085_REG_MC	0xBC
#define BMP085_REG_MD	0xBE

#define BMP085_SAMPLES_1 0
#define BMP085_SAMPLES_2 1
#define BMP085_SAMPLES_4 2
#define BMP085_SAMPLES_8 3

bool Bmp085_Init(i2c_t *bus);

int Bmp085_ReadPressure(i2c_t *bus, uint8_t oversampling, int32_t *value);

int Bmp085_ReadTemperature(i2c_t *bus, int16_t *value);

int Bmp085_ReadChipId(i2c_t *bus);

int Bmp085_ReadChipVersion(i2c_t *bus);

#endif /* BMP085_H */

