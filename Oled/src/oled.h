#ifndef _OLED_H_
#define _OLED_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "i2c.h"

enum oled_module_mode_t
{
	OLED_MODULE_MODE_1,	/* 逐列式 */
	OLED_MODULE_MODE_2, /* 逐行式 */
	OLED_MODULE_MODE_3,	/* 列行式 */
	OLED_MODULE_MODE_4, /* 行列式 */
};

enum oled_module_direct_t
{
	OLED_MODULE_FORWARD,
	OLED_MODULE_REVERSE,
};

struct oled_driver_t
{
	void(*init)(struct oled_driver_t *oled);
	void(*open)(struct oled_driver_t *oled);
	void(*close)(struct oled_driver_t *oled);
	void(*refresh)(struct oled_driver_t *oled);
	void(*clear)(struct oled_driver_t *oled);
	void(*show)(uint32_t x, 
							uint32_t y, 
							uint8_t *buffer, 
							uint8_t buffer_size, 
							uint32_t width, 
							uint32_t high, 
							enum oled_module_mode_t mode, 
							enum oled_module_direct_t direct);
	struct i2c_driver_t *driver;
	uint8_t vitual_ram[128][8];
};

int32_t oled_ssd1306_i2c_init(struct oled_driver_t *oled, struct i2c_driver_t *i2c_driver);

#endif
