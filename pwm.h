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

void PWM_init(uint8_t pr2_val, uint8_t prescaler_val, uint16_t ccpr2l_dc2b_val);
void PWM_set_period(uint8_t pr2_val, uint8_t prescaler_val);
void PWM_set_duty_cycle(uint16_t ccprxl_dcxb_val);
void PWM_stop(); //stops PWM output
void PWM_start(); // start PWM output

#endif	/* PWM_H */