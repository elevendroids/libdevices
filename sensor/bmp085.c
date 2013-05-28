#include "sensor/bmp085.h"
#include "bus/i2c.h"
#include "delay.h"
#include "byteswap.h"

#define get_short(c, i) (int16_t) ((c[i] << 8) + c[i + 1])
#define get_ushort(c, i) (uint16_t) ((c[i] << 8) + c[i + 1])

typedef struct {
	int16_t ac1, ac2, ac3;
	int16_t b1, b2;
	int16_t mb, mc, md;
	uint16_t ac4, ac5, ac6;
} CalibrationData1;

static CalibrationData1 cal_data;

static const uint8_t wait_time_ms[] = { 5, 8, 14, 26 };

static int16_t Bmp085_Read16(I2cDevice *device, uint8_t address)
{
	uint16_t result;
	I2c_ReadRegister(device, address, &result, sizeof(result));
	return bswap_16(result);
}

int Bmp085_Init( I2cDevice *device)
{
	cal_data.ac1 = Bmp085_Read16(device, BMP085_REG_AC1);
	cal_data.ac2 = Bmp085_Read16(device, BMP085_REG_AC2);
	cal_data.ac3 = Bmp085_Read16(device, BMP085_REG_AC3);
	cal_data.b1 = Bmp085_Read16(device, BMP085_REG_B1);
	cal_data.b2 = Bmp085_Read16(device, BMP085_REG_B2);
	cal_data.mb = Bmp085_Read16(device, BMP085_REG_MB);
	cal_data.mc = Bmp085_Read16(device, BMP085_REG_MC);
	cal_data.md = Bmp085_Read16(device, BMP085_REG_MD);
	cal_data.ac4 = Bmp085_Read16(device, BMP085_REG_AC4);
	cal_data.ac5 = Bmp085_Read16(device, BMP085_REG_AC5);
	cal_data.ac6 = Bmp085_Read16(device, BMP085_REG_AC6);
	return 0;
}

static int Bmp085_ReadTemperatureRef(I2cDevice *device, int32_t *value)
{
	int status;
	uint8_t data[2];
	uint16_t ut;
	int32_t x1, x2;
	
	// start temperature conversion
	status = I2c_WriteRegisterByte(device, 0xF4, 0x2E);
	if (status < 0) {
		return status;
	}
	delay_ms(5);
	status = I2c_ReadRegister(device, 0xF6, &data, sizeof(data));
	if (status < 0) {
		return status;
	}
	ut = ((uint16_t)data[0] << 8) + data[1];

	
	x1 = (((uint32_t)ut - (uint32_t)cal_data.ac6) * (uint32_t)cal_data.ac5) >> 15;
	x2 = ((int32_t)cal_data.mc << 11) / (x1 + cal_data.md);

	*value = (x1 + x2);
	return 0;
}

int Bmp085_ReadTemperature(I2cDevice *device)
{
	int status;
	int32_t value;

	status = Bmp085_ReadTemperatureRef(device, &value);
	if (status >= 0) {
		value = (value + 8) >> 4;
		return value;
	} else {
		return status;
	}
}

int32_t Bmp085_ReadPressure(I2cDevice *device, uint8_t oversampling)
{
	int status;
	uint8_t data[3];
	int32_t b3, b5, b6, b6_2, x1, x2, x3, up, p;
	uint32_t b4, b7;

	status = Bmp085_ReadTemperatureRef(device, &b5);
	if (status < 0) {
		return status;
	}

	status = I2c_WriteRegisterByte(device, 0xF4, 0x34 + (oversampling << 6));
	if (status < 0) {
		return status;
	}

	delay_ms(wait_time_ms[oversampling]);
	status = I2c_ReadRegister(device, 0xF6, &data, sizeof(data));
	if (status < 0) {
		return status;
	}
	
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
	p = p + ((x1 + x2 + 3791) >> 4);

	return p;
}

int Bmp085_ReadChipId(I2cDevice *device)
{
	int status;
	uint8_t id;
	status = I2c_ReadRegisterByte(device, 0xD0, &id);
	if (status < 0) {
		return status;
	} else {
		return id;
	}
}

int Bmp085_ReadChipVersion(I2cDevice *device)
{
	int status;
	uint8_t version;
	status = I2c_ReadRegisterByte(device, 0xD1, &version);
	if (status < 0) {
		return status;
	} else {
		return version;
	}
}

