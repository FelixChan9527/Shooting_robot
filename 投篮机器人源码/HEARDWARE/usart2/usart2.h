#ifndef __USART2_H
#define __USART2_H

#include "sys.h"
#include "stdio.h"	
#include "stm32f4xx_conf.h"
     
#define star_send   1
#define stop_send   2
#define red    3
#define blue   4
#define green  5
#define pink 6
#define edge   7
#define center 8
#define pos 9
#define ck_b 10
#define tb_en 11
#define tb_ds 12


void MY_USART2_Init(void);
void Usart2SendByte(char byte);
void ChangeIntSend(int IntSide);
void ChangeDoubleSend(double DoubleSide);
int ChangeStrRes(void);
void ReceiveWaiting(void);
void SendCommand(u16 i);
void Repead(u16 i);

#endif
