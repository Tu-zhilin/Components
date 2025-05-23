#include <string.h>
#include "oled.h"

#define OLED_SLAVE_ADDRESS  0x78
#define OLED_SLAVE_COMMAND  0x00
#define OLED_SLAVE_DATA     0x40

void oled_delay(uint32_t time);

static void oled_send_byte(struct oled_driver_t *oled, uint8_t data, uint8_t type)
{
    if(oled == NULL || oled->driver == NULL)
    {
        return;
    }

    oled->driver->start(oled->driver);
    oled->driver->sendByte(oled->driver, OLED_SLAVE_ADDRESS);
    oled->driver->sendByte(oled->driver, type);
    oled->driver->sendByte(oled->driver, data);
    oled->driver->stop(oled->driver);
}

/// @brief 刷新屏幕
static void oled_display_refresh(struct oled_driver_t *oled)
{
		uint8_t x,y;
		for(y = 0; y < 8; y++)
		{
				oled_send_byte(oled, 0xb0 | y, OLED_SLAVE_COMMAND);
				oled_send_byte(oled, 0x00, 		 OLED_SLAVE_COMMAND);
				oled_send_byte(oled, 0x10, 		 OLED_SLAVE_COMMAND);

				for(x = 0; x < 128; x++)
				{
						oled_send_byte(oled, oled->vitual_ram[x][y],  OLED_SLAVE_DATA);
				}
		}
}

/// @brief 清屏 
static void oled_display_clear(struct oled_driver_t *oled)
{
    memset(oled->vitual_ram, 0x00, 128 * 8);
		oled_display_refresh(oled);
}

/// @brief 打开显示 
static void oled_display_open(struct oled_driver_t *oled)
{
		oled_send_byte(oled, 0x8D, OLED_SLAVE_COMMAND); 
		oled_send_byte(oled, 0x14, OLED_SLAVE_COMMAND);  
	  oled_send_byte(oled, 0xAF, OLED_SLAVE_COMMAND);
}

/// @brief 关闭显示
static void oled_display_close(struct oled_driver_t *oled)
{
		oled_send_byte(oled, 0xAE, OLED_SLAVE_COMMAND);
		oled_send_byte(oled, 0x8D, OLED_SLAVE_COMMAND); 
		oled_send_byte(oled, 0x10, OLED_SLAVE_COMMAND);  
}

static void oled_fill_point(struct oled_driver_t *oled, uint32_t x, uint32_t y, uint8_t data)
{
		uint8_t x_byte, y_byte, y_bit;
	
		x_byte = x;
		y_byte = y / 8;
		y_bit = y % 8;
	
		if(data)
		{
				oled->vitual_ram[x_byte][y_byte] |= (1 << y_bit);
		}
		else
		{
				oled->vitual_ram[x_byte][y_byte] &= ~(1 << y_bit);
		}
}


static void oled_show_column_row(struct oled_driver_t *oled,
																 uint32_t x, 
																 uint32_t y, 
																 uint8_t *buffer, 
																 uint8_t buffer_size, 
																 uint32_t width, 
																 uint32_t high,  
																 enum oled_module_direct_t direct)
{
		uint8_t _x, _y, _cnt, i;
	  uint8_t *buffer_ptr = buffer;
	
		for(_cnt = 0; _cnt < (high / 8); _cnt++)
		{
			for(_x = x; _x < (x + width); _x++)
			{
				  _y = y + _cnt * 8;
				
					for(i = 0; i < 8; i++,_y++)
					{
							if(direct == OLED_MODULE_MSB)
							{
									oled_fill_point(oled, _x, _y, ((*buffer_ptr >> (7 - i)) & 0x01));
							}
							else
							{
									oled_fill_point(oled, _x, _y, ((*buffer_ptr >> i) & 0x01));
							}
					}
					buffer_ptr++;
			}
		}
}

/*
* oled_show
* x: 横坐标起点
* y: 纵坐标七点
* buffer: 数据
* buffer_size: 数据个数
* width: 横向长度（像素点）
* heigh: 纵向长度（像素点）
* mode: 取模模式
* direct: 取模方向
*/
static void oled_show(struct oled_driver_t *oled,
											uint32_t x, 
											uint32_t y, 
											uint8_t *buffer, 
											uint32_t buffer_size, 
											uint32_t width, 
											uint32_t high, 
											enum oled_module_mode_t mode, 
											enum oled_module_direct_t direct)
{
		if((x + width > 128) || 
			 (y + high > 64) ||
		   (high % 8 != 0) ||
		   (width * (high / 8) != buffer_size))
		{
				return;
		}

