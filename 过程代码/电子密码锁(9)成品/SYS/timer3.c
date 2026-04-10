/********************
①使能定时器时钟。
RCC_APB1PeriphClockCmd();
② 初始化定时器，配置ARR,PSC。
TIM_TimeBaseInit();
③定时器中断配置，同时配置NVIC。
void TIM_ITConfig();
NVIC_Init();
④使能定时器。
TIM_Cmd();
⑤编写中断服务函数。
TIMx_IRQHandler();
*****************/
#include "sys.h"


void TIM3init(u16 arr,u16 psc)//arr为重装载值，psc为预分频系数
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;

	//①使能定时器时钟。
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	//② 初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

//	//③定时器中断配置，同时配置NVIC。
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//更新中断
//	
//	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);	
	
	//④使能定时器。
	TIM_Cmd(TIM3,ENABLE);
	
}

////⑤编写中断服务函数。
//void TIM3_IRQHandler(void)
//{
//	static u8 i =0 ;
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
//			switch (i)
//			{
//				case 0:GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);i++;break;
//				case 1:GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);i=0;break;
//			
//			}
//			
//			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
//			
//		}

//}
