/**
 *
 * @file i2c.h
 * @author Michal Potrzebicz
 * @date 2013-03-13
 *
 * @brief I2C bus API
 * 
 * @section LICENSE
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
#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

#define I2C_REGISTER_NONE	255

/** @brief Valid I2C speed settings (support is platform-specific) */
typedef enum {
	I2C_SPEED_STANDARD,	// SCL = 100kHz
	I2C_SPEED_FAST,		// SCL = 400kHz
	I2C_SPEED_HIGH		// SCL = 1.7MHz
} I2cSpeed;

/** @brief Structure which binds I2C bus instance with a slave device */
typedef struct {
	int bus;
	uint8_t address;
} I2cDevice;

/**
 * @brief Intialize I2C bus
 * @param device - device id of requested bus (platform-specific)
 * @param speed - bus speed
 * @return true on success
 */
bool I2c_Open(uint8_t device, I2cSpeed speed);

/**
 * @brief Close/Finalize I2C bus
 * @param bus - bus handle
 */
void I2c_Close(int bus);

/**
 * @brief Set I2C bus speed
 * @param speed - bus speed
 */
void I2c_SetSpeed(I2cSpeed speed); //TODO: use reference to device/bus

/**
 * @brief Read data from a slave device
 * @param device - pointer to a device structure
 * @param reg - register address or I2C_REGISTER_NONE if not reading from a register
 * @param buffer - RX buffer
 * @param len - RX buffer length
 * @return true on success
 */
bool I2c_Read(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len);

/**
 * @brief Write data to a slave device
 * @param device - pointer to device structure
 * @param reg - register address or I2C_REGISTER_NONE if not writing to a register
 * @param buffer - TX buffer
 * @param len - TX buffer length
 * @return true on success
 */
bool I2c_Write(I2cDevice *device, uint8_t reg, void *buffer, uint8_t len);

/** 
 * @brief Read one byte from a slave device
 * @param device - pointer to a device structure
 * @param reg - register address or I2C_REGISTER_NONE if not reading from a register
 * @param value - destination pointer
 * @return true on success
 */
inline static bool I2c_ReadByte(I2cDevice *device, uint8_t reg, uint8_t *value)
{
	return I2c_Read(device, reg, &value, sizeof(*value));
}

/** 
 * @brief Read one word from a slave device
 * @param device - pointer to a device structure
 * @param reg - register address or I2C_REGISTER_NONE if not reading from a register
 * @param value - destination pointer
 * @return true on success
 */
inline static bool I2c_ReadWord(I2cDevice *device, uint8_t reg, uint16_t *value)
{
	return I2c_Read(device, reg, &value, sizeof(*value));
}

/** 
 * @brief Write one byte to a slave device
 * @param device - pointer to a device structure
 * @param reg - register address or I2C_REGISTER_NONE if not writing to a register
 * @param value - value to be written
 * @return true on success
 */
inline static bool I2c_WriteByte(I2cDevice *device, uint8_t reg, uint8_t value)
{
	return I2c_Write(device, reg, &value, sizeof(value));
}

/** 
 * @brief Write one word to a slave device
 * @param device - pointer to a device structure
 * @param reg - register address or I2C_REGISTER_NONE if not writing to a register
 * @param value - value to be written
 * @return true on success
 */
inline static bool I2c_WriteWord(I2cDevice *device, uint8_t reg, uint16_t value)
{
	return I2c_Write(device, reg, &value, sizeof(value));
}

#endif /* I2C_H */

