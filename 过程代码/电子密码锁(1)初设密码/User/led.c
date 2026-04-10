#include "sys.h"


void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//≈‰÷√ ±÷”
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
	//≈‰÷√“˝Ω≈		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

}

void LED_change()
{
	static u8 i=0;
	switch(i)
	{
		case 0 : PCout(13)=0;i++;break;
		case 1 : PCout(13)=1;i=0;break;
	}
}
