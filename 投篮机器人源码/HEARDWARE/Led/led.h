#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define led_r PDout(0)
#define led_g PDout(1)
#define led_b PDout(2)
#define led_2 PDout(3)
#define led_3 PDout(4)

void LedInit(void);

#endif
