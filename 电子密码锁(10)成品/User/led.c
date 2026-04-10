#include "sys.h"


void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//配置时钟
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);                //使能
	
	//配置引脚		
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;						//蜂鸣器     推挽输出 输出0时N-MOS导通输出0 1时P-MOS导通输出1
	GPIO_InitStruct.GPIO_Pin = BEEP_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//红外   浮空输入   不确定
	GPIO_InitStruct.GPIO_Pin = INFRARED_GPIO_PIN;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(INFRARED_GPIO_PORT, &GPIO_InitStruct);
	
	PCout(13)=1;
	PBout(1)=0;
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
