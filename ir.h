/* 
 * File:   ir.h
 * Author: Sibusiso.Hlatshwayo
 *
 * Created on April 16, 2015, 7:23 PM
 */

#ifndef IR_H
#define	IR_H

#include "sys.h"

//Frequency parameters
#define    IR_FREQ_30               132
#define    IR_FREQ_31               128
#define    IR_FREQ_32               124
#define    IR_FREQ_33               120
#define    IR_FREQ_34               116
#define    IR_FREQ_35               113
#define    IR_FREQ_36               110
#define    IR_FREQ_37               107
#define    IR_FREQ_38               104
#define    IR_FREQ_39               101
#define    IR_FREQ_40               99
#define    IR_FREQ_41               96
#define    IR_FREQ_42               94

//DutyCycle parameters
#define IR_DUTY_CYCLE_QUATER        0
#define IR_DUTY_CYCLE_HALF          1

void IR_Init(uint8_t DutyCycle, uint8_t Freq);
void IR_SendCommand(uint8_t *CommandFramePtr, uint8_t DutyCycle);

#endif	/* IR_H */
