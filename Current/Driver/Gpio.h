#ifndef _GPIO_H_
#define _GPIO_H_
#include "stm32f10x.h"

#define LED_GPIO_PORT                 GPIOF
#define LED1_GPIO_PIN                 GPIO_Pin_6
#define LED2_GPIO_PIN                 GPIO_Pin_7
#define LED3_GPIO_PIN                 GPIO_Pin_8
#define LED4_GPIO_PIN                 GPIO_Pin_9
#define LED5_GPIO_PIN                 GPIO_Pin_10
#define LED_GPIO_CLK                  RCC_APB2Periph_GPIOF

#define LED1(x)   ((x) ? (GPIO_SetBits(LED_GPIO_PORT, LED1_GPIO_PIN)) : (GPIO_ResetBits(LED_GPIO_PORT, LED1_GPIO_PIN)));
#define LED2(x)   ((x) ? (GPIO_SetBits(LED_GPIO_PORT, LED2_GPIO_PIN)) : (GPIO_ResetBits(LED_GPIO_PORT, LED2_GPIO_PIN)));
#define LED3(x)   ((x) ? (GPIO_SetBits(LED_GPIO_PORT, LED3_GPIO_PIN)) : (GPIO_ResetBits(LED_GPIO_PORT, LED3_GPIO_PIN)));
#define LED4(x)   ((x) ? (GPIO_SetBits(LED_GPIO_PORT, LED4_GPIO_PIN)) : (GPIO_ResetBits(LED_GPIO_PORT, LED4_GPIO_PIN)));
#define LED5(x)   ((x) ? (GPIO_SetBits(LED_GPIO_PORT, LED5_GPIO_PIN)) : (GPIO_ResetBits(LED_GPIO_PORT, LED5_GPIO_PIN)));


#define BEEP_GPIO_PORT                GPIOB
#define BEEP_GPIO_PIN                 GPIO_Pin_2
#define BEEP_GPIO_CLK                 RCC_APB2Periph_GPIOB

#define KEY2_3_GPIO_PORT              GPIOA
#define KEY2_GPIO_PIN                 GPIO_Pin_0
#define KEY3_GPIO_PIN                 GPIO_Pin_8

#define KEY4_GPIO_PORT                GPIOC
#define KEY4_GPIO_PIN                 GPIO_Pin_13

#define KEY5_GPIO_PORT                GPIOD
#define KEY5_GPIO_PIN                 GPIO_Pin_3

#define KEY2_3_GPIO_CLK               RCC_APB2Periph_GPIOA
#define KEY4_GPIO_CLK                 RCC_APB2Periph_GPIOC
#define KEY5_GPIO_CLK                 RCC_APB2Periph_GPIOD
void LED_Init(void);
void LED_Toggle(uint8_t led);
void BEEP_Init(void);
void Key_Init(void);

#endif
