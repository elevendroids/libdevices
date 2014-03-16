#ifndef _I2C_OBJECT_H
#define I2C_OBJECT_H

#include "usci.h"

typedef struct i2c_tag {
	int device;
	const UsciModule *usci;
} i2c_t;

#endif
