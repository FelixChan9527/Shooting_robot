#include "head_file.h"


extern u16 X,launch_pwm;
extern u8 dir,TRound;
extern int pwm_dianji1, pwm_dianji2, pwm_dianji3, pwm_dianji4;
extern char dirflag;
extern u16 Ball_X;

void car_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOB/C时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB/C时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	stop();								//初始化车方向脚停止
}


void stop(void)		//停止
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);//0
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//0  一号电机
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0    二号电机
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//0
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//0  三号电机
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0     
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0   四号电机
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//0 
	
	pwm_dianji1 = 0;
	pwm_dianji2 = 0;
	pwm_dianji3 = 0;
	pwm_dianji4 = 0;
}


void front(void)		//前平移
{
	GPIO_SetBits(GPIOF,GPIO_Pin_0);//1    1号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_3);//1  2号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_4);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0   3号电机正转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_7);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0  4号电机反转
}


void back(void)		//后平移
{
	GPIO_SetBits(GPIOF,GPIO_Pin_1);//1    1号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_2);//1  2号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_5);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//0   3号电机反转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_6);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//0  4号电机正转
}


void left(void)		//左平移
{
	GPIO_SetBits(GPIOF,GPIO_Pin_1);//1    1号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_2);//1  2号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_4);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0   3号电机反转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_7);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0  4号电机反转
	
	pwm_dianji1 = 160;
	pwm_dianji2 = 160;
	pwm_dianji3 = 160;
	pwm_dianji4 = 160;
}


void right(void)	//右平移
{
	GPIO_SetBits(GPIOF,GPIO_Pin_0);//1    1号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_3);//1  2号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_5);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//0   3号电机正转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_6);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//0  4号电机正转
	
	pwm_dianji1 = 160;
	pwm_dianji2 = 160;
	pwm_dianji3 = 160;
	pwm_dianji4 = 160;
}


void left_front(void)
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);//1    1号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//0

	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//1  2号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_4);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0   3号电机正转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_7);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0  4号电机反转
	
	pwm_dianji1 = 2000;
	pwm_dianji2 = 2000;
	pwm_dianji3 = 2000;
	pwm_dianji4 = 2000;
}


void right_front(void)
{
	GPIO_SetBits(GPIOF,GPIO_Pin_0);//1    1号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_3);//1  2号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0	
    
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0   3号电机正转
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0  4号电机反转
	
	pwm_dianji1 = 2000;
	pwm_dianji2 = 2000;
	pwm_dianji3 = 2000;
	pwm_dianji4 = 2000;
}


void turnright(void)//原地右
{
	GPIO_SetBits(GPIOF,GPIO_Pin_1);//1    1号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_3);//1  2号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_5);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//0   3号电机反转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_7);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0  4号电机反转
	
	if(TRound == 0)
	{
		pwm_dianji1 = 450;
		pwm_dianji2 = 450;
		pwm_dianji3 = 450;
		pwm_dianji4 = 450;
	}
	else 
	{
		pwm_dianji1 = 100;
		pwm_dianji2 = 100;
		pwm_dianji3 = 100;
		pwm_dianji4 = 100;		
	}
	
}


void crossright(void)//原地右
{
	GPIO_SetBits(GPIOF,GPIO_Pin_1);//1    1号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);//0

	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//1  2号电机反转
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);//0	
    
	GPIO_SetBits(GPIOF,GPIO_Pin_5);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//0   3号电机反转
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_6);//0  4号电机反转
	
		pwm_dianji1 = 1000;
		pwm_dianji2 = 1000;
		pwm_dianji3 = 1000;
		pwm_dianji4 = 1000;

}





void turnleft(void)//原地左
{
	GPIO_SetBits(GPIOF,GPIO_Pin_0);//1    1号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_2);//1  2号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//0	+

	GPIO_SetBits(GPIOF,GPIO_Pin_4);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0   3号电机正转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_6);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//0  4号电机正转
	
	if(TRound == 0)
	{
		pwm_dianji1 = 450;
		pwm_dianji2 = 450;
		pwm_dianji3 = 450;
		pwm_dianji4 = 450;
	}
	else 
	{
		pwm_dianji1 = 100;
		pwm_dianji2 = 100;
		pwm_dianji3 = 100;
		pwm_dianji4 = 100;		
	}
	
}


void crossleft(void)//原地左
{
	GPIO_ResetBits(GPIOF,GPIO_Pin_0);//1    1号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_1);//0

	GPIO_SetBits(GPIOF,GPIO_Pin_2);//1  2号电机正转
	GPIO_ResetBits(GPIOF,GPIO_Pin_3);//0	+

	GPIO_ResetBits(GPIOF,GPIO_Pin_4);//1
	GPIO_ResetBits(GPIOF,GPIO_Pin_5);//0   3号电机正转
	
	GPIO_SetBits(GPIOF,GPIO_Pin_6);//1 
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);//0  4号电机正转

		pwm_dianji1 = 1000;
		pwm_dianji2 = 1000;
		pwm_dianji3 = 1000;
		pwm_dianji4 = 1000;	
}


void car_no_adjust(void)	//无PID
{
	pwm_dianji1 = 160;
	pwm_dianji2 = 160;
	pwm_dianji3 = 160;
	pwm_dianji4 = 160;
}

void avoid_speed(void)	//无PID
{
	pwm_dianji1 = 2000;
	pwm_dianji2 = 2000;
	pwm_dianji3 = 2000;
	pwm_dianji4 = 2000;
}


void car_adjust(void)		//调整车走直线，PID算法
{
	u16 Target = Ball_X;		//Target是摄像头画面的中间垂线，用于对中
	int PWM, PWM1,PWM2;
	
	PWM = PID_Realize(X, Target);		
	
	if(dirflag == 0)
	{
		PWM1 = 450 + PWM;		//这里的10和35是按照车子走直线的时候往哪边偏，偏多少调的。
		PWM2 = 450 - PWM - 55;		//上4 下55
	}
		
	else if(dirflag == 1)
	{
		PWM1 = 160 - PWM;
		PWM2 = 160 + PWM + 55;
	}
	
	else if(dirflag == 6)
	{
		PWM1 = 120 + PWM;
		PWM2 = 120 - PWM + 55;
	}
	
	else if(dirflag == 2)
	{
		PWM1 = 1500 - PWM;		//这里的10和35是按照车子走直线的时候往哪边偏，偏多少调的。
		PWM2 = 1500 + PWM + 200;		//上4 下55
	}
		
	else if(dirflag == 3)
	{
		PWM1 = 200 + 15 * PWM;
		PWM2 = 200 - 20 * PWM - 55;
	}
	
	else if(dirflag == 4)
	{
		PWM1 = 300;		//这里的10和35是按照车子走直线的时候往哪边偏，偏多少调的。
		PWM2 = 300;		//上4 下55
	}
		
	else if(dirflag == 5)
	{
		PWM1 = 200;
		PWM2 = 200;
	}
//////////////////////////////////////////////////////////////////
//////////////////限定电机PWM/////////////////////////////////////
	if(PWM1 > 2000)
		PWM1 = 2000;
	else if(PWM1 <= 0)
		PWM1 = 0;
	if(PWM2 > 2000)
		PWM2 = 2000;
	else if(PWM2 <= 0)
		PWM2 = 0;
	
	pwm_dianji1 = PWM1;
	pwm_dianji3 = PWM1;
	pwm_dianji2 = PWM2;
	pwm_dianji4 = PWM2;
}


void Launch(int i)
{
	if(i <= 0)
		i = 0;
	else if(i >= 2000)
		i = 2000;
	else launch_pwm = i; 
}


