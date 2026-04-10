#ifndef __USART_H
#define __USART_H
#include "sys.h"

void uart1(void);
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data);
void USART_SendString( USART_TypeDef* USARTx, char *str);
uint8_t USART_ReceiveByte(USART_TypeDef* USARTx);

#endif
