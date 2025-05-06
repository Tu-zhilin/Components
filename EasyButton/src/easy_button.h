#ifndef _EASY_BUTTON_H_
#define _EASY_BUTTON_H_

#include <stdio.h>
#include <stdint.h>

#define EASY_BUTTON_SCANF_CYCLE  10  /* 轮询周期 */

/* 用户编辑 */
typedef enum easy_button_t
{
    EASY_BUTTON_UP,
    EASY_BUTTON_DOWN,
    EASY_BUTTON_LEFT,
    EASY_BUTTON_RIGHT,
    EASY_BUTTON_MAX
}easy_button;

typedef enum easy_button_state_t
{
    EASY_BUTTON_LOW,
    EASY_BUTTON_HIGH
}easy_button_state;

typedef enum easy_button_event_t
{
    EASY_BUTTON_SHORT,
    EASY_BUTTON_LONG_PRESS,
    EASY_BUTTON_LONG_RELEASE
}easy_button_event;

typedef struct easy_button_config_t
{
    easy_button button;
    easy_button_state active;             /* 有效状态 */
    uint32_t debounce;                    /* 消抖时间(ms) */
    uint32_t longPress;                   /* 长按时间(ms) */
    easy_button_state(*read)(void);       /* IO状态读取 */
}easy_button_config;

typedef struct easy_button_context_t
{
	  uint8_t longPress;										/* 长按时间发送记录 */
    uint32_t time;                        /* 累计时间(ms) */
}easy_button_context;


void easy_button_notify(easy_button button, easy_button_event event);
extern const easy_button_config easy_button_config_tab[];
extern const uint8_t easy_button_config_tab_num;

void easy_button_init(void);
void easy_button_scanf(void);

#endif

