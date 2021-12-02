#include "head_file.h"

int count=1000;
extern int X,Y; 
extern u16 launch_pwm, ServoArm, ServoPitch;
extern u8 CamYawFlag,CheckFlag;									//摄像头舵机航向调整标志位
extern u8 KeyFlag4,KeyFlag5,KeyFlag6,KeyFlag7;
extern int pwm_dianji1, pwm_dianji2, pwm_dianji3, pwm_dianji4;
u8 AdvoidFlag = 0, BallFlag = 0;										//红外避障的标志位
u8 FreeFlag = 0;
u16 GoFlag = 0,Step = 1,BallColor = blue, InfraredFlag = 0, Ball_X=159;				//步骤标志位，前进后退标志位
u8 TRound = 0;											//自转标志位，用于二次校验的时候，速度调节
extern unsigned char ErrFlag;
u16 dirflag = 0;
/*************************************************************************************
*************************************************************************************/
void Avoidance(void)	//红外避障算法
{
	if((KeyFlag4 == 0)&&(KeyFlag5 == 1))
	{
		AdvoidFlag = 1;
		TRound = 0;
		Step--;
		back();
		avoid_speed();
		delay_ms(500);
		crossleft();
		delay_ms(1000);
		stop();
	}
	
	else if((KeyFlag4 == 1)&&(KeyFlag5 == 0))
	{
		TRound = 0;
		AdvoidFlag = 1;
		Step--;
		back();
		avoid_speed();
		delay_ms(500);
		crossright();
		delay_ms(1000);
		stop();
	}
	
	else if((KeyFlag4 == 0)&&(KeyFlag5 == 0))
	{
		TRound = 0;
		AdvoidFlag = 1;
		Step--;
		back();
		avoid_speed();
		delay_ms(500);
		crossright();
		delay_ms(1000);
		stop();
	}
	
	else GoFlag = 0;
}


void MoveToFree(void)
{
	camera_more_up();
	delay_ms(150);
	Repead(green);		//切换阈值至绿色
	Repead(center);		//识别圆心
	SetBackCore();			//对中
	MoveTargetFree(229);	//跑向投球区
	Step = 1;
}


void SetCore(void)								//对中算法
{
	int i=0,j=0;
	AdvoidFlag = 0;
	BallFlag = 0;
	if(X > Ball_X)
	{
		while((X > Ball_X)&&(BallFlag == 0))
		{
			j ++;
			TRound = 0;
			turnright();							//右转
			if(j == count)
			{
				i++;
				j=0;
			}
			if(i == 1000)
			{
				MoveToFree();
				BallFlag = 1;
				Step = 1;
			}
		}
		stop();
		delay_ms(150);
	
		while((X < Ball_X)&&(BallFlag == 0))
		{
			TRound = 1;
			turnleft();								//左转
		}
		stop();
	}
	
	else if(X < Ball_X)
	{
		while((X < Ball_X)&&(BallFlag == 0))
		{
			j++;
			TRound = 0;
			turnleft();								//左转
			if(j == count)
			{
				i++;
				j=0;
			}
			if(i == 1000)
			{
				MoveToFree();
				BallFlag = 1;
				Step = 1;
			}
		}
		stop();
		delay_ms(100);
	
		while((X > Ball_X)&&(BallFlag == 0))
		{
			TRound = 1;
			turnright();							//右转
		}
		stop();
		delay_ms(80);
	}
	
	if(BallFlag == 1)
		BallFlag = 0;
}


void SetBackCore(void)								//对中算法
{
	FreeFlag = 1 - FreeFlag;
	
	if(FreeFlag == 0)
	{
		while((X > 170) || (X < 130))
		{
			TRound = 0;
			turnright();							//右转
		}
		stop();
		delay_ms(150);
		
	}
	
	else if(FreeFlag == 1)
	{
		while((X > 170) || (X < 130))
		{
			TRound = 0;
			turnleft();							//右转
		}
		stop();
		delay_ms(150);
	}
}



void SetPinkCore(void)								//对中算法
{
	AdvoidFlag = 0;
	if(X > 130)
	{
		while(X > 130)
		{
			TRound = 0;
			turnright();							//右转
		}
		stop();
		delay_ms(150);
	
		while(X < 130)
		{
			TRound = 1;
			turnleft();								//左转
		}
		stop();
	}
	
	else if(X < 130)
	{
		while(X < 130)
		{
			TRound = 0;
			turnleft();								//左转
		}
		stop();
		delay_ms(100);
	
		while(X > 130)
		{
			TRound = 1;
			turnright();							//右转
		}
		stop();
		delay_ms(80);
	}
}


void MoveTargetBlue(u16 target)						//移动到蓝色目标位置i
{
	
	AdvoidFlag = 0;
	while((Y <= target)&&(AdvoidFlag ==0)&&(Y != 10000)&&(Step == 2))							//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 0;
		Avoidance();
		front();
		car_adjust();
	}
	if((Y == 10000) || (X == 10000))
		Step = 1;
	stop();
	delay_ms(200);
	while((Y > target)&&(AdvoidFlag ==0)&&(Y != 10000)&&(Step == 2))				//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 1;
		Avoidance();
		back();
		car_adjust();
	}
	if((Y == 10000) || (X == 10000))
		Step = 1;
	stop();
	delay_ms(100);
	while((Y <= target)&&(AdvoidFlag ==0)&&(Y != 10000)&&(Step == 2))							//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 6;
		Avoidance();
		front();
		car_adjust();
	}
	if((Y == 10000) || (X == 10000))
		Step = 1;
	stop();
	delay_ms(100);
}


