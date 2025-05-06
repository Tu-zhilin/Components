
# 【Components】EasyButton

## 1. 前言

**目标**

实现按键抽象层

**功能**

* [x] 硬件无关
* [x] 参数配置
* [x] 按键消抖
* [x] 检测按键短按
* [x] 检测按键长按
* [x] 事件通知上层

## 2. 功能介绍

### 2.1 硬件无关

**用户实现**

按键IO电平状态读取，用户实现。
```
uint8_t easy_button_state_read_xxx(void);
```

### 2.2 按键消抖

按键按下开始，根据配置的过滤时间对IO电平信号进行过滤。

### 2.3 按键检测

#### 2.3.1 短按

当按键按下后，若按下维持时间未达到长按的时间之前就释放了，则识别为短按。

#### 2.3.2 长按

当按键按下后，若按下维持时间达到长按时间，则识别为长按，若在长按状态下按键释放，则向上层发送释放事件。

### 2.4 事件通知

提供一个通知函数，里面内容由用户实现（或者改成用户注册的方式也可以）。

```
void easy_button_notify(easy_button_t button, easy_button_event_t event);
```


## 3. 软件设计

### 3.1 结构体

```C

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
    easy_button_state active; /* 有效状态 */
    uint32_t debounce;        /* 消抖时间 */
    uint32_t longPress;       /* 长按时间 */
    uint8_t(*read)(void);     /* IO状态读取 */
}easy_button_config;

typedef struct easy_button_context_t
{
    uint8_t longPress;		  /* 长按事件发送记录 */
    uint32_t timer;           /* 累计时间 */
}easy_button_context;
```





