#include "oled.h"

#define OLED_SLAVE_ADDRESS  0x78
#define OLED_SLAVE_COMMAND  0x00
#define OLED_SLAVE_DATA     0x40

static struct i2c_driver_t* bus_driver = NULL;

static uint8_t virtual_display_ram[128][8] = {0};

/// @brief 清屏 
static void oled_display_clear()
{
    memset(virtual_display_ram, 0x00, sizeof(virtual_display_ram));
}

static void oled_display_refresh()
{
	
}

static void oled_send_byte(uint8_t data, uint8_t type)
{
    if(bus_driver == NULL)
    {
        return;
    }

    bus_driver->start(bus_driver);
    bus_driver->sendByte(bus_driver, OLED_SLAVE_ADDRESS);
    bus_driver->sendByte(bus_driver, type);
    bus_driver->sendByte(bus_driver, data);
    bus_driver->stop(bus_driver);
}

static int32_t oled_init(const struct i2c_driver_t *i2c_driver)
{
    if(i2c_driver == NULL)
    {
        return -1;
    }

    bus_driver = (struct i2c_driver_t *)i2c_driver;

    oled_send_byte(0xAE, OLED_SLAVE_COMMAND); //--turn off oled panel
		oled_send_byte(0x00, OLED_SLAVE_COMMAND); //---set low column address
		oled_send_byte(0x10, OLED_SLAVE_COMMAND); //---set high column address
		oled_send_byte(0x40, OLED_SLAVE_COMMAND); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
		oled_send_byte(0x81, OLED_SLAVE_COMMAND); //--set contrast control register
		oled_send_byte(0xCF, OLED_SLAVE_COMMAND); // Set SEG Output Current Brightness
		oled_send_byte(0xA1, OLED_SLAVE_COMMAND); //--Set SEG/Column Mapping     
		oled_send_byte(0xC8, OLED_SLAVE_COMMAND); //Set COM/Row Scan Direction  
		oled_send_byte(0xA6, OLED_SLAVE_COMMAND); //--set normal display
		oled_send_byte(0xA8, OLED_SLAVE_COMMAND); //--set multiplex ratio(1 to 64)
		oled_send_byte(0x3f, OLED_SLAVE_COMMAND); //--1/64 duty
		oled_send_byte(0xD3, OLED_SLAVE_COMMAND); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
		oled_send_byte(0x00, OLED_SLAVE_COMMAND); //-not offset
		oled_send_byte(0xd5, OLED_SLAVE_COMMAND); //--set display clock divide ratio/oscillator frequency
		oled_send_byte(0x80, OLED_SLAVE_COMMAND); //--set divide ratio, Set Clock as 100 Frames/Sec
		oled_send_byte(0xD9, OLED_SLAVE_COMMAND); //--set pre-charge period
		oled_send_byte(0xF1, OLED_SLAVE_COMMAND); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
		oled_send_byte(0xDA, OLED_SLAVE_COMMAND); //--set com pins hardware configuration
		oled_send_byte(0x12, OLED_SLAVE_COMMAND);
		oled_send_byte(0xDB, OLED_SLAVE_COMMAND); //--set vcomh
		oled_send_byte(0x40, OLED_SLAVE_COMMAND); //Set VCOM Deselect Level
		oled_send_byte(0x20, OLED_SLAVE_COMMAND); //-Set Page Addressing Mode (0x00/0x01/0x02)
		oled_send_byte(0x02, OLED_SLAVE_COMMAND); 
		oled_send_byte(0x8D, OLED_SLAVE_COMMAND); //--set Charge Pump enable/disable
		oled_send_byte(0x14, OLED_SLAVE_COMMAND); //--set(0x10) disable  
		oled_send_byte(0xA4, OLED_SLAVE_COMMAND); // Disable Entire Display On (0xa4/0xa5)
		oled_send_byte(0xA6, OLED_SLAVE_COMMAND); // Disable Inverse Display On (0xa6/a7) 
		oled_send_byte(0xAF, OLED_SLAVE_COMMAND);

    return 0;
}

const struct oled_driver_t oled_driver = 
{
  .init  = oled_init,
	.clear = oled_display_clear
};
