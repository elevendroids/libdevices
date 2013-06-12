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
#ifndef I2C_COMMON_H
#define I2C_COMMON_H

#include <stdint.h>

#define I2C_SPEED_STANDARD	0x00	// SCL = 100kHz
#define I2C_SPEED_FAST		0x01	// SCL = 400kHz
#define I2C_SPEED_HIGH		0x02	// SCL = 1.7MHz

typedef struct {
	int bus;
	uint8_t address;
} I2cDevice;

extern int I2c_Open(uint8_t device, uint8_t speed);
extern void I2c_Close(int bus);

extern int I2c_Read(I2cDevice *device, void *buffer, uint8_t len);
extern int I2c_Write(I2cDevice *device, void *buffer, uint8_t len);
extern int I2c_WriteThenRead(I2cDevice *device, void *tx_buf, uint8_t tx_len, void *rx_buf, uint8_t rx_len);

inline static int I2c_ReadRegister(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	return I2c_WriteThenRead(device, &reg, sizeof(reg), buffer, len);
}

extern int I2c_WriteRegister(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len);

inline static int I2c_ReadByte(I2cDevice *device, uint8_t *value)
{
	return I2c_Read(device, value, sizeof(*value));
}

inline static int I2c_WriteByte(I2cDevice *device, uint8_t value)
{
	return I2c_Write(device, &value, sizeof(value));
}

inline static int I2c_ReadRegisterByte(I2cDevice *device, uint8_t reg, uint8_t *value)
{
	return I2c_ReadRegister(device, reg, value, sizeof(*value));
}

inline static int I2c_WriteRegisterByte(I2cDevice *device, uint8_t reg, uint8_t value)
{
	return I2c_WriteRegister(device, reg, &value, sizeof(value));
}

#endif
