#include "i2c.h"
#include "stm32f4xx_hal.h"

#define I2C_SIMULATION_DELAY_TIME		1		/* 延时确保波形的变化能被从机采到，发送波特率需要慢于从机接收波特率 
																					 注意:在不同的代码优化等级下时间是不同的，要根据实际测试来调整
																					 优化等级是0的时候可以设置为1*/

void delay(uint32_t t)
{
	while(t--); 
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
static void simulate_i2c_start(struct i2c_driver_t *driver)
{
		driver->config->set_sda(1);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_scl(1);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_sda(0);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_scl(0);
		delay(I2C_SIMULATION_DELAY_TIME);
}
 
 /**
   * @brief  I2C停止
   * @param  无
   * @retval 无
   */
static void simulate_i2c_stop(struct i2c_driver_t *driver)
{
		driver->config->set_scl(1);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_sda(0);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_sda(1);
		delay(I2C_SIMULATION_DELAY_TIME);
}

static void simulate_i2c_waitAck(struct i2c_driver_t *driver)
{
	driver->config->set_scl(1);
	delay(I2C_SIMULATION_DELAY_TIME);	 
	driver->config->set_scl(0);
	delay(I2C_SIMULATION_DELAY_TIME);
}
 
 /**
   * @brief  I2C发送一个字节
   * @param  Byte 要发送的一个字节
   * @retval 无
   */
static void simulate_i2c_sendByte(struct i2c_driver_t *driver, uint8_t Byte)
{
	uint8_t i;
	
	for (i = 0; i < 8; i++)
	{
		driver->config->set_scl(0);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_sda((Byte >> (7 - i)) & 0x01);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_scl(1);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_scl(0);
		delay(I2C_SIMULATION_DELAY_TIME);
	}
	simulate_i2c_waitAck(driver);
}

static void simulate_i2c_init(struct i2c_driver_t *driver)
{
		driver->config->set_scl(1);
		delay(I2C_SIMULATION_DELAY_TIME);
		driver->config->set_sda(1);
		delay(I2C_SIMULATION_DELAY_TIME);
}

void i2c_simulate_drivers_init(struct i2c_driver_t *driver, struct i2c_driver_config_t *config)
{
		driver->init = simulate_i2c_init;
		driver->start = simulate_i2c_start;
		driver->stop = simulate_i2c_stop;
		driver->sendByte = simulate_i2c_sendByte;
	  driver->config = config;
}

