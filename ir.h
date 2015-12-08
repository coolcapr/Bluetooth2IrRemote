/* 
 * File:   ir.h
 * Author: Sibusiso.Hlatshwayo
 *
 * Created on April 16, 2015, 7:23 PM
 */

#ifndef IR_H
#define	IR_H

#include "sys.h"

#define    IR_FREQ_30_0     1
#define    IR_FREQ_30_5     2
#define    IR_FREQ_31_0     3
#define    IR_FREQ_31_5     4
#define    IR_FREQ_32_0     5
#define    IR_FREQ_32_5     6
#define    IR_FREQ_33_0     7
#define    IR_FREQ_33_5     8
#define    IR_FREQ_34_0     9
#define    IR_FREQ_34_5     10
#define    IR_FREQ_35_0     11
#define    IR_FREQ_35_5     12
#define    IR_FREQ_36_0     13
#define    IR_FREQ_36_5     14
#define    IR_FREQ_37_0     15
#define    IR_FREQ_37_5     16
#define    IR_FREQ_38_0     17
#define    IR_FREQ_38_5     18
#define    IR_FREQ_39_0     19
#define    IR_FREQ_39_5     20
#define    IR_FREQ_40_0     21
#define    IR_FREQ_40_5     22
#define    IR_FREQ_41_0     23
#define    IR_FREQ_41_5     24
#define    IR_FREQ_42_0     25
#define    IR_FREQ_42_5     26

void IR_init(uint8_t IR_freq);
void IR_set_carrier(uint8_t IR_freq);
void IR_send_command(uint8_t *command);

#endif	/* IR_H */
