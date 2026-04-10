#include "sys.h"


void EXTIX_Init(void)
{
 
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

   //GPIOA.8	  中断线以及中断初始化配置  双边沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}

void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(shield==0&PAin(8)==0)	 //有人破坏锁
	{
		PCout(13)=0;
		PBout(1)=1;
	}
	else
	{
		PCout(13)=1;
		PBout(1)=0;
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //清除LINE4上的中断标志位
}
 