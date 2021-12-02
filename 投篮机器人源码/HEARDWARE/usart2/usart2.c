#include "head_file.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////此文件为树莓派与单片机通信的协议，不可修改！！！////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


u8 res, flag=0, CheckFlag = 0;
u16 Position = 0;
int X,Y,HandX=137,HandY=82,BallX=153,BallY=101;	//树莓派发送过来的坐标信息，通过串口2赋值与X、Y
char ResData[40];			//接收缓存数组
extern u16 step;

void MY_USART2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//串口使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//IO口使能
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
		    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//IO口初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=115200;//串口初始化
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART2,&USART_InitStructure);
		
	USART_Cmd(USART2,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;//中断初始化
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
}


//////////////////////////////////////////////////////////////////////////////////////
void Usart2SendByte(char byte)//发送一个字节
{
	USART2->SR; 
	USART_SendData(USART2, (uint8_t) byte);
	while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);	
}


void SendCommand(u16 i)	//发送至树莓派的指令
{
	u16 j;
	u8 
	StarSand[3] = "<K>",	//开启树莓派位置横坐标数据的发送
	StopSend[3] = "<S>",	//关闭树莓派位置横坐标数据的发送
	Red[3] = "<R>",			//切换阈值至红色
	Blue[3] = "<B>",		//切换阈值至蓝色
	Green[3] = "<G>",		//切换阈值至绿色
	Pink[3] = "<P>",		//切换阈值至橙色
	Edge[3] = "<E>",		//切换切换至发送边缘最高点指令
	Center[3] = "<C>",		//切换切换至发送圆心指令
	JAW_POS[9] = "<JAW_POS=",
	CK_B[6] = "<CK=",
	TB_EN[7] = "<TB_EN>",
	TB_DS[7] = "<TB_DS>";
//	StarSandY[3] = "<Y>";	//开启树莓派位置纵坐标数据的发送
	
	switch (i)
	{
		case star_send:
		{
			for(j = 0;j<=2;j++)
				{Usart2SendByte(StarSand[j]);}
			break;
		}
		case stop_send:
		{
			for(j = 0;j<=2;j++)
				{Usart2SendByte(StopSend[j]);}
			break;
		}
		case red:
		{
			led_r = 0;led_g = 1;led_b = 1;
			for(j = 0;j<=2;j++)
				{Usart2SendByte(Red[j]);}
			break;
		}
		case blue:
		{
			led_r = 1;led_g = 1;led_b = 0;
			for(j = 0;j<=2;j++)
				{Usart2SendByte(Blue[j]);}
			break;
		}
		case green:
		{
			led_r = 1;led_g = 0;led_b = 1;
			for(j = 0;j<=2;j++)
				{Usart2SendByte(Green[j]);}
			break;
		}
		case pink:
		{
			led_r = 1;led_g = 1;led_b = 1;
			for(j = 0;j<=2;j++)
				{Usart2SendByte(Pink[j]);}
			break;
		}
		case edge:
		{
			for(j = 0;j<=2;j++)
				{Usart2SendByte(Edge[j]);}
			break;
		}
		case center:
		{
			for(j = 0;j<=2;j++)
				{Usart2SendByte(Center[j]);}
			break;
		}
		case pos:
		{
			for(j = 0;j<=8;j++)
				{Usart2SendByte(JAW_POS[j]);}
			ChangeIntSend(HandX);
			Usart2SendByte(',');
			ChangeIntSend(HandY);
			Usart2SendByte('>');
			break;
		}
		case ck_b:
		{
			for(j = 0;j<=5;j++)
				{Usart2SendByte(CK_B[j]);}
			ChangeIntSend(BallX);
			Usart2SendByte(',');
			ChangeIntSend(BallY);
			Usart2SendByte('>');
			break;
		}
		case tb_en:
		{
			for(j = 0;j<=6;j++)
				{Usart2SendByte(TB_EN[j]);}
			break;
		}
		case tb_ds:
		{
			for(j = 0;j<=6;j++)
				{Usart2SendByte(TB_DS[j]);}
			break;
		}
	}
}


void Repead(u16 i)		//因为树莓派跟单片机通讯有数据丢失现象，所以需要一条指令发送多次
{						//延时可以长一点，最好不要太短，太短会造成数据接收错误和丢失，200ms延时
	SendCommand(i);		//重复5次发送，基本上OK
	SendCommand(i);
	SendCommand(i);
	SendCommand(i);
	SendCommand(i);
}


void ChangeIntSend(int IntSide)		//整型数据转换为字符串并通过串口发送
{
	unsigned int i = 0,ReadySend;
	char CharString[30];
	ReadySend = sprintf(CharString, "%d", IntSide);
	
	while(ReadySend>0)
	{
		Usart2SendByte(CharString[i]);
		ReadySend--;
		i++;
	}	
}


void ChangeDoubleSend(double DoubleSide)		//浮点数据转换为字符串并通过串口发送
{
	unsigned int i = 0,ReadySend;
	char CharString[30];
	ReadySend = sprintf(CharString, "%f", DoubleSide);
	
	while(ReadySend>0)
	{
		Usart2SendByte(CharString[i]);
		ReadySend--;
		i++;
	}
	Usart2SendByte('\r');	
	Usart2SendByte('\n');	
}


void ReceiveWaiting(void)				//数据读取缓存
{
	if((res == '>') && (ResData[0] == '<'))	//判断是否接收完毕，若接收到帧头帧尾，则接收完毕，flag置位
		flag = 1;
	else flag = 0;						//若没有接收到，则flag一直为0
		
	if(flag == 0)						//flag=0，即未接收完数据，则将接收的每一位放进数据缓存数组中
	{									//切记，单片机所接收的整型数据不超过65535，否则出错
		ResData[Position] = res;
		Position++;
	}
	else if(flag == 1)					//若接收完毕，将数据转换为数值并将数值转换为字符串发送至上位机
	{
		switch (ResData[1]) 
		{
			case 'N' :{X = 10000,Y = 10000;Position = 0;break;}//接收到'N'，没目标，X，Y变为10000
			case 'X' :{X = ChangeStrRes();break;}//接收到X，即为识别的X坐标，其后的数据即X坐标信息
			case 'Y' :{Y = ChangeStrRes();break;}//接收到X，即为识别的X坐标，其后的数据即X坐标信息
			case 'T' :{CheckFlag=1;break;}
		}
	}
}	


int ChangeStrRes(void)			//将接收到的整型字符串数据转为整型数值
{
	unsigned int i=0, j,RealNumber;
	
	if(flag == 1)							//串口数据接收完毕，进行字符串到数值的转化
	{
		for(j=2;j<Position;j++)				//接收缓存数组移位
			ResData[j-2] = ResData[j];		
		ResData[Position-1] = 0;			//最后一位清零
		ResData[Position-2] = 0;			//最后两位清零
		
		RealNumber = atoi(ResData);			//将缓存字符数组转为整型数字
		Position = 0;						//数据指针位置指向首地址
		flag = 0;								//串口接收标志位清零
	}		
	for(i = 0; i < 40; i ++)	//数据缓存区清零
		ResData[i] = 0;
	
	return 	RealNumber;				//返回转换后的数值
}


void USART2_IRQHandler(void)//串口中断服务函数
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		led_2 = 0;
		led_3 = !led_3;
		res = USART_ReceiveData(USART2);
		ReceiveWaiting();
	}
}



 




