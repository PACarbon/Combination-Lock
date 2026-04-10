/****************************
芯片：STM32F103C8T6

实现功能：电子密码锁

引脚：PB0(SCL),PB1(SDA)
******************************/

#include "sys.h"

char change(u8 key);
void input_password(u8 c);
u8 key=0;		//按键返回值
u8 retraction=0;
u8 password[10];
u8 password_check_1[10];
u8 password_check_2[10];
	
int main(void)
{
	
		delay_init();
		LED_GPIO_Config();
		KEY_4x4_Init();
		OLED_Init();
//		SPI_LCD_Init();
		uart1();

		OLED_Clear();
		OLED_ShowString(0,0,"input password",16);
	
		while(1)
		{
				input_password(1);		//上电后输入新密码
				if(key==15)		//按下确认						//可封装
				{
					OLED_Clear();
//					OLED_ShowString(0,0,"input again",16);
//					input_password(2);		//再次输入密码

					OLED_ShowString(0,0,"OK",16);
					delay_ms(750);
					OLED_ShowString(0,0,"Enter the code to open",16);
				}
				
				
//					OLED_ShowNum(0,6,retraction,2);		//text
					key=0;

		}
}

char change(u8 key)
{
	char number;
	switch(key)
	{
		case 1:number=1;break;//OLED_ShowChar(0,0,'1',16);
		case 2:number=2;break;//OLED_ShowChar(0,0,'2',16);
		case 3:number=3;break;//OLED_ShowChar(0,0,'3',16);
		case 4:number='A';break;//OLED_ShowChar(0,0,'A',16);
		case 5:number=4;break;//OLED_ShowChar(0,0,'4',16);
		case 6:number=5;break;//OLED_ShowChar(0,0,'5',16);
		case 7:number=6;break;//OLED_ShowChar(0,0,'6',16);
		case 8:number='B';break;//OLED_ShowChar(0,0,'B',16);
		case 9:number=7;break;//OLED_ShowChar(0,0,'7',16);
		case 10:number=8;break;//OLED_ShowChar(0,0,'8',16);
		case 11:number=9;break;//OLED_ShowChar(0,0,'9',16);
		case 12:number='C';break;//OLED_ShowChar(0,0,'C',16);
	}
	return number;
}

void input_password(u8 c)
{
	u8 i=0;
		while(key!=15)
		{
				KEY_Scan (&key);		//按键扫描

					if(FLAG == 1)    //按键按下
					{
						if(c==1)
							password[i]=key;					//上电的初始密码
						else if(c==2)
							password_check_1[i]=key;	//开门密码的一次密码 & 改密码的一次密码
						else if(c==3)
							password_check_2[i]=key;	//改密码的二次密码
						OLED_ShowChar(retraction,2,'*',16);
						i++;
						retraction+=8;
						FLAG = 0;
					}

		OLED_ShowNum(0,6,retraction,2);		//text
		}
	retraction=0;		//光标清零
}
