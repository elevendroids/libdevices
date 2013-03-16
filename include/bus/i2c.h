#ifndef I2C_COMMON_H
#define I2C_COMMON_H

#include <stdint.h>

typedef struct {
	int bus;
	uint8_t address;
} I2cDevice;

extern int I2c_Open(uint8_t device);
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
