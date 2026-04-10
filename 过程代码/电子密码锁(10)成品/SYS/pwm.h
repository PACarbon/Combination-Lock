#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

void Pwm_Init(void);
void PWM_SetCompare3(uint16_t Compare);
void Servo_SetAngle(float Angle);

#endif
