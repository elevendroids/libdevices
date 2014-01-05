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
#ifndef RFM22_H
#define RFM22_H

#include <stdint.h>
#include <bus/spi.h>

#define RFM22_WRITE_MASK					0x80

#define RFM22_REG_00_DEVICE_TYPE				0x00
#define RFM22_REG_01_VERSION_CODE				0x01
#define RFM22_REG_02_DEVICE_STATUS				0x02
#define RFM22_REG_03_INTERRUPT_STATUS1				0x03
#define RFM22_REG_04_INTERRUPT_STATUS2					0x04
#define RFM22_REG_05_INTERRUPT_ENABLE1				0x05
#define RFM22_REG_06_INTERRUPT_ENABLE2				0x06
#define RFM22_REG_07_OPERATING_MODE1				0x07
#define RFM22_REG_08_OPERATING_MODE2				0x08
#define RFM22_REG_09_OSCILLATOR_LOAD_CAPACITANCE		0x09
#define RFM22_REG_0A_UC_OUTPUT_CLOCK				0x0a
#define RFM22_REG_0B_GPIO_CONFIGURATION0			0x0b
#define RFM22_REG_0C_GPIO_CONFIGURATION1			0x0c
#define RFM22_REG_0D_GPIO_CONFIGURATION2			0x0d
#define RFM22_REG_0E_IO_PORT_CONFIGURATION			0x0e
#define RFM22_REG_0F_ADC_CONFIGURATION				0x0f

#define RFM22_REG_10_ADC_SENSOR_AMP_OFFSET			0x10
#define RFM22_REG_11_ADC_VALUE					0x11
#define RFM22_REG_12_TEMPERATURE_SENSOR_CALIBRATION		0x12
#define RFM22_REG_13_TEMPERATURE_VALUE_OFFSET			0x13
#define RFM22_REG_14_WAKEUP_TIMER_PERIOD1			0x14
#define RFM22_REG_15_WAKEUP_TIMER_PERIOD2			0x15
#define RFM22_REG_16_WAKEUP_TIMER_PERIOD3			0x16
#define RFM22_REG_17_WAKEUP_TIMER_VALUE1			0x17
#define RFM22_REG_18_WAKEUP_TIMER_VALUE2			0x18
#define RFM22_REG_19_LDC_MODE_DURATION				0x19
#define RFM22_REG_1A_LOW_BATTERY_DETECTOR_THRESHOLD		0x1a
#define RFM22_REG_1B_BATTERY_VOLTAGE_LEVEL			0x1b
#define RFM22_REG_1C_IF_FILTER_BANDWIDTH			0x1c
#define RFM22_REG_1D_AFC_LOOP_GEARSHIFT_OVERRIDE		0x1d
#define RFM22_REG_1E_AFC_TIMING_CONTROL			0x1e
#define RFM22_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE		0x1f

#define RFM22_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE		0x20
#define RFM22_REG_21_CLOCK_RECOVERY_OFFSET2			0x21
#define RFM22_REG_22_CLOCK_RECOVERY_OFFSET1			0x22
#define RFM22_REG_23_CLOCK_RECOVERY_OFFSET0			0x23
#define RFM22_REG_24_CLOCK_RECOVERY_TIMING_LOOP_GAIN1		0x24
#define RFM22_REG_25_CLOCK_RECOVERY_TIMING_LOOP_GAIN0		0x25
#define RFM22_REG_26_RSSI					0x26
#define RFM22_REG_27_RSSI_THRESHOLD				0x27
#define RFM22_REG_28_ANTENNA_DIVERSITY1			0x28
#define RFM22_REG_29_ANTENNA_DIVERSITY2			0x29
#define RFM22_REG_2A_AFC_LIMITER				0x2a
#define RFM22_REG_2B_AFC_CORRECTION_READ			0x2b
#define RFM22_REG_2C_OOK_COUNTER_VALUE_1			0x2c
#define RFM22_REG_2D_OOK_COUNTER_VALUE_2			0x2d
#define RFM22_REG_2E_SLICER_PEAK_HOLD				0x2e

