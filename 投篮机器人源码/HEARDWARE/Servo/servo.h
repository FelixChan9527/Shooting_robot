#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"

#define UP 1
#define DOWN 2
#define FRONT 3
#define BACK 4
#define LEFT 5

void camera_up(void);
void camera_down(void);
void camera_more_up(void);
void camera_front(void);
void camera_back(void);
void camera_left(void);
void arm_up(void);
void arm_down(void);
void HandOpen(void);
void HandClose(void);
void HandPut(void);
void camera_pink_up(void);

#endif
