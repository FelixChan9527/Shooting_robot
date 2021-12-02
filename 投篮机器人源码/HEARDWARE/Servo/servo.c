#include "head_file.h"


//174放球张开
extern u16 ServoPitch, ServoYaw, ServoArm, ServoHand;
u8 CamYawFlag = 0;


void camera_up(void)
{
	ServoPitch = 191;		//当球在摄像头底端，则摄像头抬头；
}

void camera_more_up(void)
{
	ServoPitch = 176;		//当球在摄像头底端，则摄像头抬头；
}


void camera_pink_up(void)
{
	ServoPitch = 176;		//当球在摄像头底端，则摄像头抬头；
}


void camera_down(void)
{
	ServoPitch = 200;		//当球在摄像头顶端，则摄像头低头；
}


void camera_front(void)
{
	ServoYaw = 208;		//摄像头前看
}


void camera_back(void)
{
	ServoYaw = 87;		//摄像头后看	(还没调)
}


void camera_left(void)
{
	ServoYaw = 220;		//摄像头左看
}

void arm_up(void)//举起手臂
{
	ServoArm = 100;
}

void arm_down(void)//放下手臂
{
	ServoArm = 204;
}

void HandOpen(void)//打开机械爪
{
	ServoHand = 90;
}

void HandClose(void)//合上机械爪
{
	ServoHand = 53;//72
}

void HandPut(void)//打开机械爪
{
	ServoHand = 90;

}

