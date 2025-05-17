#ifndef _OLED_H_
#define _OLED_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "i2c.h"

struct oled_driver_t
{
	int32_t(*init)(const struct i2c_driver_t *driver);
	void(*clear)(void);
};

extern const struct oled_driver_t oled_driver;

#endif
