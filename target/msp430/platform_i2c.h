#ifndef _PLATFORM_I2C_H
#define _PLATFORM_I2C_H

#include "usci.h"

typedef struct i2c_tag {
	int device;
	const UsciModule *usci;
} i2c_t;

#endif
