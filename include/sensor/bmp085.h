#ifndef BMP085_H
#define BMP085_H

#include <stdint.h>

#define BMP085_ADDR 0x77

typedef struct {
	int bus;
	uint8_t calibration_data[22];
	int32_t b5;
} bmp085_device;

extern int bmp085_init(int bus, bmp085_device *device);

extern int bmp085_read_pressure(bmp085_device *device);
extern int bmp085_read_temperature(bmp085_device *device);
int bmp085_read_chip_id(bmp085_device *device);
int bmp085_read_chip_version(bmp085_device *device);

#endif
