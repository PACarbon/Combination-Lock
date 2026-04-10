/********************
①使能定时器时钟。
RCC_APB1PeriphClockCmd();
② 初始化定时器，配置ARR,PSC。
TIM_TimeBaseInit();
③使能定时器。
TIM_Cmd();
*****************/
#include "sys.h"


void Pwm_Init(void)//arr为重装载值，psc为预分频系数
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_OCInitTypeDef TIM_OCInitStruct;

		//①使能定时器,PA时钟。
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟

		//定时器3通道3(PB0)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		//初始化TIM3 Channel 3 PWM模式
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStruct.TIM_Pulse = 100; //设置待装入捕获比较寄存器的脉冲值
		TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
		TIM_OC3Init(TIM3, &TIM_OCInitStruct);  //根据T指定的参数初始化外设TIM3 OC2

		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
		TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
		
		TIM3init(19999,71);//50Hz
	
}

void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM3, Compare);																	//改通道记得也改函数名
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare3(Angle / 180 * 2000 + 500);//五个通道
	
}
