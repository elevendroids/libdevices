#include "bus/i2c.h"
#include "gpio/mcp23x17.h"


inline int Mcp23017_WriteRegister(I2cDevice *device, uint8_t reg, uint8_t value)
{
	return I2c_WriteRegister(device, reg, &value, sizeof(value));
}

inline int Mcp23017_ReadRegister(I2cDevice *device, uint8_t reg, uint8_t *value)
{
	return I2c_ReadRegister(device, reg, value, sizeof(value));
}

int Mcp23017_Init(I2cDevice *device)
{
	return 0;
}

inline int Mcp23017_ReadPortA(I2cDevice *device, uint8_t *value)
{
	return I2c_ReadRegisterByte(device, MCP23X17_SEQ_GPIOA, value);
}

inline int Mcp23017_ReadPortB(I2cDevice *device, uint8_t *value)
{
	return I2c_ReadRegisterByte(device, MCP23X17_SEQ_GPIOB, value);
}

inline int Mcp23017_WritePortA(I2cDevice *device, uint8_t value)
{
	return I2c_WriteRegisterByte(device, MCP23X17_SEQ_OLATA, value);
}

inline int Mcp23017_WritePortB(I2cDevice *device, uint8_t value)
{
	return I2c_WriteRegisterByte(device, MCP23X17_SEQ_OLATB, value);
}

inline int Mcp23017_SetPinA(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortA(device, &port_value);
	if (status < 0 ) return status;
	port_value |= pin;
	return Mcp23017_WritePortA(device, port_value);
}

inline int Mcp23017_SetPinB(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortB(device, &port_value);
	if (status < 0 ) return status;
	port_value |= pin;
	return Mcp23017_WritePortB(device, port_value);
}

inline int Mcp23017_ResetPinA(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortA(device, &port_value);
	if (status < 0) return status;
	port_value &= ~pin;
	return Mcp23017_WritePortA(device, port_value);
}

inline int Mcp23017_ResetPinB(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortB(device, &port_value);
	if (status < 0) return status;
	port_value &= ~pin;
	return Mcp23017_WritePortB(device, port_value);
}