#define RFM22_REG_30_DATA_ACCESS_CONTROL			0x30
#define RFM22_REG_31_EZMAC_STATUS				0x31
#define RFM22_REG_32_HEADER_CONTROL1				0x32
#define RFM22_REG_33_HEADER_CONTROL2				0x33
#define RFM22_REG_34_PREAMBLE_LENGTH				0x34
#define RFM22_REG_35_PREAMBLE_DETECTION_CONTROL		0x35
#define RFM22_REG_36_SYNC_WORD3				0x36
#define RFM22_REG_37_SYNC_WORD2				0x37
#define RFM22_REG_38_SYNC_WORD1				0x38
#define RFM22_REG_39_SYNC_WORD0				0x39
#define RFM22_REG_3A_TRANSMIT_HEADER3				0x3a
#define RFM22_REG_3B_TRANSMIT_HEADER2				0x3b
#define RFM22_REG_3C_TRANSMIT_HEADER1				0x3c
#define RFM22_REG_3D_TRANSMIT_HEADER0				0x3d
#define RFM22_REG_3E_PACKET_LENGTH				0x3e
#define RFM22_REG_3F_CHECK_HEADER3				0x3f

#define RFM22_REG_40_CHECK_HEADER2				0x40
#define RFM22_REG_41_CHECK_HEADER1				0x41
#define RFM22_REG_42_CHECK_HEADER0				0x42
#define RFM22_REG_43_HEADER_ENABLE3				0x43
#define RFM22_REG_44_HEADER_ENABLE2				0x44
#define RFM22_REG_45_HEADER_ENABLE1				0x45
#define RFM22_REG_46_HEADER_ENABLE0				0x46
#define RFM22_REG_47_RECEIVED_HEADER3				0x47
#define RFM22_REG_48_RECEIVED_HEADER2				0x48
#define RFM22_REG_49_RECEIVED_HEADER1				0x49
#define RFM22_REG_4A_RECEIVED_HEADER0				0x4a
#define RFM22_REG_4B_RECEIVED_PACKET_LENGTH			0x4b

// Hmmm.... These are marked as <reserved> in the register reference...
#define RFM22_REG_50_ANALOG_TEST_BUS_SELECT			0x50
#define RFM22_REG_51_DIGITAL_TEST_BUS_SELECT			0x51
#define RFM22_REG_52_TX_RAMP_CONTROL				0x52
#define RFM22_REG_53_PLL_TUNE_TIME				0x53
#define RFM22_REG_55_CALIBRATION_CONTROL			0x55
#define RFM22_REG_56_MODEM_TEST				0x56
#define RFM22_REG_57_CHARGE_PUMP_TEST				0x57
#define RFM22_REG_58_CHARGE_PUMP_CURRENT_TRIMMING		0x58
#define RFM22_REG_59_DIVIDER_CURRENT_TRIMMING			0x59
#define RFM22_REG_5A_VCO_CURRENT_TRIMMING			0x5a
#define RFM22_REG_5B_VCO_CALIBRATION				0x5b
#define RFM22_REG_5C_SYNTHESIZER_TEST				0x5c
#define RFM22_REG_5D_BLOCK_ENABLE_OVERRIDE1			0x5d
#define RFM22_REG_5E_BLOCK_ENABLE_OVERRIDE2			0x5e
#define RFM22_REG_5F_BLOCK_ENABLE_OVERRIDE3			0x5f
//

#define RFM22_REG_60_CHANNEL_FILTER_COEFFICIENT_ADDRESS	0x60
#define RFM22_REG_61_CHANNEL_FILTER_COEFFICIENT_VALUE		0x61
#define RFM22_REG_62_CRYSTAL_OSCILLATOR_POR_CONTROL		0x62
#define RFM22_REG_63_RC_OSCILLATOR_COARSE_CALIBRATION		0x63
#define RFM22_REG_64_RC_OSCILLATOR_FINE_CALIBRATION		0x64
#define RFM22_REG_65_LDO_CONTROL_OVERRIDE			0x65
#define RFM22_REG_66_LDO_LEVEL_SETTINGS			0x66
#define RFM22_REG_67_DELTA_SIGMA_ADC_TUNING1			0x67
#define RFM22_REG_68_DELTA_SIGMA_ADC_TUNING2			0x68
#define RFM22_REG_69_AGC_OVERRIDE1				0x69
#define RFM22_REG_6A_AGC_OVERRIDE2				0x6a
#define RFM22_REG_6B_GFSK_FIR_FILTER_COEFFICIENT_ADDRESS	0x6b
#define RFM22_REG_6C_GFSK_FIR_FILTER_COEFFICIENT_VALUE		0x6c
#define RFM22_REG_6D_TX_POWER					0x6d
#define RFM22_REG_6E_TX_DATA_RATE1				0x6e
#define RFM22_REG_6F_TX_DATA_RATE0				0x6f

