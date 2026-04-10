/****************************
芯片：STM32F103C8T6

实现功能：电子密码锁

引脚：PB0(SCL),PB1(SDA)
******************************/

#include "sys.h"

char change(u8 key);
void input_password(u8 c);
int arraysEqual(int array1[], int array2[], int size);
void LCD_Test_Clear(void);

u8 key=0;		//按键返回值
u8 retraction=22;
int j=3;
int password[10]={0};
int password_check_1[10]={0};
int password_check_2[10]={0};
	
int main(void)
{
	
		delay_init();
		LED_GPIO_Config();
		KEY_4x4_Init();
		SPI_LCD_Init();
//		uart1();

		LCD_SetDirection(Direction_V);		
		LCD_SetTextFont(&CH_Font24);			// 设置2424中文字体,ASCII字体对应为2412
		LCD_SetColor(LCD_BLACK);				// 设置画笔颜色
		LCD_SetBackColor(LCD_CYAN);
		LCD_Clear();		// 清屏
		LCD_DisplayString(22,20,"put new password");
		LCD_DisplayString(10,200,"Press # to confirm");
	
		while(1)
		{
				input_password(1);		//上电后输入新密码
			LCD_DisplayString(20,150,"OK");
				key=0;
				FLAG=0;
				delay_ms(750);
				LCD_Clear();

				while(j--)				//三次机会
				{
					LCD_DisplayString(10,20,"Enter password");
					input_password(2);		//开门输入密码
					if (arraysEqual(password, password_check_1, 10))
						break;
					LCD_Clear();
					key=0;
					FLAG=0;
				}
				LCD_Clear();
				if(j!=-1)			//正确开锁
				{
					LCD_DisplayString(80,60,"True");
															//开门
				}
				else
				{
					LCD_DisplayString(80,60,"Warn!!!");
					PCout(13)=0;
				}
				key=0;
				FLAG=0;
				delay_ms(1000);delay_ms(1000);delay_ms(1000);
				
				
//			for(int e=0,c=0;e<7;e++)					//检查密码
//			{
//				OLED_ShowNum(c,6,password[e],2);
//				c+=16;
//			}

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

					if(FLAG == 1&key!=15)    //按键按下
					{
						if(c==1)
							password[i]=key;					//上电的初始密码
						else if(c==2)
							password_check_1[i]=key;	//开门密码的一次密码 & 改密码的一次密码
						else if(c==3)
							password_check_2[i]=key;	//改密码的二次密码
						LCD_DisplayString(retraction,50,"*");
						i++;
						retraction+=15;
						FLAG = 0;
					}

				LCD_DisplayNumber(20,250,i,1);
		}
	retraction=22;		//光标清零
}

int arraysEqual(int array1[], int array2[], int size) {
    for (int i = 0; i < size; i++) {
        if (array1[i] != array2[i]) {
            return 0; // 数组不相等
        }
    }
    return 1; // 数组相等
}
