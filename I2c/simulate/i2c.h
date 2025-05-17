#ifndef _I2C_H_
#define _I2C_H_

#include <stdio.h>
#include <stdint.h>

struct i2c_driver_config_t
{
	  void(*set_scl)(uint8_t level);
		void(*set_sda)(uint8_t level);
};

struct i2c_driver_t
{
		void(*init)(struct i2c_driver_t *driver);
    void(*start)(struct i2c_driver_t *driver);
    void(*stop)(struct i2c_driver_t *driver);
    void(*sendByte)(struct i2c_driver_t *driver, uint8_t data);
		struct i2c_driver_config_t *config;
};

void i2c_simulate_drivers_init(struct i2c_driver_t *driver, struct i2c_driver_config_t *config);

#endif