#define RFM22_REG_70_MODULATION_CONTROL1			0x70
#define RFM22_REG_71_MODULATION_CONTROL2			0x71
#define RFM22_REG_72_FREQUENCY_DEVIATION			0x72
#define RFM22_REG_73_FREQUENCY_OFFSET1				0x73
#define RFM22_REG_74_FREQUENCY_OFFSET2				0x74
#define RFM22_REG_75_FREQUENCY_BAND_SELECT			0x75
#define RFM22_REG_76_NOMINAL_CARRIER_FREQUENCY1		0x76
#define RFM22_REG_77_NOMINAL_CARRIER_FREQUENCY0		0x77
#define RFM22_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT		0x79
#define RFM22_REG_7A_FREQUENCY_HOPPING_STEP_SIZE		0x7a
#define RFM22_REG_7C_TX_FIFO_CONTROL1				0x7c
#define RFM22_REG_7D_TX_FIFO_CONTROL2				0x7d
#define RFM22_REG_7E_RX_FIFO_CONTROL				0x7e
#define RFM22_REG_7F_FIFO_ACCESS				0x7f


// Flags
// RFM22_REG_02_DEVICE_STATUS
#define RFM22_FFOVFL		0x80
#define RFM22_FFUNFL		0x40
#define RFM22_RXFFEM		0x20
#define RFM22_HEADERR		0x10
#define RFM22_CPS_TX		0x02
#define RFM22_CPS_RX		0x01
#define RFM22_CPS_IDLE		0x00

// RFM22_REG_03_INTERRUPT_STATUS1
#define RFM22_IFFERR		0x80
#define RFM22_ITXFFAFULL		0x40
#define RFM22_ITXFFAEM		0x20
#define RFM22_IRXFFAFULL		0x10
#define RFM22_IEXT		0x08
#define RFM22_IPKSENT		0x04
#define RFM22_IPKVALID		0x02
#define RFM22_ICRCERROR		0x01

// RFM22_REG_04_INTERRUPT_STATUS2
#define RFM22_ISWDET		0x80
#define RFM22_IPREAVAL		0x40
#define RFM22_IPREAINVAL		0x20
#define RFM22_IRSSI		0x10
#define RFM22_IWUT		0x08
#define RFM22_ILBD		0x04
#define RFM22_ICHIPRDY		0x02
#define RFM22_IPOR		0x01

// RFM22_REG_05_INTERRUPT_ENABLE1
#define RFM22_ENFFERR		0x80
#define RFM22_ENTXFFAFULL	0x40
#define RFM22_ENTXFFAEM		0x20
#define RFM22_ENRXFFAFULL	0x10
#define RFM22_ENEXT		0x08
#define RFM22_ENPKSENT		0x04
#define RFM22_ENPKVALID		0x02
#define RFM22_ENCRCERROR		0x01

// RFM22_REG_06_INTERRUPT_ENABLE2
#define RFM22_ENSWDET		0x80
#define RFM22_ENPREAVAL		0x40
#define RFM22_ENPREAINVAL	0x20
#define RFM22_ENRSSI		0x10
#define RFM22_ENWUT		0x08
#define RFM22_ENLBDINT		0x04
#define RFM22_ENCHIPRDY		0x02
#define RFM22_ENPOR		0x01

// RFM22_REG_07_OPERATING_MODE1
#define RFM22_SWRES		0x80
#define RFM22_ENLBD		0x40
#define RFM22_ENWT		0x20
#define RFM22_X32KSEL		0x10
#define RFM22_TXON		0x08
#define RFM22_RXON		0x04
#define RFM22_PLLON		0x02
#define RFM22_XTON		0x01

// RFM22_REG_08_OPERATING_MODE2
#define RFM22_ANTDIV		0xE0
#define RFM22_RXMPK		0x10
#define RFM22_AUTOTX		0x08
#define RFM22_ENLDM		0x04
#define RFM22_FFCLRRX		0x02
#define RFM22_FFCLRTX		0x01

// RFM22_REG_09_OSCILLATOR_LOAD_CAPACITANCE
#define RFM22_XTALSHIFT		0x80
#define RFM22_XLC		0x7F

// RFM22_REG_0A_UC_OUTPUT_CLOCK
#define RFM22_CLKT		0x30
#define RFM22_ENLFC		0x08
#define RFM22_MCLK		0x07

