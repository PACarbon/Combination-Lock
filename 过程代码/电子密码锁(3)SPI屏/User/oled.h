#ifndef __OLED_H__
#define __OLED_H__

#include "sys.h"

//配置PB1的输入输出模式
#define SDA_IN()  {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=(u32)8<<4;}//u32：强制转化为无符号,防止被判为符号位，下同
#define SDA_OUT() {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=(u32)3<<4;}
	 
#define IIC_SCL    PBout(0) //SCL
#define IIC_SDA    PBout(1) //SDA	 
#define READ_SDA   PBin(1)  //输入SDA 
#define GPIO_SCL GPIO_Pin_0
#define GPIO_SDA GPIO_Pin_1

u8 IICAdress(u8 addr);
u8 IICWrite(u8 dat);
void IIC_Stop(void);
void IIC_Start(void);
void OLED_Init(void);

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y); 
void OLED_On(void); 
void OLED_Clear(void); 
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Command(unsigned char IIC_Command);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 Size);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
void OLED_ShowNum(u8 x,u8 y,int num,u8 Length);
void OLED_ShowSignedNum(u8 x,u8 y,int num,u8 Length);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif

