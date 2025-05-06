#include "easy_button.h"

static easy_button_context easy_button_context_tab[EASY_BUTTON_MAX];

void easy_button_init(void)
{   
    uint8_t i = 0;

    if(easy_button_config_tab_num > EASY_BUTTON_MAX)
    {
        /* assert */
    }

    for(i = 0; i < easy_button_config_tab_num; i++)
    {
        easy_button_context_tab[i].time = 0;
				easy_button_context_tab[i].longPress = 0;
    }
}


void easy_button_scanf(void)
{
    uint8_t i = 0;

    for(i = 0; i < easy_button_config_tab_num; i++)
    {
        if(easy_button_config_tab[i].read == NULL)
        {
            continue;
        }

        if(easy_button_config_tab[i].active == easy_button_config_tab[i].read()) 
        {
            easy_button_context_tab[i].time += EASY_BUTTON_SCANF_CYCLE;

						if(easy_button_context_tab[i].longPress)
						{
								continue;
						}
            if(easy_button_context_tab[i].time > easy_button_config_tab[i].longPress)
            {
                easy_button_notify(easy_button_config_tab[i].button, EASY_BUTTON_LONG_PRESS);
								easy_button_context_tab[i].longPress = 1;
            }
        }
        else /* 释放 */
        {
            if(easy_button_context_tab[i].time == 0)
            {

            }
            else if(easy_button_context_tab[i].time <= easy_button_config_tab[i].debounce)
            {
                /* 未满足消抖时间 */
            }
            else if(easy_button_context_tab[i].time <= easy_button_config_tab[i].longPress)
            {
                /* 未满足长按时间(短按) */
                easy_button_notify(easy_button_config_tab[i].button, EASY_BUTTON_SHORT);
            }
            else
            {
                /* 满足长按时间 */
                easy_button_notify(easy_button_config_tab[i].button, EASY_BUTTON_LONG_RELEASE);
            }

            easy_button_context_tab[i].time = 0;
						easy_button_context_tab[i].longPress = 0;
        }
    }
}

