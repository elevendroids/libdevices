#include <unistd.h>
#include "sensor/bmp085.h"
#include "bus/i2c.h"
#include "delay.h"

#define get_short(c, i) (int16_t) ((c[i] << 8) + c[i + 1])
#define get_ushort(c, i) (uint16_t) ((c[i] << 8) + c[i + 1])

int bmp085_init(int bus, bmp085_device *device)
{
	int status;
	device->bus = bus;
	device->b5 = -1;
	status = i2c_read_reg(bus, BMP085_ADDR, 0xAA, &device->calibration_data, sizeof(device->calibration_data));
	return status;

}

int bmp085_read_temperature(bmp085_device *device)
{
	int status;
	uint8_t data[2];
	uint16_t ut, ac5, ac6;
	int16_t mc, md;
	int32_t x1, x2, b5;

	// start temperature conversion
	status = i2c_write_reg_byte(device->bus, BMP085_ADDR, 0xF4, 0x2E);
	if (status < 0) {
		return status;
	}
	delay_ms(5);
	status = i2c_read_reg(device->bus, BMP085_ADDR, 0xF6, &data, sizeof(data));
	if (status < 0) {
		return status;
	}
	ut = (data[0] << 8) + data[1];

	ac5 = get_ushort(device->calibration_data, 8);
	ac6 = get_ushort(device->calibration_data, 10);
	mc = get_short(device->calibration_data, 18);
	md = get_short(device->calibration_data, 20);
	
	x1 = ((ut - ac6) * ac5) >> 15;
	x2 = (mc << 11) / (x1 + md);
	b5 = x1 + x2;

	device->b5 = b5;

	return (b5 + 8) >> 4;
}

int bmp085_read_pressure(bmp085_device *device)
{
	int status;
	int16_t oss = 3;
	uint8_t data[3];

	int16_t ac1, ac2, ac3, b1, b2;
	uint16_t ac4;
	int32_t b3, b6, b6_2, x1, x2, x3, up, p;
	uint32_t b4, b7;

	if (device->b5 < 0) {
		status = bmp085_read_temperature(device);
		if (status < 0) {
			return -1;
		}
	}
	
	status = i2c_write_reg_byte(device->bus, BMP085_ADDR, 0xF4, 0x34 + (oss << 6));
	if (status < 0) {
		return status;
	}

	delay_ms(50);
	status = i2c_read_reg(device->bus, BMP085_ADDR, 0xF6, &data, sizeof(data));
	if (status < 0) {
		return status;
	}
	
	up = ((data[0] << 16) + (data[1] << 8) + data[0]) >> (8 - oss);

	ac1 = get_short(device->calibration_data, 0);
	ac2 = get_short(device->calibration_data, 2);
	ac3 = get_short(device->calibration_data, 4);
	ac4 = get_ushort(device->calibration_data, 6);
	b1 = get_short(device->calibration_data, 12);
	b2 = get_short(device->calibration_data, 14);

	b6 = device->b5 - 4000;
	b6_2 = b6 * b6 >> 12;
	x1 = (b2 * b6_2) >> 11;
	x2 = ac2 * b6 >> 11; 
	x3 = x1 + x2;
	b3 = (((ac1 * 4 + x3) << oss) + 2) >> 2;

	x1 = ac3 * b6 >> 13;
	x2 = (b1 * b6_2) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
	b7 = ((uint32_t)up - b3) * (50000 >> oss);
	if (b7 < 0x80000000) {
		p = (b7 * 2) / b4;
	} else {
		p = (b7 / b4) * 2;
	}
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	p = p + ((x1 + x2 + 3791) >> 4);

	device->b5 = -1;

	return p;
}

int bmp085_read_chip_id(bmp085_device *device)
{
	int status;
	uint8_t id;
	status = i2c_read_reg_byte(device->bus, BMP085_ADDR, 0xD0, &id);
	if (status < 0) {
		return status;
	} else {
		return id;
	}
}

int bmp085_read_chip_version(bmp085_device *device)
{
	int status;
	uint8_t version;
	status = i2c_read_reg_byte(device->bus, BMP085_ADDR, 0xD1, &version);
	if (status < 0) {
		return status;
	} else {
		return version;
	}
}
