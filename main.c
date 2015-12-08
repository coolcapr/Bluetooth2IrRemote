#include "sys.h"
#include "ir.h"
#include "pwm.h"
#include "usart.h"

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = ON       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

#define CLEAR               0
#define SET                 1
#define BRG16_VAL           0 //Use BRG 8 bit mode
#define PRESCALER_VAL       1 //Use 1:1 prescaler
#define BAUDRATE_VAL        51 //Configure baudrate to 19200bps
#define BUFFER_SIZE         20 

void main(void)
{
    //Initialize system wide parameters
    sys_init();
       
    //Initialize UART mode: 16MHz Oscillator, Baud-rate 19200, Asynchronous Eight Bit Mode
    USART_init(BRG16_VAL, PRESCALER_VAL, BAUDRATE_VAL, USART_ASYNC_EIGHT_BIT_MODE);
    
    //Initialize IR Transmitter
    IR_init(IR_FREQ_30_0);    
    
    uint8_t IR_command_buffer[BUFFER_SIZE]; // command buffer, 20 byte long
    
    //Command receive loop
    while(1)
    {        
        if(USART_INDbits.USART_RX_IND == SET)
        {
            USART_receive_data(IR_command_buffer, BUFFER_SIZE);
            IR_send_command(IR_command_buffer);
        }
    }
}