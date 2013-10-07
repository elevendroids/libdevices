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
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <bus/spi.h>

static const char* SPI_DEVICE_MAP[] = {
	"/dev/spidev0.0", // 0x00
	"/dev/spidev0.1", // 0x01
	"/dev/spidev1.0", // 0x02
	"/dev/spidev1.1"  // 0x03
}; 

int Spi_Open(SpiDevice *device, uint8_t device_id)
{
	const char *device_file = SPI_DEVICE_MAP[device_id];
	device->bus = open(device_file, O_RDWR);
	return device->bus;
}

void Spi_Close(SpiDevice *device)
{
	close(device->bus);	
}

int Spi_Write(SpiDevice *device, void *buffer, uint8_t len)
{
	return write(device->bus, buffer, len);
}

int Spi_Read(SpiDevice *device, void *buffer, uint8_t len)
{
	return read(device->bus, buffer, len);
}

int Spi_WriteThenRead(SpiDevice *device, void *tx_buf, uint8_t tx_len, void *rx_buf, uint8_t rx_len)
{
	struct spi_ioc_transfer xfers[2] = {
		{
			.tx_buf = (unsigned long) tx_buf,
			.len = tx_len,
			.cs_change = 0
		},
		{
			.rx_buf = (unsigned long) rx_buf,
			.len = rx_len
		}
	};

	return ioctl(device->bus, SPI_IOC_MESSAGE(2), xfers);
}

int Spi_WriteThenWrite(SpiDevice *device, void *tx_buf1, uint8_t tx_len1, void *tx_buf2, uint8_t tx_len2)
{
	struct spi_ioc_transfer xfers[2] = {
		{
			.tx_buf = (unsigned long) tx_buf1,
			.len = tx_len1,
			.cs_change = 0
		},
		{
			.tx_buf = (unsigned long) tx_buf2,
			.len = tx_len2
		}
	};
	
	return ioctl(device->bus, SPI_IOC_MESSAGE(2), xfers);
}


