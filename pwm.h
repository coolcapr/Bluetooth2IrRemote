/* 
 * File:   pwm.h
 * Author: Sibusiso.Hlatshwayo
 *
 * Created on 14 April 2015, 7:41 PM
 */

#ifndef PWM_H
#define	PWM_H

/*
 *
 *
 * Description
 *  
 * 
 */

#include "sys.h"

void PWM_Init(uint8_t pr2_val, uint8_t prescaler_val, uint16_t ccpr2l_dc2b_val);
void PWM_SetPeriod(uint8_t pr2_val, uint8_t prescaler_val);
void PWM_SetDutyCycle(uint16_t ccprxl_dcxb_val);
void PWM_Stop(); //stops PWM output
void PWM_Start(); // start PWM output

#endif	/* PWM_H */