		switch(mode)
		{
				case OLED_MODULE_MODE_1: //TODO:
					break;
				case OLED_MODULE_MODE_2: //TODO: 
					break;
				case OLED_MODULE_MODE_3: /* 行列式 */
					oled_show_column_row(oled, x, y, buffer, buffer_size, width, high, direct);
					break;
				case OLED_MODULE_MODE_4: //TODO:
					break;
				default:
					return;
		}
}

static void oled_init(struct oled_driver_t *oled)
{
		oled_send_byte(oled, 0xAE, OLED_SLAVE_COMMAND); //--turn off oled panel
		oled_send_byte(oled, 0x00, OLED_SLAVE_COMMAND); //---set low column address
		oled_send_byte(oled, 0x10, OLED_SLAVE_COMMAND); //---set high column address
		oled_send_byte(oled, 0x40, OLED_SLAVE_COMMAND); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
		oled_send_byte(oled, 0x81, OLED_SLAVE_COMMAND); //--set contrast control register
		oled_send_byte(oled, 0xCF, OLED_SLAVE_COMMAND); // Set SEG Output Current Brightness
		oled_send_byte(oled, 0xA1, OLED_SLAVE_COMMAND); //--Set SEG/Column Mapping     
		oled_send_byte(oled, 0xC8, OLED_SLAVE_COMMAND); //Set COM/Row Scan Direction  
		oled_send_byte(oled, 0xA6, OLED_SLAVE_COMMAND); //--set normal display
		oled_send_byte(oled, 0xA8, OLED_SLAVE_COMMAND); //--set multiplex ratio(1 to 64)
		oled_send_byte(oled, 0x3f, OLED_SLAVE_COMMAND); //--1/64 duty
		oled_send_byte(oled, 0xD3, OLED_SLAVE_COMMAND); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
		oled_send_byte(oled, 0x00, OLED_SLAVE_COMMAND); //-not offset
		oled_send_byte(oled, 0xd5, OLED_SLAVE_COMMAND); //--set display clock divide ratio/oscillator frequency
		oled_send_byte(oled, 0x80, OLED_SLAVE_COMMAND); //--set divide ratio, Set Clock as 100 Frames/Sec
		oled_send_byte(oled, 0xD9, OLED_SLAVE_COMMAND); //--set pre-charge period
		oled_send_byte(oled, 0xF1, OLED_SLAVE_COMMAND); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
		oled_send_byte(oled, 0xDA, OLED_SLAVE_COMMAND); //--set com pins hardware configuration
		oled_send_byte(oled, 0x12, OLED_SLAVE_COMMAND);
		oled_send_byte(oled, 0xDB, OLED_SLAVE_COMMAND); //--set vcomh
		oled_send_byte(oled, 0x40, OLED_SLAVE_COMMAND); //Set VCOM Deselect Level
		oled_send_byte(oled, 0x20, OLED_SLAVE_COMMAND); //-Set Page Addressing Mode (0x00/0x01/0x02)
		oled_send_byte(oled, 0x02, OLED_SLAVE_COMMAND); 
		oled_send_byte(oled, 0x8D, OLED_SLAVE_COMMAND); //--set Charge Pump enable/disable
		oled_send_byte(oled, 0x10, OLED_SLAVE_COMMAND); //--set(0x10) disable  
		oled_send_byte(oled, 0xA4, OLED_SLAVE_COMMAND); // Disable Entire Display On (0xa4/0xa5)
		oled_send_byte(oled, 0xA6, OLED_SLAVE_COMMAND); // Disable Inverse Display On (0xa6/a7) 
		oled_send_byte(oled, 0xAF, OLED_SLAVE_COMMAND);
}

int32_t oled_ssd1306_i2c_init(struct oled_driver_t *oled, struct i2c_driver_t *i2c_driver)
{
    if(oled == NULL ||i2c_driver == NULL)
    {
        return -1;
    }

		oled->init    = oled_init;
		oled->open    = oled_display_open;
		oled->close   = oled_display_close;
		oled->clear   = oled_display_clear;
		oled->refresh = oled_display_refresh;
		oled->show    = oled_show;
		
		oled->driver = i2c_driver;
		
		memset(oled->vitual_ram, 0x00, 128 * 8);

    return 0;
}
