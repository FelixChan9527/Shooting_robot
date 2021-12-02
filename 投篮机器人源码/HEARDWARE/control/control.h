#ifndef __CONTROL_H
#define __CONTROL_H	
#include "sys.h" 

void SetCore(void);
void MoveTargetBlue(u16 target);
void MoveTargetGreen(u16 target);
void BallCatched(void);
void TranslateHorizontal(void);
void TranslateVertical(void);
void PickBall(void);
void MoveToGreen(void);
void OnGreenCore(void);
void SeekAndLunch(void);
void Avoidance(void);
void SetBackCore(void);
void SetPinkCore(void);
void MoveToFree(void);
void MoveTargetFree(u16 target);

#endif 
