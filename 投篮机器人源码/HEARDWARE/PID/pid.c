#include "head_file.h"

unsigned char ErrFlag = 0;

typedef struct PID
{
	float P ,I ,D ,B;	//比例、积分、微分系数、偏置值
}PID;

typedef struct Error
{
	float Current_Error;  //当前误差
	float Last_Error;		//上一次误差
	float Previous_Error;	//历史误差
	   
}Error;

//struct PID pid = {2, 0, 0, 0.2};		//初始化PID		//需要修改
struct PID pid = {10, 0, 0.8, 0};		//初始化PID		//需要修改
struct Error error = {0, 0, 0};		//初始化偏差值


//#############################################################################
//####################位置式PID算法############################################
//#############################################################################


float PID_Realize(u16 NowPlace,u16 Point)	//输入球的当前位置、球的目标位置，输出车的PID值
{
	int PidPwm;
	float PWM_P,PWM_I,PWM_D, PWM_B;
	
	if(ErrFlag == 1)
	{
		error.Current_Error = 0;
		error.Last_Error = 0;
		error.Previous_Error = 0;
		ErrFlag = 0;
	}
		
	error.Current_Error = Point - NowPlace;		//当前误差
	error.Previous_Error = error.Current_Error + error.Previous_Error;		//历史误差
	
	PWM_P = pid.P * error.Current_Error;
	PWM_I = pid.I * error.Previous_Error;
	PWM_D = pid.D * (error.Last_Error - error.Current_Error);
	PWM_B = pid.B;
	
	error.Last_Error = error.Current_Error;	//上一次误差
	
	PidPwm = PWM_P + PWM_I + PWM_D + PWM_B;
	
	error.Last_Error = error.Current_Error;
	error.Previous_Error  = error.Previous_Error;
	
	return PidPwm;
}
	


