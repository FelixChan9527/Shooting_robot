#include "head_file.h"

extern int X, Y, H1, H2, H3, H4;
#define dianji1 PCout(6)
#define dianji2 PCout(7)
#define dianji3 PCout(8)
#define dianji4 PCout(9)                  //四个电机编号,奇数在左，偶数在右

int pwm_dianji1=0, pwm_dianji2=0, pwm_dianji3=0, pwm_dianji4=0;
u16 ServoPitch, ServoYaw,
	ServoArm ,ServoHand,
	launch_pwm=360;		
//行走电机、摄像头俯仰舵机、摄像头横滚舵机,发球装置电机

u16 timer=0;


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
		
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		timer++;
		
		if(timer == 2000)
			timer = 0;
		
		if(timer < pwm_dianji1)
			dianji1 = 1;				
		else if(timer >= pwm_dianji1)
			dianji1 = 0;		
		
		if(timer < pwm_dianji2)
			dianji2 = 1;
		else if(timer >= pwm_dianji2)
			dianji2 = 0;
		
		if(timer < pwm_dianji3)
			dianji3 = 1;
		else if(timer >= pwm_dianji3)
			dianji3 = 0;
		
		if(timer < pwm_dianji4)
			dianji4 = 1;
		else if(timer >= pwm_dianji4)
			dianji4 = 0;	
		
		if(timer < ServoPitch)		
			PCout(4) = 1;
		else if(timer > ServoPitch)
			PCout(4) = 0;
		
		if(timer < ServoYaw)		
			PCout(5) = 1;
		else if(timer > ServoYaw)
			PCout(5) = 0;
		
		if(timer < ServoArm)		
			PCout(2) = 1;
		else if(timer > ServoArm)
			PCout(2) = 0;
		
		if(timer < ServoHand)		
			PCout(3) = 1;
		else if(timer > ServoHand)
			PCout(3) = 0;
		
		if(timer < launch_pwm)		
		{PFout(9) = 1;PFout(8) = 0;}
		else if(timer > launch_pwm)
		{PFout(9) = 0;PFout(8) = 0;}
		
	}
	TIM_ClearITPendingBit(TIM3 ,TIM_IT_Update);  //清除中断标志位
}


