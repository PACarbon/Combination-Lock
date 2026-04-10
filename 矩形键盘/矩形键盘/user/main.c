#include "stm32f10x.h"                  // Device header
#include "Delay.H"
#include "key.H"

/****************************************************/
/*引脚使用说明：									*/
/*				矩形键盘： C1~C4对应GPIOA 4,5,6,7   */  
/*						   R1~R4对应GPIOA 0,1,2,3   */
/*				LED中的R,G,B对应GPIOB 10,11,12      */
/****************************************************/


void GPIO_Config(void)    //对GPIOB进行配置初始化
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE );
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure .GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_Initstructure .GPIO_Pin=0X1D00 ;    //0001 1100 0000 0000
	GPIO_Initstructure .GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init (GPIOB ,&GPIO_Initstructure );
}

void LED_RGB(float  time)
{	
	u16 t =time *1000;		
	GPIO_Write (GPIOB ,0X0400);  // 0000 0100 0000 0000
	Delay_ms (t);
	GPIO_Write (GPIOB ,0X0800);  // 0000 1000 0000 0000
	Delay_ms (t);
	GPIO_Write (GPIOB ,0X1000);  // 0001 0000 0000 0000
	Delay_ms (t);
	GPIO_Write (GPIOB ,0X0000);
}

u8 key=0;
u8 k =0;
int main(void)
{
	GPIO_Config ();
	KEY_4x4_Init();
	while(1)
	{
		KEY_Scan (&key);
		if(FLAG == 1)    //按键按下
		{
			FLAG = 0;
			if(key==1)  //按下S1，亮红灯	
			{
				GPIO_SetBits (GPIOB ,GPIO_Pin_10 );
			}	
			else if(key==2)  //按下S2，亮绿灯
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_11 );
			}
			else if(key==3)		//按下S3，亮蓝灯
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_12 );
			}
			else if(key==4)   //按下S4，关闭所有灯
			{
				GPIO_Write (GPIOB,0X0000);   //0000 0000 0000 0000
			}
			else if(key==5)
			{
				LED_RGB (0.3);   //子函数参数为时间，单位为s
			}
			else if(key==6)
			{
				for(k=0;k<3;k++)
				{
				LED_RGB (0.5);
				}
			}		
		}
	}
}
