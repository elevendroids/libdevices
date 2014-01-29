/*
 *
 * @file timer.h
 * @author Michal Potrzebicz
 * @date
 *
 * @brief Simple tick timer
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
#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

/**
 * @brief Initialize timer
 */
void Timer_Init(void);

/**
 * @brief Get current tick (ms) count
 * @returns Number of ticks
 */
unsigned int Timer_GetTicks(void);

/**
 * @brief Calculate difference between tick values
 * @param start - initial ticks value
 * @param end - final ticks value
 * @returns Number of elapsed ticks between start and end
 */
unsigned int Timer_ElapsedTicks(unsigned int start, unsigned int end);

/**
 * @brief Check if a timeout occured between current and saved tick value
 * @param ticks - last ticks value
 * @param timeout - timeout value
 * @returns true if timeout has occured
 */
bool Timer_IsTimeOut(unsigned int ticks, unsigned int timeout);

/**
 * @brief Sleeps (board specific, using PM if available) for a given number of miliseconds
 * @param ticks - number of ticks (ms) to sleep
 */
void Timer_Sleep(unsigned int ticks);

#endif
