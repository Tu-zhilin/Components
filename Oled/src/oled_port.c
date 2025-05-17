#include "cmsis_os.h"

/* time: ms */
void oled_delay(uint32_t time)
{
	osDelay(time);
}
