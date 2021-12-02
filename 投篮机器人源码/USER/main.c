#include "head_file.h"

/*
需要留意的是，当画面中没有目标时，将会赋值X,Y等于10000，这个这么大的值主要用于区分是否是正确值
也方便使其找目标时，若没有目标，则一直转，直到摄像头找到目标为止

需要修改的地方：

1.各个位置的舵机角度，因为安装的位置不太一样，所以需要各组自己调
2.车前走后走的位置(即目标应该在画面中何处的位置) ：X、Y
3.对中时候的X值，画面中的中间值为X = 160，但是车转的惯性还有行走的惯性远因，达不到该位置
  个人不建议修改这里，建议加入二次对中
4.PID的参数(pid.c)
5.小车原地旋转的速度(在car.c)

红外暂时还没加入，各队可加进去

*/

extern int X, Y, H1, H2, H3, H4;
extern u8 TRound;
extern u16 Step,BallColor,launch_pwm,ServoArm,ServoPitch;
unsigned int PickFlag = 0;
extern char hand_flag;
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级
	MY_USART2_Init();
	car_Init();
	uart_init(115200);
	TIM3_Int_Init(10-1, 84-1);
	TIM4_Int_Init(10-1, 84-1);
	delay_init(168);
	LedInit();
	printf("ok\r\n");
	arm_up();
	HandOpen();
	Repead(star_send);	//启动识别位置数据接收，因为接收数据对控制影响不大，可让其一直发送	
	camera_front();		//摄像头前看
	camera_up();		//当球在摄像头顶端，则摄像头低头
	Launch(launch_pwm);
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 1)
	{
		BallColor = blue;
		led_r = 1;
		led_g = 1;
		led_b = 0;
	}
	else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0) 
	{
		BallColor = red;
		led_r = 0;
		led_g = 1;
		led_b = 1;
	}
	delay_ms(3000);
	right_front();
	delay_ms(1500);
	stop();
	Step = 1;
/**************************************************************************************************
**************************************************************************************************/
	/////////////////////////////////////////////////////////////////////////////////	
	while(1)
	{
////		printf("X = %d Y = %d\r\n",X, Y);
/////********************************************************************************************************
////将所有步骤编号，加入Step1,3,5,7是为了当红外碰到障碍物后可以重头执行本次步骤，而不是从上一步重新开始
////*************************************************************************************************************/	
		switch(Step)			
		{
			case 1: Step++;break;
			case 2: PickBall();break;		//识别球并捡球
			case 3: Step++;break;
			case 4: MoveToGreen();break;	//识别绿色边缘
			case 5: Step++;break; 
			case 6: OnGreenCore();break;	//识别绿色核心
			case 7: Step++;break;
			case 8: SeekAndLunch();break;	//寻找篮筐并投球
		}		
	}
}
