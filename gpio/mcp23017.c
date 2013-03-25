#include "bus/i2c.h"
#include "gpio/mcp23x17.h"


int Mcp23017_WriteRegister(I2cDevice *device, uint8_t reg, uint8_t value)
{
	return I2c_WriteRegister(device, reg, &value, sizeof(value));
}

int Mcp23017_ReadRegister(I2cDevice *device, uint8_t reg, uint8_t *value)
{
	return I2c_ReadRegister(device, reg, value, sizeof(value));
}

int Mcp23017_Init(I2cDevice *device)
{
	return 0;
}

int Mcp23017_ReadPortA(I2cDevice *device, uint8_t *value)
{
	return I2c_ReadRegisterByte(device, MCP23X17_SEQ_GPIOA, value);
}

int Mcp23017_ReadPortB(I2cDevice *device, uint8_t *value)
{
	return I2c_ReadRegisterByte(device, MCP23X17_SEQ_GPIOB, value);
}

int Mcp23017_ReadPorts(I2cDevice *device, Mcp23x17PortsData *data)
{
	return I2c_ReadRegister(device, MCP23X17_SEQ_GPIOA, data, sizeof(data->as_word));
}

int Mcp23017_WritePortA(I2cDevice *device, uint8_t value)
{
	return I2c_WriteRegister(device, MCP23X17_SEQ_GPIOA, &value, sizeof(value));
}

int Mcp23017_WritePortB(I2cDevice *device, uint8_t value)
{
	return I2c_WriteRegister(device, MCP23X17_SEQ_GPIOB, &value, sizeof(value));
}

int Mcp23017_WritePorts(I2cDevice *device, Mcp23x17PortsData data)
{
	return I2c_WriteRegister(device, MCP23X17_SEQ_GPIOA, &data, sizeof(data));
}

int Mcp23017_SetPinA(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortA(device, &port_value);
	if (status < 0 ) return status;
	port_value |= pin;
	return Mcp23017_WritePortA(device, port_value);
}

int Mcp23017_SetPinB(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortB(device, &port_value);
	if (status < 0 ) return status;
	port_value |= pin;
	return Mcp23017_WritePortB(device, port_value);
}

int Mcp23017_ResetPinA(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortA(device, &port_value);
	if (status < 0) return status;
	port_value &= ~pin;
	return Mcp23017_WritePortA(device, port_value);
}

int Mcp23017_ResetPinB(I2cDevice *device, uint8_t pin)
{
	uint8_t port_value;
	int status;
	status = Mcp23017_ReadPortB(device, &port_value);
	if (status < 0) return status;
	port_value &= ~pin;
	return Mcp23017_WritePortB(device, port_value);
}

