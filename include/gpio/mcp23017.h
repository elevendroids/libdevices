/*
*
* Copyright (c) 2013, Michal Potrzebicz <michal@elevendroids.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
* conditions are met:
*
*    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*      disclaimer.
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*      disclaimer in the documentation and/or other materials provided with the distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#ifndef MCP23017_H
#define MCP23017_H

#include "gpio/mcp23x17.h"
#include "bus/i2c.h"

int Mcp23017_Init(I2cDevice *device);
int Mcp23017_WriteRegister(I2cDevice *device, uint8_t reg, uint8_t value);
int Mcp23017_ReadRegister(I2cDevice *device, uint8_t reg, uint8_t *value);

int Mcp23017_ReadPortA(I2cDevice *device, uint8_t *value);
int Mcp23017_ReadPortB(I2cDevice *device, uint8_t *value);
int Mcp23017_ReadPorts(I2cDevice *device, Mcp23x17PortsData *data);

int Mcp23017_WritePortA(I2cDevice *device, uint8_t value); 
int Mcp23017_WritePortB(I2cDevice *device, uint8_t value);
int Mcp23017_WritePorts(I2cDevice *device, Mcp23x17PortsData data);

inline int Mcp23017_SetPinA(I2cDevice *device, uint8_t pin);
inline int Mcp23017_SetPinB(I2cDevice *device, uint8_t pin);

inline int Mcp23017_ResetPinA(I2cDevice *device, uint8_t pin);
inline int Mcp23017_ResetPinB(I2cDevice *device, uint8_t pin);

#endif