void MoveTargetFree(u16 target)						//移动到绿色目标位置i
{
	AdvoidFlag = 0;
	while((Y <= target)&&(AdvoidFlag ==0))			//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 2;
		Avoidance();
		front();
		car_adjust();
	}
	stop();
	delay_ms(200);
}


void MoveTargetGreen(u16 target)					//移动到绿色目标位置i
{
	AdvoidFlag = 0;
	while(Y <= target)								//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 2;
		back();
		car_adjust();
	}
	stop();
	delay_ms(200);
}


void Check(void)				//检验程序：半举机械手，若球在范围内，则抓到球，若不在，则没抓到球，重新抓球
{
	TRound = 0;					//不用管，我这边代码的标志位
	Repead(tb_ds);				//关闭跟踪
	arm_up();
	delay_ms(200);
	ServoPitch = 220;
	delay_ms(1000);
	if((X < 185) && (X > 125) && (Y < 149) && (Y > 89))		//校对球是否在目标位置
		Step = 3;				//正确则执行下一步
	else 
	{
		right_front();
		delay_ms(500);
		TRound = 0;
		turnright();
		delay_ms(300);
		stop();
		ServoArm = 180;
		delay_ms(300);
		HandOpen();
		delay_ms(200);
		arm_up();
		Step = 1;				//否则重新抓球
	}
	Repead(tb_en);				//开启跟踪
}


void BallCatched(void)			//执行抓球程序
{
	arm_down();
	delay_ms(900);
	HandClose();
	delay_ms(130);
	Check();
	arm_up();
}


void TranslateHorizontal(void)		//平移至目标位置，用于平移对中
{
	if(X < 150)						//若一开始球在机器人右端
	{
		while(X < 150)				//若不到目标位置则右平移
		{
			right();
		}
		stop();
		delay_ms(100);
		while(X > 150)				//再次平移对中，防止对过头
		{
			left();
		}
		stop();
		delay_ms(80);
	}
	
	else if(X > 150)				//若一开始球在机器人左端
	{
		while(X > 150)				//若不到目标位置则左平移
		{
			left();
		}
		stop();
		delay_ms(100);
		while(X < 150)				//再次平移对中，防止对过头
		{
			right();
		}
		stop();
		delay_ms(80);
	}
}


void TranslateVertical(void)
{
	AdvoidFlag = 0;
	while(Y <= 160)				//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 4;
		back();
		car_adjust();
	}
	stop();
	delay_ms(150);
	while(Y > 160)				//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 5;
		front();
		car_adjust();
	}
	stop();
	delay_ms(150);
	while(Y <= 160)				//若不到目标位置(Y = i)，则车一直往前走，同时车PID纠正
	{
		dirflag = 5;
		back();
		car_adjust();
	}
	stop();
	delay_ms(100);
}

/********************************************************************************
****************以下为机器人的运动步骤*******************************************
*********************************************************************************/


/*
摄像头切换至识别蓝色中心，对中后移动至球前面进行抓球
*/
void PickBall(void)
{
	HandOpen();
	camera_front();		//摄像头前看
	camera_up();		//摄像头仰起
	delay_ms(150);
	Repead(BallColor);	//切换阈值至蓝色
	delay_ms(30);
	Repead(center);		//识别圆心	
	SetCore();			//对中	
	if(Step == 2)
	{
		MoveTargetBlue(141);	//移动到250的位置
		ErrFlag = 1;
	}
	if(Step == 2)
	{
		BallCatched();		//开启抓球
	}
}


/*
寻找绿色投球区，并移动至接近投球区的地方
*/
void MoveToGreen(void)
{
	camera_back();		//摄像头后看
	camera_up();		//摄像头仰起
	delay_ms(500);
	Repead(green);		//切换阈值至绿色
	Repead(center);		//识别圆心
	SetBackCore();			//对中
	if(Step == 4)
		MoveTargetGreen(229);	//跑向投球区
	Step = 5;
}


/*
切换至识别绿色边缘，先摄像头向前低头，识别到投球区顶端后，先平移对中，后向前移动，
使其停在绿色投球点的中心
*/
void OnGreenCore(void)
{			
	camera_down();		//摄像头低头；
	delay_ms(100);		//必要的延时，非常重要	
	Repead(edge);		//识别边缘
	TranslateVertical();	//前进至目标位置
	TranslateHorizontal();	//平移对
	Step = 7;
}


void SeekAndLunch(void)		//寻找篮筐并投球
{
	camera_back();		//摄像头后看
	camera_pink_up();		//摄像头抬头
	delay_ms(100);
	Repead(pink);		//切换阈值至橙色
	delay_ms(30);
	Repead(center);		//识别圆心	
	SetPinkCore();			//对中	
	HandPut();			//放开爪子，球落下
	delay_ms(1000);
	Step = 1;
}















