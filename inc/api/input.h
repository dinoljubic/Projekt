#ifndef INPUT_H
#define INPUT_H

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/*
Button 1 - PG2
Button 2 - PG3
Button 3 - PD4
*/

#define BUTTON1_RCC_GPIOx				RCC_AHB1Periph_GPIOG
#define BUTTON2_RCC_GPIOx				RCC_AHB1Periph_GPIOG
#define BUTTON3_RCC_GPIOx				RCC_AHB1Periph_GPIOD

#define BUTTON1_GPIOx						GPIOG
#define BUTTON2_GPIOx						GPIOG
#define BUTTON3_GPIOx						GPIOD

#define BUTTON1_PinNumber				GPIO_Pin_2
#define BUTTON2_PinNumber				GPIO_Pin_3
#define BUTTON3_PinNumber				GPIO_Pin_4

//exti
#define BUTTON1_EXTI_PortSourceGPIOx		EXTI_PortSourceGPIOG
#define BUTTON2_EXTI_PortSourceGPIOx		EXTI_PortSourceGPIOG
#define BUTTON3_EXTI_PortSourceGPIOx		EXTI_PortSourceGPIOD

#define BUTTON1_EXTI_PinSourcex					EXTI_PinSource2
#define BUTTON2_EXTI_PinSourcex					EXTI_PinSource3
#define BUTTON3_EXTI_PinSourcex					EXTI_PinSource4

#define BUTTON1_EXTI_Line								EXTI_Line2
#define BUTTON2_EXTI_Line								EXTI_Line3
#define BUTTON3_EXTI_Line								EXTI_Line4

//nvic
#define BUTTON1_NVIC_IRQChannel					EXTI2_IRQn
#define BUTTON2_NVIC_IRQChannel					EXTI3_IRQn
#define BUTTON3_NVIC_IRQChannel					EXTI4_IRQn

//led (legacy)
#define LED3_GREEN_RCC_GPIOx		RCC_AHB1Periph_GPIOG
#define LED4_RED_RCC_GPIOx			RCC_AHB1Periph_GPIOG

#define	LED3_GREEN_GPIOx				GPIOG
#define	LED4_RED_GPIOx					GPIOG

#define LED3_GREEN_PinNumber		GPIO_Pin_13
#define LED4_RED_PinNumber			GPIO_Pin_14

#define LED3_GREEN_ID						1
#define LED4_RED_ID							2

void button_init(void);
void gpio_led_toggle(uint8_t LED_ID);
void gpio_led_init(void);

static void dequeue(void);
static void queue(uint32_t irq);

#endif
