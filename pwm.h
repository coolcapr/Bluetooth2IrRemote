/* 
 * File:   pwm.h
 * Author: Sibusiso.Hlatshwayo
 *
 * Created on 14 April 2015, 7:41 PM
 */

#ifndef PWM_CCP2_H
#define	PWM_CCP2_H

/*
 *
 *
 * Description
 *  
 * 
 */

#include "sys.h"

void PWM_CCP2Init(uint16_t Ccpr2Val, uint8_t Pr2Val);
void PWM_CCP2SetPeriod(uint8_t Pr2Val);
void PWM_CCP2SetDutyCycle(uint16_t Ccpr2Val);
void PWM_CCP2Stop(); //stops PWM output
void PWM_CCP2Start(); // start PWM output

#endif	/* PWM_CCP2_H */