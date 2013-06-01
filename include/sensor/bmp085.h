#ifndef BMP085_H
#define BMP085_H

#include <stdint.h>
#include "bus/i2c.h"

#define BMP085_ADDR 0x77

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

int Bmp085_Init(I2cDevice *device);
int32_t Bmp085_ReadPressure(I2cDevice *device, uint8_t oversampling);
int Bmp085_ReadTemperature(I2cDevice *device);
int Bmp085_ReadChipId(I2cDevice *device);
int Bmp085_ReadChipVersion(I2cDevice *device);

#endif

