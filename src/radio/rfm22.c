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
#include <delay.h>
#include <radio/rf22.h>

static const RFM22CarrierFreqConfig RFM22_CARRIER_FREQ_CONFIG[] = {
	{{0x53, 0x64, 0x00}},	// 434MHz
	{{0x73, 0x64, 0x00}},	// 868MHz
	{{0x75, 0xBB, 0x80}}	// 915MHz
};


static const RFM22ModemConfig RFM22_MODEM_CONFIG[] = {
//        1C    20    21    22    23    24    25    2A    2C    2D    2E    6E    6F    70    71    72
	{0x1B, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x1E, 0x2A, 0x08, 0x2A, 0x13, 0xA9, 0x2C, 0x00, 0x3A},  // UnmodulatedCarrier
	{0x1B, 0x41, 0x60, 0x27, 0x52, 0x00, 0x07, 0x1E, 0x2A, 0x08, 0x2A, 0x13, 0xA9, 0x2C, 0x23, 0x3A}  // GFSK_2400bps36kHz 
};

void RFM22_Init(RFM22Device *device)
{
	// wait for device to settle
	Delay_Ms(16);

	RFM22_SetMode(device, RFM22_ModeIdle);	

	// Configure GPIO for antenna switching (RFM22B only)
	// We're assuming that GPIO0 is connected to RX_ANT and GPIO1 is connected to TX_ANT
	Spi_WriteRegister(device->spi, RFM22_REG_0B_GPIO_CONFIGURATION0 | RFM22_WRITE_MASK, 0x15);
	Spi_WriteRegister(device->spi, RFM22_REG_0C_GPIO_CONFIGURATION1 | RFM22_WRITE_MASK, 0x12);

	// set default config	
	RFM22_SetCarrierFreq(device, RFM22_Frequency868MHz);
	RFM22_SetModemConfig(device, RFM22_GFSK2400bps36kHz);
	RFM22_SetTxPower(device, RFM22_TX_POWER_1DBM);
}

void RFM22_SetMode(RFM22Device *device, RFM22ModeEnum mode)
{
	uint8_t reg_value = 0;
	if (device->_mode != mode) {
		switch (mode) {
			case RFM22_ModeIdle	: reg_value = RFM22_XTON; break;
			case RFM22_ModeStandby	: reg_value = 0x00; break;
			case RFM22_ModeSleep	: reg_value = RFM22_ENWT; break;
			case RFM22_ModeTune	: reg_value = RFM22_PLLON; break;
			case RFM22_ModeTx	: reg_value = RFM22_TXON; break;
			case RFM22_ModeRx	: reg_value = RFM22_RXON; break;
			default			: return;
		}
		Spi_WriteRegister(device->spi, RFM22_REG_07_OPERATING_MODE1 | RFM22_WRITE_MASK, reg_value);
		device->_mode = mode;
	}
}

uint8_t RFM22_ReadStatus(RFM22Device *device)
{
	return Spi_ReadRegister(device->spi, RFM22_REG_02_DEVICE_STATUS);
}

uint8_t RFM22_ReadRssi(RFM22Device *device)
{
	return Spi_ReadRegister(device->spi, RFM22_REG_26_RSSI);
}

void RFM22_Reset(RFM22Device *device)
{
	Spi_WriteRegister(device->spi, RFM22_REG_07_OPERATING_MODE1 | RFM22_WRITE_MASK, RFM22_SWRES);
	Delay_Ms(1);
}

void RFM22_SetCarrierFreq(RFM22Device *device, RFM22CarrierFreqEnum frequency)
{
	const RFM22CarrierFreqConfig *data = &RFM22_CARRIER_FREQ_CONFIG[frequency];
	Spi_BurstWriteRegister(device->spi, RFM22_REG_75_FREQUENCY_BAND_SELECT | RFM22_WRITE_MASK, 
		(void *) data, sizeof(RFM22CarrierFreqConfig));
}

void RFM22_SetModemConfig(RFM22Device *device, RFM22ModemConfigEnum config)
{
	const RFM22ModemConfig *data = &RFM22_MODEM_CONFIG[config];
	SpiDevice *spi = device->spi;

	Spi_WriteRegister(spi, RFM22_REG_1C_IF_FILTER_BANDWIDTH | RFM22_WRITE_MASK, data->reg_1c);
	Spi_BurstWriteRegister(spi, RFM22_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE | RFM22_WRITE_MASK, (void *) &data->reg_20, 6);
	Spi_WriteRegister(spi, RFM22_REG_2A_AFC_LIMITER | RFM22_WRITE_MASK, data->reg_2a);
	Spi_BurstWriteRegister(spi, RFM22_REG_2C_OOK_COUNTER_VALUE_1 | RFM22_WRITE_MASK, (void *) &data->reg_2c, 3);
	Spi_BurstWriteRegister(spi, RFM22_REG_6E_TX_DATA_RATE1 | RFM22_WRITE_MASK, (void *) &data->reg_6e, 5);
}

void RFM22_SetTxPower(RFM22Device *device, uint8_t power)
{
	Spi_WriteRegister(device->spi, RFM22_REG_6D_TX_POWER | RFM22_WRITE_MASK, power);
}

void RFM22_ResetTxFifo(RFM22Device *device)
{
	Spi_WriteRegister(device->spi, RFM22_REG_08_OPERATING_MODE2 | RFM22_WRITE_MASK, RFM22_FFCLRTX);
}

void RFM22_ResetRxFifo(RFM22Device *device)
{
	Spi_WriteRegister(device->spi, RFM22_REG_08_OPERATING_MODE2 | RFM22_WRITE_MASK, RFM22_FFCLRRX);
}

void RFM22_HandleInterrupts(RFM22Device *device)
{
	uint8_t flags[2];

	// Read (and clear) interrupt flags	
	Spi_BurstReadRegister(device->spi, RFM22_REG22_INTERRUPT_STATUS1, flags, sizeof(flags));
	
	//TODO: Process interrupt flags
		
}
