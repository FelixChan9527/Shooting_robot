#include "head_file.h"

u8 KeyFlag4=1,KeyFlag5=1,KeyFlag6=1,KeyFlag7=1;

/**************************************************************************************
红外避障程序
***************************************************************************************/
void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM4_IRQHandler()//中断服务函数
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==!RESET)//判断为什么中断
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 0)
			KeyFlag4=0;
		else KeyFlag4=1;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0)
			KeyFlag5=0;
		else KeyFlag5=1;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 0)
			KeyFlag6=0;
		else KeyFlag6=1;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == 0)
			KeyFlag7=0;
		else KeyFlag7=1;
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除标志位
	}
}




