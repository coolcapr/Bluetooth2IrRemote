#include "pwm.h"

#define ENABLE              0   
#define DISABLE             1
#define CLEAR               0
#define SET                 1
#define OFF                 0
#define ON                  1
#define MODE_PWM            0x0C


void PWM_init(uint8_t pr2_val, uint8_t prescaler_val, uint16_t ccpr2l_dc2b_val)
{
    APFCONbits.CCP2SEL = SET; //Configure CCP2 function to be on pin RB3
    TRISBbits.TRISB3 = DISABLE; //Disable PWM output.
    CCP2CON = MODE_PWM; //Set CCP2 mode to Single output PWM mode:
    PWM_set_period(pr2_val, prescaler_val);
    PWM_set_duty_cycle(ccpr2l_dc2b_val);
    PIR1bits.TMR2IF = CLEAR; //Clear timer 2 interrupt flag
    T2CONbits.TMR2ON = ON; //Enable timer 2
}

void PWM_set_period(uint8_t pr2_val, uint8_t prescaler_val)
{
    PR2 = pr2_val; //Set Timer2 period
    T2CON = prescaler_val; //Set Timer2 pre-scaler value
}

void PWM_set_duty_cycle(uint16_t ccpr2l_dc2b_val)
{
    CCPR2L = (ccpr2l_dc2b_val >> 2); //Duty cycle least low byte
    CCP2CONbits.DC2B1 = ((ccpr2l_dc2b_val & 0x02)>> 1); //Mask duty-cycle high byte bit0
    CCP2CONbits.DC2B0 = ((ccpr2l_dc2b_val & 0x01)); //Mask duty-cycle high byte bit1
}

void PWM_stop()
{
    TRISBbits.TRISB3 = DISABLE; //Disable PWM output.
}

void PWM_start()
{
    TMR2 = PR2; //Reset PWM by forcing a timer overflow event
    TRISBbits.TRISB3 = ENABLE;  //Enable PWM output.    
}