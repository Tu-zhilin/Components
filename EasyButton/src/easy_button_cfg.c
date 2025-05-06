#include "easy_button.h"
#include "stm32f4xx_hal.h"

void easy_button_notify(easy_button button, easy_button_event event)
{

}

/* PA0 */
easy_button_state easy_button_read_up(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET ? EASY_BUTTON_LOW : EASY_BUTTON_HIGH;
}

/* PE3 */
easy_button_state easy_button_read_down(void)
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET ? EASY_BUTTON_LOW : EASY_BUTTON_HIGH;
}

/* PE4 */
easy_button_state easy_button_read_left(void)
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET ? EASY_BUTTON_LOW : EASY_BUTTON_HIGH;
}

/* PE2 */
easy_button_state easy_button_read_right(void)
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == GPIO_PIN_RESET ? EASY_BUTTON_LOW : EASY_BUTTON_HIGH;
}

const easy_button_config easy_button_config_tab[] =
{
    {EASY_BUTTON_UP, EASY_BUTTON_HIGH, 20, 3000, easy_button_read_up},
    {EASY_BUTTON_DOWN, EASY_BUTTON_LOW, 20, 3000, easy_button_read_down},
    {EASY_BUTTON_LEFT, EASY_BUTTON_LOW, 20, 3000, easy_button_read_left},
    {EASY_BUTTON_RIGHT, EASY_BUTTON_LOW, 20, 3000, easy_button_read_right},
};

const uint8_t easy_button_config_tab_num = sizeof(easy_button_config_tab) / sizeof(easy_button_config_tab[0]);

