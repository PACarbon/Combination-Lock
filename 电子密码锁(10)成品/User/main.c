/****************************
芯片：STM32F103C8T6

实现功能：电子密码锁

引脚：PB3(SCK),PB5(SDA),PB6(CS),PB7(DC),PB8(BL),PA0~7(KEY),PB0(SERVO),PB1(BEEP),PA8(INFRARED)
******************************/

#include "sys.h"

#define MAX_PASSWORD_LENGTH 13		//密码长度
#define MAX_TRIES 			3         // 最大尝试次数
#define KEY_CONFIRM			15					//按键确认值
#define KEY_UP				12					//向上翻页
#define KEY_DOWN			16					//向下翻页

void setupPassword(void);						//初始化密码函数
void input_password(u8 c);						//输入密码函数
int arraysEqual(int array1[], int array2[], int size);				//密码比较函数
void open_password(void);									//开门函数
void change_password(void);							//更改密码函数

u8 key=0;		//按键返回值
u8 retraction=27;		//*位置
int initialized = 0;  // 用于检测是否已初始化
int flag=0;		//功能标志位
u8 shield=1;	//开锁屏蔽红外
int password[MAX_PASSWORD_LENGTH]={0};					//存放密码数组
int password_check_1[MAX_PASSWORD_LENGTH]={0};	//密码中转站1
int password_check_2[MAX_PASSWORD_LENGTH]={0};	//密码中转站2

int main(void)
{
		delay_init();					//延时初始化
		LED_GPIO_Config();		//LED,BEEP,INFREAD初始化
		KEY_4x4_Init();				//矩阵键盘初始化
		SPI_LCD_Init();				//TFT液晶屏初始化
		Pwm_Init();						//舵机初始化
		EXTIX_Init();					//外部中断初始化

		LCD_SetDirection(Direction_V);		//正向
		LCD_SetTextFont(&CH_Font24);			// 设置2424中文字体,ASCII字体对应为2412
		LCD_SetColor(LCD_BLACK);				// 设置画笔颜色
		LCD_SetBackColor(LIGHT_CYAN);			//青色
		LCD_Clear();		// 清屏
		LCD_DisplayString(40,20,"put password");
		LCD_DisplayString(10,200,"Press # to confirm");
		Servo_SetAngle(0);		//复位
	
		while(1)
		{
			if (!initialized) 		//设置新密码
			{
				setupPassword();
			}
																											//（1）输入密码开门（2）更改密码
			KEY_Scan (&key);
			switch (key) 
			{
				case KEY_UP:
					LCD_DisplayString(15, 40, "-> Enter password");
					LCD_DisplayString(15, 70, "   Change password");
					LCD_DisplayString(10,200,"Press # to confirm");
					LCD_DisplayString(150,140,"C:up");
					LCD_DisplayString(150,170,"D:down");
					flag = 1;				//输入密码
					break;
				case KEY_DOWN:
					LCD_DisplayString(15, 40, "   Enter password");
					LCD_DisplayString(15, 70, "-> Change password");
					LCD_DisplayString(10,200,"Press # to confirm");
					LCD_DisplayString(150,140,"C:up");
					LCD_DisplayString(150,170,"D:down");
					flag = 2;				//改密码
					break;
				case KEY_CONFIRM:
					if (flag == 1) open_password();
					if (flag == 2) change_password();
					break;
			}
		}
}


/*设置密码*/
void setupPassword(void)
{
	 input_password(1);  // 上电后输入新密码
	 LCD_DisplayString(20, 150, "OK");
	 key = KEY_UP;
	 FLAG = 1;
	 delay_ms(750);  // 延时750毫秒
	 LCD_Clear();  // 清屏
	 initialized = 1;  // 标记已初始化，不再执行此代码块
	 shield = 0;
}

void input_password(u8 mode)
{
    u8 i = 0;
    while (1)
		{
			KEY_Scan(&key);  // 按键扫描
			if (key == KEY_CONFIRM)
			{
				break;  // 如果按下确认键，退出循环
			}
			if (i < MAX_PASSWORD_LENGTH)
			{
				if (FLAG == 1 && key != KEY_UP && key != KEY_DOWN && key != KEY_CONFIRM) 		 // 按键按下且不是特殊按键
				{
					switch (mode)
					{
						case 1:
							password[i] = key;  // 设置新密码
							break;
						case 2:
							password_check_1[i] = key;  // 输入密码校验
							break;
						case 3:
							password_check_2[i] = key;  // 二次密码输入
							break;
					}
					LCD_DisplayString(retraction, 80, "*");
//					LCD_DisplayNumber(retraction, 80,key, 1);
					i++;
					retraction += 15;
					FLAG = 0;
				}
			}
        LCD_DisplayNumber(20, 250, i, 1);  // 显示当前输入的字符数
		}
    retraction = 27;  // 重置*位置
}