// RFM22_REG_6D_TX_POWER
// RFM22B values
#define RFM22_TX_POWER_1DBM	0x00
#define RFM22_TX_POWER_2DBM	0x01
#define RFM22_TX_POWER_5DBM	0x02
#define RFM22_TX_POWER_8DBM	0x03
#define RFM22_TX_POWER_11DBM	0x04
#define RFM22_TX_POWER_14DBM	0x05
#define RFM22_TX_POWER_17DBM	0x06
#define RFM22_TX_POWER_20DBM	0x07

// RFM23B values
#define RFM23_TX_POWER__8DBM	0x00
#define RFM23_TX_POWER__5DBM	0x01
#define RFM23_TX_POWER__2DBM	0x02
#define RFM23_TX_POWER_1DBM	0x03
#define RFM23_TX_POWER_4DBM	0x04
#define RFM23_TX_POWER_7DBM	0x05
#define RFM23_TX_POWER_10DBM	0x06
#define RFM23_TX_POWER_13DBM	0x07

typedef struct {
	uint8_t regs[3];
} RFM22CarrierFreqConfig;

// preconfigured carrier frequencies
typedef enum {
	RFM22_Frequency434MHz = 0,
	RFM22_Frequency868MHz,
	RFM22_Frequency915MHz
} RFM22CarrierFreqEnum;

typedef struct {
	uint8_t reg_1c; // RFM22_REG_1C_IF_FILTER_BANDWIDTH
	uint8_t reg_20; // RFM22_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE
	uint8_t reg_21; // RFM22_REG_21_CLOCK_RECOVERY_OFFSET2
	uint8_t reg_22; // RFM22_REG_22_CLOCK_RECOVERY_OFFSET1
	uint8_t reg_23; // RFM22_REG_23_CLOCK_RECOVERY_OFFSET0
	uint8_t reg_24; // RFM22_REG_24_CLOCK_RECOVERY_TIMING_LOOP_GAIN1
	uint8_t reg_25; // RFM22_REG_25_CLOCK_RECOVERY_TIMING_LOOP_GAIN0
	uint8_t reg_2a; // RFM22_REG_2A_AFC_LIMITER
	uint8_t reg_2c; // RFM22_REG_2C_OOK_COUNTER_VALUE_1
	uint8_t reg_2d; // RFM22_REG_2D_OOK_COUNTER_VALUE_2
	uint8_t reg_2e; // RFM22_REG_2E_SLICER_PEAK_HOLD
	uint8_t reg_6e; // RFM22_REG_6E_TX_DATA_RATE1
	uint8_t reg_6f; // RFM22_REG_6F_TX_DATA_RATE0
	uint8_t reg_70; // RFM22_REG_70_MODULATION_CONTROL1
	uint8_t reg_71; // RFM22_REG_71_MODULATION_CONTROL2
	uint8_t reg_72; // RFM22_REG_72_FREQUENCY_DEVIATION
} RFM22ModemConfig;

// preconfigured modem configs
typedef enum {
	RFM22_UnmodulatedCarrier = 0,
	RFM22_GFSK2400bps36kHz
} RFM22ModemConfigEnum;


typedef enum {
	RFM22_ModeIdle = 0,
	RFM22_ModeStandby,
	RFM22_ModeSleep,
	RFM22_ModeTune,
	RFM22_ModeTx,
	RFM22_ModeRx
} RFM22ModeEnum;

typedef struct {
	SpiDevice *spi;

	RFM22ModeEnum _mode;
} RFM22Device;

extern void RFM22_Init(RFM22Device *device);
extern void RFM22_Reset(RFM22Device *device);
extern void RFM22_SetCarrierFreq(RFM22Device *device, RFM22CarrierFreqEnum frequency);
extern void RFM22_SetMode(RFM22Device *device, RFM22ModeEnum mode);
extern void RFM22_SetModemConfig(RFM22Device *device, RFM22ModemConfigEnum config);
extern void RFM22_SetTxPower(RFM22Device *device, uint8_t power);

extern uint8_t RFM22_ReadStatus(RFM22Device *device);
extern uint8_t RFM22_ReadRssi(RFM22Device *device);

extern void RFM22_ResetTxFifo(RFM22Device *device);
extern void RFM22_ResetRxFifo(RFM22Device *device);

extern void RFM22_HandleInterrupts(void);

#endif /* RFM22_H */
