/**
 *
 * @file port.h
 * @author Michal Potrzebicz
 * @date 2014-02-22
 *
 * @brief MSP430 digital port common header
 *
 * @section LICENSE
 *
 * Copyright (c) 2014, Michal Potrzebicz <michal@elevendroids.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the followin
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _MSP430_PORT_H
#define _MSP430_PORT_H

#include <stdint.h>
#include "digital_io.h"

/**
 * @brief Container for port registers
 */
typedef struct {
    volatile uint8_t *out;      ///< Output register
    volatile const uint8_t *in; ///< Input register
    volatile uint8_t *dir;      ///< Direction register
    volatile uint8_t *ren;      ///< Pullup/pulldown resistor enable register
    volatile uint8_t *ie;       ///< Interrupt enable register
    volatile uint8_t *ies;      ///< Interrupt mode register
    volatile uint8_t *ifg;      ///< Interrupt flag register
    volatile uint8_t *sel;      ///< Primary peripherial module function select register
    volatile uint8_t *sel2;     ///< Secondary peripherial module function select register
} Msp430Port;

/**
 * @param port - IO port index
 * @returns Pointer to port structure
 */
const Msp430Port* Msp430_GetPort(const unsigned int port);

/**
 * @param pin - Pin index (zero based)
 * @param mask - pin bit mask to use with port
 * @returns Pointer to port structure
 */
const Msp430Port* Msp430_GetPinPort(const unsigned int pin, uint8_t *mask);

#endif /* _MSP430_PORT_H */
