/**
 *
 * @file digital_io.h
 * @author Michal Potrzebicz
 * @date 2014-01-05
 * 
 * @brief Simple digital IO API
 *
 * @section LICENSE
 *
 * Copyright (c) 2014, Michal Potrzebicz <michal@elevendroids.com>
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

#ifndef _DIGITAL_IO_H
#define _DIGITAL_IO_H

/** @brief Pin low state */
#define PIN_STATE_LOW		0x00
/** @brief Pin high state */
#define PIN_STATE_HIGH		0x01
/** @brief Selects input pin mode */
#define PIN_MODE_INPUT		0x01
/** @brief Selects output pin mode */
#define PIN_MODE_OUTPUT		0x02
/** @brief Enables input pin's pullup resistor */
#define PIN_MODE_PULLUP		0x10
/** @brief Enables input pin's pulldown resistor */
#define PIN_MODE_PULLDOWN	0x20

/**
 * @brief Set an output pin to 1
 * @param pin an output pin id
 */
void Pin_Set(int pin);

/**
 * @brief Gets an input pin state
 * @param pin an input pin id
 * @returns #PIN_STATE_LOW or #PIN_STATE_HIGH
 */
int Pin_Get(int pin);

/**
 * @brief Reset an output pin to 0
 * @param pin a pin id
 */
void Pin_Reset(int pin);

/**
 * @brief Toggle an output pin
 * @param pin an output pin id
 */
void Pin_Toggle(int pin);

/**
 * @brief Set a pin mode - direction, pullup/pulldown resistors (if applicable)
 * @param pin a pin id
 * @param mode a pin mode
 *
 * Mode can be set to
 * * #PIN_MODE_OUTPUT
 * * #PIN_MODE_INPUT
 *
 * For input pins one of pull resistor modes can be enabled: #PIN_MODE_PULLDOWN or #PIN_MODE_PULLUP. 
 * Enabling pullup/pulldown has no effect if hardware doesn't support it.
 * 
 */
void Pin_SetMode(int pin, int mode);


#endif /* _DIGITAL_IO_H */

