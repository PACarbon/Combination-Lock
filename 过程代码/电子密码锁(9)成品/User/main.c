/****************************
芯片：STM32F103C8T6

实现功能：电子密码锁

引脚：PB3(SCK),PB5(SDA),PB6(CS),PB7(DC),PB8(BL),PA0~7(KEY),PB0(SERVO),PB1(BEEP),PA8(INFRARED)
******************************/

#include "sys.h"

void input_password(u8 c);
int arraysEqual(int array1[], int array2[], int size);
void open_password(void);
void change_password(void);

u8 key=0;		//按键返回值
u8 retraction=27;
int i=1,j=3;
int flag=0;		//功能标志位
u8 shield=0;	//开锁屏蔽红外
int password[12]={0};
int password_check_1[12]={0};
int password_check_2[12]={0};

int main(void)
{
	
		delay_init();
		LED_GPIO_Config();
		KEY_4x4_Init();
		SPI_LCD_Init();
		Pwm_Init();
		EXTIX_Init();

		LCD_SetDirection(Direction_V);		
		LCD_SetTextFont(&CH_Font24);			// 设置2424中文字体,ASCII字体对应为2412
		LCD_SetColor(LCD_BLACK);				// 设置画笔颜色
		LCD_SetBackColor(LIGHT_CYAN);
		LCD_Clear();		// 清屏
		LCD_DisplayString(22,20,"put new password");
		LCD_DisplayString(10,200,"Press # to confirm");
		Servo_SetAngle(0);
	
		while(1)
		{
				while(i)
				{
					shield=1;
					input_password(1);		//上电后输入新密码
					LCD_DisplayString(20,150,"OK");
					key=12;
					FLAG=1;
					delay_ms(750);
					LCD_Clear();
					i--;					//只进入一次
					shield=0;
				}
																											//（1）输入密码开门（2）更改密码
				KEY_Scan (&key);
				if(FLAG == 1&key==12)
				{
					LCD_DisplayString(15,40,"-> Enter password");
					LCD_DisplayString(15,70,"   Change password");
					LCD_DisplayString(10,200,"Press # to confirm");
					LCD_DisplayString(150,140,"C:up");LCD_DisplayString(150,170,"D:down");
					flag=1;				//输入密码
				}
				if(FLAG == 1&key==16)
				{
					LCD_DisplayString(15,40,"   Enter password");
					LCD_DisplayString(15,70,"-> Change password");
					LCD_DisplayString(10,200,"Press # to confirm");
					LCD_DisplayString(150,140,"C:up");LCD_DisplayString(150,170,"D:down");
					flag=2;				//改密码
				}

				if(flag==1&key==15)		//输入密码
					open_password();
				
				if(flag==2&key==15)			//改密码
					change_password();
				
//			for(int e=0,c=0;e<7;e++)					//检查密码
//			{
//				OLED_ShowNum(c,6,password[e],2);
//				c+=16;
//			}
		}
}

void input_password(u8 c)
{
	u8 i=0;
		while(key!=15)
		{
				KEY_Scan (&key);		//按键扫描

					if(FLAG == 1&key!=15&key!=12&key!=16)    //按键按下
					{
						if(c==1)
							password[i]=key;					//上电的初始密码
						else if(c==2)
							password_check_1[i]=key;	//开门密码的一次密码 & 改密码的一次密码
						else if(c==3)
							password_check_2[i]=key;	//改密码的二次密码
						LCD_DisplayString(retraction,80,"*");
						i++;
						retraction+=15;
						FLAG = 0;
					}

				LCD_DisplayNumber(20,250,i,1);
		}
	retraction=27;		//光标清零
}

int arraysEqual(int array1[], int array2[], int size) {
    for (int i = 0; i < size; i++) {
        if (array1[i] != array2[i]) {
            return 0; // 数组不相等
        }
    }
    return 1; // 数组相等
}

void open_password(void)								//输入密码
{
		j=3;
		key=12;
		LCD_Clear();
		while(j--)				//三次机会
		{
			LCD_DisplayString(35,20,"Enter password");
			input_password(2);		//开门输入密码
			LCD_Clear();
			key=0;
			FLAG=0;
			if (arraysEqual(password, password_check_1, 12))
				break;
		}
			if(j!=-1)			//正确开锁				//自动和手动
			{
				shield=1;
				LCD_DisplayString(90,60,"True");
				LCD_DisplayString(25,190,"Press # to close");
				Servo_SetAngle(90);										//开门
			}
			else
			{
				LCD_DisplayString(80,60,"Warn!!!");
				PCout(13)=0;																	//警报(灯光)
				PBout(1)=1;																		//警报(蜂鸣器)
				delay_ms(1000);delay_ms(1000);delay_ms(1000);
				LCD_DisplayString(25,190,"Press # to close");
			}
			while(key==0)
			{
					KEY_Scan (&key);		//按键扫描
				if(key==15)
				{
					shield=0;
					Servo_SetAngle(0);
					LED_change();
					break;
				}
			}
			memset(password_check_1, 0, sizeof(password_check_1));			//数组清零

		key=12;
		FLAG=1;
		PCout(13)=1;																		//关闭警报(灯光)
		PBout(1)=0;																			//关闭警报(蜂鸣器)
		LCD_Clear();
		
}

void change_password(void)								//改密码
{
		key=16;
		LCD_Clear();
		LCD_DisplayString(25,20,"Enter previous");
		LCD_DisplayString(25,45,"password");
		LCD_DisplayString(10,200,"Press # to confirm");
		input_password(2);
		key=0;
		FLAG=0;
		if (arraysEqual(password, password_check_1, 12))							//输入新密码
		{
			LCD_Clear();
			LCD_DisplayString(50,20,"new password");
			LCD_DisplayString(10,200,"Press # to confirm");
			input_password(3);
			LCD_Clear();

			 if (arraysEqual(password_check_1, password_check_2, 12))			//输入相同的新密码
				 LCD_DisplayString(12,45,"Duplicate password");
			 else
			 {
				 LCD_DisplayString(33,45,"successfully!!!");							//唯一真理
				 for (int i = 0; i < 12; i++) {
					 password[i] = password_check_2[i];									//覆盖新密码
				}
			 }
		}
		else
		{
			LCD_Clear();
			LCD_DisplayString(22,40,"Wrong password!!!");
			PCout(13)=0;																	//警报(灯光)
			PBout(1)=1;																							//警报(蜂鸣器)
			delay_ms(1000);
		}
		memset(password_check_1, 0, sizeof(password_check_1));			//数组清零
		memset(password_check_2, 0, sizeof(password_check_2));			//数组清零
		key=12;
		FLAG=1;
		delay_ms(1000);delay_ms(500);
		PBout(1)=0;																			//关闭警报(蜂鸣器)
		PCout(13)=1;																		//关闭警报(灯光)
		LCD_Clear();
}
