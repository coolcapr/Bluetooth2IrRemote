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

#define BAUDCON_VAL                     (USART_BAUDCON_DEFAULT)
#define RCSTA_VAL                       (USART_SERIAL_PORT_ENABLE | USART_CONT_RX_ENABLE)
#define TXSTA_VAL                       (USART_TX_ENABLE | USART_HIGH_BAUDRATE)
#define SPBRG_VAL                       51 //baudrate: 19200bps
#define COMMAND_BUFFER_SIZE             20 

void main(void)
{
    //Initialize system wide parameters
    SYS_Init();
       
    //Initialize UART mode: 16MHz Oscillator, Baud-rate 19200, Asynchronous Eight Bit Mode
    USART_Init(BAUDCON_VAL, RCSTA_VAL, TXSTA_VAL, SPBRG_VAL);
    
    //Initialize IR Transmitter
    IR_Init(IR_DUTY_CYCLE_QUATER, IR_FREQ_38);    
    
    //uint8_t CommandBuffer[COMMAND_BUFFER_SIZE] // command buffer, 20 byte long
    //Apple TV
    uint8_t CommandBuffer1[COMMAND_BUFFER_SIZE] = {0x00,0x11,0x00,0x28,0x23,0x94,0x11,0x9A,0x06,0x30,0x02,0x30,0x02,0x30,0x02,0x20,0xEE,0x87,0x02,0x39};
    
    //Samsung
    uint8_t CommandBuffer2[COMMAND_BUFFER_SIZE] = {0x00,0x11,0x00,0x94,0x11,0x94,0x11,0x9A,0x06,0x30,0x02,0x30,0x02,0x30,0x02,0x20,0x07,0x07,0x8B,0x74};
    //Command receive loop
    while(1)
    {        
        //if(USART_INDbits.USART_RX_IND == SET)
        //{
        //    USART_receive_data(CommandBuffer, BUFFER_SIZE);
            IR_SendCommand(CommandBuffer1, IR_DUTY_CYCLE_QUATER);
            __delay_ms(2500);
            IR_SendCommand(CommandBuffer2, IR_DUTY_CYCLE_QUATER);
            __delay_ms(2500);
        //}
    }
}