/* 
 * File:   usart.h
 * Author: Sibusiso.Hlatshwayo
 *
 * Created on March 26, 2015, 9:38 PM
 */

#ifndef USART_H
#define	USART_H

#include "sys.h"

//BAUDCON VALUES
#define BAUDCON_DEFAULT                 0b00000000
#define CLK_IDLE_STATE_HIGH             0b00010000
#define BAUDRATE_GEN_16BIT              0b00001000
#define AUTO_BAUDRATE_DETECT_ENABLE     0b00000001

//RCSTA VALUES
#define RCSTA_DEFAULT                   0b00000000
#define SERIAL_PORT_ENABLE              0b10000000
#define RX_9BIT_MODE                    0b01000000
#define SINGLE_RX_ENABLE                0b00100000
#define CONT_RX_ENABLE                  0b00010000
#define ADDRESS_DETECT_ENABLE           0b00001000

//TXSTA VALUES
#define TXSTA_DEFAULT                   0b00000000
#define INTERNAL_CLK_GENERATION         0b10000000
#define TX_9BIT_MODE                    0b01000000
#define TX_ENABLE                       0b00100000
#define SYNCH_MODE_ENABLE               0b00010000
#define HIGH_BAUDRATE                   0b00000100


//Structure for USART TX and RX bits on PORTC RC6(TX) and RC7(RX) respectively
typedef union
{
    struct
    {
        unsigned    NOT_USED        :6; //RC0 - RC5
        unsigned    USART_TXD       :1; //RC6
        unsigned    USART_RXD       :1; //RC7
    };
}USARTbits_t;
volatile USARTbits_t USARTbits @ &PORTC;


//USART related functions
void USART_init(const uint8_t baudcon_val, const uint8_t rcsta_val, const uint8_t txsta_val, const uint16_t spbrg_val);
void USART_putch(const uint8_t _char);
void USART_puts(const uint8_t *data_ptr, uint8_t length);
uint8_t USART_getch();
bool USART_gets(uint8_t *buffer_ptr, uint8_t length);

#endif	/* USART_H */