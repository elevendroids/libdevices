#ifndef I2C_COMMON_H
#define I2C_COMMON_H

#include <stdint.h>

extern int i2c_open(uint8_t device);
extern void i2c_close(int bus);

extern int i2c_read(int bus, uint8_t addr, void *buffer, uint8_t len);
extern int i2c_write(int bus, uint8_t addr, void *buffer, uint8_t len);
extern int i2c_write_then_read(int bus, uint8_t addr, void *tx_buf, uint8_t tx_len, void *rx_buf, uint8_t rx_len);

inline static int i2c_read_reg(int bus, uint8_t addr, uint8_t reg, void *buffer, uint8_t len)
{
	return i2c_write_then_read(bus, addr, &reg, sizeof(reg), buffer, len);
}

extern int i2c_write_reg(int bus, uint8_t addr, uint8_t reg, void *buffer, uint8_t len);

inline static int i2c_read_byte(int bus, uint8_t addr, uint8_t *value)
{
	return i2c_read(bus, addr, value, sizeof(*value));
}

inline static int i2c_read_word(int bus, uint8_t addr, uint16_t *value)
{
	return i2c_read(bus, addr, value, sizeof(*value));
}

inline static int i2c_write_byte(int bus, uint8_t addr, uint8_t value)
{
	return i2c_write(bus, addr, &value, sizeof(value));
}

inline static int i2c_write_word(int bus, uint8_t addr, uint16_t value)
{
	return i2c_write(bus, addr, &value, sizeof(value));
}

inline static int i2c_read_reg_byte(int bus, uint8_t addr, uint8_t reg, uint8_t *value)
{
	return i2c_read_reg(bus, addr, reg, value, sizeof(*value));
}

inline static int i2c_read_reg_word(int bus, uint8_t addr, uint8_t reg, uint16_t *value)
{
	return i2c_read_reg(bus, addr, reg, value, sizeof(*value));
}

inline static int i2c_write_reg_byte(int bus, uint8_t addr, uint8_t reg, uint8_t value)
{
	return i2c_write_reg(bus, addr, reg, &value, sizeof(value));
}

inline static int i2c_write_reg_word(int bus, uint8_t addr, uint8_t reg, uint16_t value)
{
	return i2c_write_reg(bus, addr, reg, &value, sizeof(value));
}

#endif
