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
#ifndef SPI_H
#define SPI_H

typedef struct {
	int bus;
} SpiDevice;

extern int Spi_Open(SpiDevice *device, uint8_t device_id);
extern void Spi_Close(SpiDevice *device);

extern int Spi_Write(SpiDevice *device, void *buffer, uint8_t len);
extern int Spi_Read(SpiDevice *device, void *buffer, uint8_t len); 

extern int Spi_WriteThenRead(SpiDevice *device, void *tx_buffer, uint8_t tx_len, void *rx_buffer, uint8_t rx_len);
extern int Spi_WriteThenWrite(SpiDevice *device, void *tx_buffer1, uint8_t tx_len1, void *tx_buffer2, uint8_t tx_len2);

inline static int Spi_ReadRegister(SpiDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	return Spi_WriteThenRead(device, &reg, sizeof(reg), buffer, len);
}

inline static int Spi_ReadRegisterByte(SpiDevice *device, uint8_t reg, uint8_t *value)
{
	return Spi_WriteThenRead(device, &reg, sizeof(reg), value, sizeof(*value));
}

inline static int Spi_WriteRegister(SpiDevice *device, uint8_t reg, void *buffer, uint8_t len)
{
	return Spi_WriteThenWrite(device, &reg, sizeof(reg), buffer, len);
}

inline static int Spi_WriteRegisterByte(SpiDevice *device, uint8_t reg, uint8_t value)
{
	return Spi_WriteThenWrite(device, &reg, sizeof(reg), &value, sizeof(value));
}

#endif /* SPI_H */