int arraysEqual(int array1[], int array2[], int size)
{
    for (int i = 0; i < size; i++) 
	{
        if (array1[i] != array2[i]) 
		{
            return 0; // 数组不相等
        }
    }
    return 1; // 数组相等
}

/*开门函数*/

void open_password(void)								//输入密码
{
		int attempts = MAX_TRIES;
		key=KEY_UP;
		LCD_Clear();
		while(attempts--)				//三次机会
		{
			LCD_DisplayString(35,20,"Enter password");
			input_password(2);		//开门输入密码
			LCD_Clear();
			key=0;
			FLAG=0;
			if (arraysEqual(password, password_check_1, MAX_PASSWORD_LENGTH))	
				break;//比较数组内的内容
		}
			if(attempts>0)			//正确开锁
			{
				shield=1;
				LCD_DisplayString(40,60,"Access Granted");
				LCD_DisplayString(25,190,"Press # to close");
				Servo_SetAngle(90);										//开门
			}
			else
			{
				LCD_SetColor(LCD_RED);				// 设置画笔颜色
				LCD_DisplayString(45,60,"Access Denied");
				PCout(13)=0;																	//警报(灯光)
				PBout(1)=1;																		//警报(蜂鸣器)
				delay_ms(1000);delay_ms(1000);delay_ms(1000);
				LCD_DisplayString(25,190,"Press # to close");
				LCD_SetColor(LCD_BLACK);				// 设置画笔颜色
			}
			while(key==0)
			{
					KEY_Scan (&key);		//按键扫描
				if(key == KEY_CONFIRM)
				{
					shield=0;
					Servo_SetAngle(0);
					LED_change();
					break;
				}
			}
			memset(password_check_1, 0, sizeof(password_check_1));			//数组清零

		key=KEY_UP;
		FLAG=1;
		PCout(13)=1;																		//关闭警报(灯光)
		PBout(1)=0;																			//关闭警报(蜂鸣器)
		LCD_Clear();
		
}


/*改密码*/
void change_password(void)								//改密码
{
		key=KEY_DOWN;
		LCD_Clear();
		LCD_DisplayString(25,20,"Enter previous");
		LCD_DisplayString(25,45,"password");
		LCD_DisplayString(10,200,"Press # to confirm");
		input_password(2);
		key=0;
		FLAG=0;
		if (arraysEqual(password, password_check_1, MAX_PASSWORD_LENGTH))							//新旧密码校验
		{
			LCD_Clear();
			LCD_DisplayString(50,20,"new password");
			LCD_DisplayString(10,200,"Press # to confirm");
			input_password(3);
			LCD_Clear();

			 if (arraysEqual(password_check_1, password_check_2, MAX_PASSWORD_LENGTH))			//检测新旧密码是否相同
				 LCD_DisplayString(12,45,"Duplicate password");
			 else
			 {
				 LCD_DisplayString(25,45,"Password updated");							//唯一真理
				 memcpy(password, password_check_2, sizeof(password));			//覆盖新密码
			 }
		}
		else
		{
			LCD_Clear();
			LCD_DisplayString(22,40,"Wrong password!!!");
			PCout(13)=0;																	//警报(灯光)
			PBout(1)=1;																		//警报(蜂鸣器)
			delay_ms(1000);
		}
		memset(password_check_1, 0, sizeof(password_check_1));			//数组清零
		memset(password_check_2, 0, sizeof(password_check_2));			//数组清零
		key=KEY_DOWN;
		FLAG=1;
		delay_ms(1500);
		PBout(1)=0;																			//关闭警报(蜂鸣器)
		PCout(13)=1;																		//关闭警报(灯光)
		LCD_Clear();
}

//void text(u8 a)
//{
//	char c;
//	switch(a)
//	{
//		case 1:break;
//	}
//}
