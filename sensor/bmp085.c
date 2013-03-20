#include "sensor/bmp085.h"
#include "bus/i2c.h"
#include "delay.h"

#define get_short(c, i) (int16_t) ((c[i] << 8) + c[i + 1])
#define get_ushort(c, i) (uint16_t) ((c[i] << 8) + c[i + 1])

#define CALIBRATION_DATA_LEN 22

typedef uint8_t CalibrationData[CALIBRATION_DATA_LEN];

static const uint8_t wait_time_ms[] = { 5, 8, 14, 26 };

static inline int Bmp085_ReadCalibrationData(I2cDevice *device, CalibrationData *data)
{
	return I2c_ReadRegister(device, 0xAA, data, CALIBRATION_DATA_LEN);
}

static int Bmp085_ReadTemperatureRef(I2cDevice *device, CalibrationData *calibration, int32_t *value)
{
	int status;
	uint8_t data[2];
	uint16_t ut, ac5, ac6;
	int16_t mc, md;
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
	ut = (data[0] << 8) + data[1];

	ac5 = get_ushort((*calibration), 8);
	ac6 = get_ushort((*calibration), 10);
	mc = get_short((*calibration), 18);
	md = get_short((*calibration), 20);
	
	x1 = ((ut - ac6) * ac5) >> 15;
	x2 = (mc << 11) / (x1 + md);

	*value = (x1 + x2);
	return 0;
}

int Bmp085_ReadTemperature(I2cDevice *device)
{
	int status;
	int32_t value;
	CalibrationData calibration;

	status = Bmp085_ReadCalibrationData(device, &calibration);
	if (status < 0) {
		return status;
	}
	
	status = Bmp085_ReadTemperatureRef(device, &calibration, &value);
	if (status >= 0) {
		return (value + 8) >> 4;
	} else {
		return status;
	}
}

int Bmp085_ReadPressure(I2cDevice *device, uint8_t oversampling)
{
	int status;
	uint8_t data[3];
	CalibrationData calibration;
	int16_t ac1, ac2, ac3, b1, b2;
	uint16_t ac4;
	int32_t b3, b5, b6, b6_2, x1, x2, x3, up, p;
	uint32_t b4, b7;

	status = Bmp085_ReadCalibrationData(device, &calibration);
	if (status < 0) {
		return status;
	}
	
	status = Bmp085_ReadTemperatureRef(device, &calibration, &b5);
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

	ac1 = get_short(calibration, 0);
	ac2 = get_short(calibration, 2);
	ac3 = get_short(calibration, 4);
	ac4 = get_ushort(calibration, 6);
	b1 = get_short(calibration, 12);
	b2 = get_short(calibration, 14);

	b6 = b5 - 4000;
	b6_2 = b6 * b6 >> 12;
	x1 = (b2 * b6_2) >> 11;
	x2 = ac2 * b6 >> 11; 
	x3 = x1 + x2;
	b3 = (((ac1 * 4 + x3) << oversampling) + 2) >> 2;

	x1 = ac3 * b6 >> 13;
	x2 = (b1 * b6_2) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
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

