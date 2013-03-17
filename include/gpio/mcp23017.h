#ifndef MCP23017_H
#define MCP23017_H

#include "bus/i2c.h"

int Mcp23017_Init(I2cDevice *device);
int Mcp23017_WriteRegister(I2cDevice *device, uint8_t reg, uint8_t value);
int Mcp23017_ReadRegister(I2cDevice *device, uint8_t reg, uint8_t *value);

int Mcp23017_WritePortA(I2cDevice *device, uint8_t value); 
int Mcp23017_WritePortB(I2cDevice *device, uint8_t value);

inline int Mcp23017_SetPinA(I2cDevice *device, uint8_t pin);
inline int Mcp23017_SetPinB(I2cDevice *device, uint8_t pin);

inline int Mcp23017_ResetPinA(I2cDevice *device, uint8_t pin);
inline int Mcp23017_ResetPinB(I2cDevice *device, uint8_t pin);

#endif
