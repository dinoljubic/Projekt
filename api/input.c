#include <input.h>

static uint8_t wait_queue = 0;

void gpio_led_init()
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	//init LEDs
	RCC_AHB1PeriphClockCmd(LED3_GREEN_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin		= LED3_GREEN_PinNumber;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_Init(LED3_GREEN_GPIOx, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(LED4_RED_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin		= LED4_RED_PinNumber;
	GPIO_Init(LED4_RED_GPIOx, &GPIO_InitStruct);
}


void button_init(void)
{	
	
	NVIC_InitTypeDef	NVIC_InitStruct;
	EXTI_InitTypeDef	EXTI_InitStruct;
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	//button 1
	GPIO_InitStruct.GPIO_Pin		= BUTTON1_PinNumber;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_NOPULL;//
	GPIO_Init(BUTTON1_GPIOx, &GPIO_InitStruct);
	
	EXTI_InitStruct.EXTI_Line		= BUTTON1_EXTI_Line;
	EXTI_InitStruct.EXTI_Mode		= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= BUTTON1_NVIC_IRQChannel	;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	RCC_AHB1PeriphClockCmd(BUTTON1_RCC_GPIOx, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(BUTTON1_EXTI_PortSourceGPIOx, BUTTON1_EXTI_PinSourcex);
	
	//button 2
	GPIO_InitStruct.GPIO_Pin		= BUTTON2_PinNumber;
	GPIO_Init(BUTTON2_GPIOx, &GPIO_InitStruct);
	
	EXTI_InitStruct.EXTI_Line		= BUTTON2_EXTI_Line;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= BUTTON2_NVIC_IRQChannel;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	RCC_AHB1PeriphClockCmd(BUTTON2_RCC_GPIOx, ENABLE);
	SYSCFG_EXTILineConfig(BUTTON2_EXTI_PortSourceGPIOx, BUTTON2_EXTI_PinSourcex);
	
	//button 3
	GPIO_InitStruct.GPIO_Pin		= BUTTON3_PinNumber;
	GPIO_Init(BUTTON3_GPIOx, &GPIO_InitStruct);
	
	EXTI_InitStruct.EXTI_Line		= BUTTON3_EXTI_Line;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel	= BUTTON3_NVIC_IRQChannel;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStruct);
	
	RCC_AHB1PeriphClockCmd(BUTTON3_RCC_GPIOx, ENABLE);
	SYSCFG_EXTILineConfig(BUTTON3_EXTI_PortSourceGPIOx, BUTTON3_EXTI_PinSourcex);
}


//3 exti, ako se moze preko jednog, polling
void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line2);
		queue(EXTI2_IRQn);
		
		gpio_led_toggle(1);
	}
}

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line3);
		queue(EXTI3_IRQn);
		
		gpio_led_toggle(2);
	}
}


void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		//queue(EXTI4_IRQn);
		//za test bez timera:
		dequeue();
		//promijeni trenutni blok - inace radi timer
		wait_queue ^= 0x02;
	}
}

static void queue(uint32_t irq){
	uint8_t top = 0;
	
	NVIC_DisableIRQ(irq);
	
	switch (irq){
		case EXTI2_IRQn:
			top = top | 0x80;
			break;
		case EXTI3_IRQn:
			top |= 0x40;
			break;
		case EXTI4_IRQn:
			top |= 0x20;
			break;
	}
	
	//puni sljedeci blok
	if (!(wait_queue & 0x02))
		top = top >> 3;
	
	wait_queue |= top;
}

static void dequeue(void){
	uint8_t top = wait_queue;
	
	if (wait_queue & 0x02){
			top = top << 3;
			wait_queue &= 0xE3;
	}
	else wait_queue &= 0x1F;
	
	if (top & 0x80)
		NVIC_EnableIRQ(EXTI2_IRQn);
	if (top & 0x40)
		NVIC_EnableIRQ(EXTI3_IRQn);
	if (top & 0x20)
		NVIC_EnableIRQ(EXTI4_IRQn);
	
}

void gpio_led_toggle(uint8_t LED_ID)
{
	switch(LED_ID)
	{
		case LED3_GREEN_ID:
			GPIO_ToggleBits(LED3_GREEN_GPIOx, LED3_GREEN_PinNumber);
			break;
		case LED4_RED_ID:
			GPIO_ToggleBits(LED4_RED_GPIOx, LED4_RED_PinNumber);
			break;
	}
}
