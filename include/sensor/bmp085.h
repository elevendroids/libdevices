#ifndef BMP085_H
#define BMP085_H

#include <stdint.h>
#include "bus/i2c.h"

#define BMP085_ADDR 0x77

#define BMP085_SAMPLES_1 0
#define BMP085_SAMPLES_2 1
#define BMP085_SAMPLES_4 2
#define BMP085_SAMPLES_8 3

extern int Bmp085_Init(I2cDevice *device);
extern int Bmp085_ReadPressure(I2cDevice *device, uint8_t oversampling);
extern int Bmp085_ReadTemperature(I2cDevice *device);
int Bmp085_ReadChipId(I2cDevice *device);
int Bmp085_ReadChipVersion(I2cDevice *device);

#endif
