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
#include "byteswap.h"
#include "common.h"
#include "delay.h"
#include "sensor/bmp085.h"
#include "bus/i2c.h"

typedef struct {
	int16_t ac1, ac2, ac3;
	uint16_t ac4, ac5, ac6;
	int16_t b1, b2;
	int16_t mb, mc, md;
} CalibrationData;

static CalibrationData cal_data;

static const uint8_t wait_time_ms[] = { 5, 8, 14, 26 };

int Bmp085_Init( I2cDevice *device)
{
	int status = I2c_Read(device->bus, device->address, BMP085_REG_AC1, &cal_data, sizeof(cal_data));
	if (status != STATUS_OK)
		return status;

	// byte-swap all values
	int16_t *value_ptr = (int16_t *)&cal_data;
	for (int i = 0; i < (sizeof(cal_data) / sizeof(int)); i++) {
		*value_ptr = bswap_16(*value_ptr);
		value_ptr++;
	}

	return STATUS_OK;
}

static int Bmp085_ReadTemperatureRef(I2cDevice *device, int32_t *value)
{
	int status;
	uint8_t data[2];
	uint16_t ut;
	int32_t x1, x2;
	
	// start temperature conversion
	status = I2c_WriteByte(device->bus, device->address, 0xF4, 0x2E);
	if (status != STATUS_OK) 
		return status;

	Delay_Ms(5);
	status = I2c_Read(device->bus, device->address, 0xF6, &data, sizeof(data));
	if (status != STATUS_OK)
		return status;

	ut = ((uint16_t)data[0] << 8) + data[1];

	x1 = (((uint32_t)ut - (uint32_t)cal_data.ac6) * (uint32_t)cal_data.ac5) >> 15;
	x2 = ((int32_t)cal_data.mc << 11) / (x1 + cal_data.md);

	*value = (x1 + x2);
	return STATUS_OK;
}

int Bmp085_ReadTemperature(I2cDevice *device, int16_t *value)
{
	int status;
	int32_t reference;

	status = Bmp085_ReadTemperatureRef(device, &reference);
	if (status != STATUS_OK)
		return status;

	*value = (reference + 8) >> 4;
	return STATUS_OK;
}

int Bmp085_ReadPressure(I2cDevice *device, uint8_t oversampling, int32_t *value)
{
	int status;
	uint8_t data[3];
	int32_t b3, b5, b6, b6_2, x1, x2, x3, up, p;
	uint32_t b4, b7;

	status = Bmp085_ReadTemperatureRef(device, &b5);
	if (status != STATUS_OK)
		return status;

	status = I2c_WriteByte(device->bus, device->address, 0xF4, 0x34 + (oversampling << 6));
	if (status != STATUS_OK) 
		return status;

	Delay_Ms(wait_time_ms[oversampling]);
	status = I2c_Read(device->bus, device->address, 0xF6, &data, sizeof(data));
	if (status != STATUS_OK)
		return status;
	
	up = (((int32_t)data[0] << 16) + ((int32_t)data[1] << 8) + data[0]) >> (8 - oversampling);

	b6 = b5 - 4000;
	b6_2 = (b6 * b6) >> 12;
	x1 = ((int32_t)cal_data.b2 * b6_2) >> 11;
	x2 = ((int32_t)cal_data.ac2 * b6) >> 11; 
	x3 = x1 + x2;
	b3 = ((((int32_t)cal_data.ac1 * 4 + x3) << oversampling) + 2) >> 2;

	x1 = ((int32_t)cal_data.ac3 * b6) >> 13;
	x2 = ((int32_t)cal_data.b1 * b6_2) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = ((uint32_t)cal_data.ac4 * (uint32_t)(x3 + 32768)) >> 15;
	b7 = ((uint32_t)up - b3) * (50000 >> oversampling);

	if (b7 < 0x80000000) {
		p = (b7 * 2) / b4;
	} else {
		p = (b7 / b4) * 2;
	}

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	*value = p + ((x1 + x2 + 3791) >> 4);

	return STATUS_OK;
}

int Bmp085_ReadChipId(I2cDevice *device)
{
	int status;
	uint8_t id;
	status = I2c_ReadByte(device->bus, device->address, 0xD0, &id);
	if (status != STATUS_OK) {
		return status;
	} else {
		return id;
	}
}

int Bmp085_ReadChipVersion(I2cDevice *device)
{
	int status;
	uint8_t version;
	status = I2c_ReadByte(device->bus, device->address, 0xD1, &version);
	if (status != STATUS_OK) {
		return status;
	} else {
		return version;
	}
}

