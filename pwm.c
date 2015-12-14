#include "pwm.h"

#define PWM_CCP2_PIN_RB3                1
#define PWM_CCP2_PIN_RC1                0
#define PWM_CCP2_OUTPUT_STOP            1
#define PWM_CCP2_OUTPUT_START           0
#define CCP_PWM_MODE_VAL                0x0C
#define CCP_PWM_MODE_MASK               0xF0
#define TMR_PRESCALER_VAL               0x00
#define TMR_PRESCALER_MASK              0x0C

void PWM_CCP2Init(uint16_t Ccpr2Val, uint8_t Pr2Val)
{
    APFCONbits.CCP2SEL = PWM_CCP2_PIN_RB3; //Configure CCP2 function to be on pin RB3
    TRISBbits.TRISB3 = PWM_CCP2_OUTPUT_STOP; //Disable PWM output.
    CCP2CON = (CCP2CON & CCP_PWM_MODE_MASK) | CCP_PWM_MODE_VAL; //Set CCP2 mode to Single output PWM mode:
    PWM_CCP2SetPeriod(Pr2Val);
    PWM_CCP2SetDutyCycle(Ccpr2Val);
    PIR1bits.TMR2IF = 0; //Clear timer 2 interrupt flag
    T2CONbits.TMR2ON = 1; //Enable timer 2
}

void PWM_CCP2SetPeriod(uint8_t Pr2Val)
{
        PR2 = Pr2Val; //Set Timer4 period
        T2CON = (T2CON & TMR_PRESCALER_MASK) | TMR_PRESCALER_VAL; //Set Timer2 pre-scaler value to 1:1  
}

void PWM_CCP2SetDutyCycle(uint16_t Ccpr2Val)
{
    CCPR2L = (Ccpr2Val >> 2); //Duty cycle low byte
    CCP2CONbits.DC2B1 = ((Ccpr2Val & 0x02)>> 1); //Mask duty-cycle high byte bit0
    CCP2CONbits.DC2B0 = ((Ccpr2Val & 0x01)); //Mask duty-cycle high byte bit1
}

void PWM_CCP2Stop()
{
    TRISBbits.TRISB3 = PWM_CCP2_OUTPUT_STOP; //Disable PWM output.
}

void PWM_CCP2Start()
{
    TMR2 = PR2; //Reset PWM by forcing a timer overflow event
    TRISBbits.TRISB3 = PWM_CCP2_OUTPUT_START;  //Enable PWM output.    
}