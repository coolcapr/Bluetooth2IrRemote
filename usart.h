/* 
 * File:   usart.h
 * Author: Sibusiso.Hlatshwayo
 *
 * Created on March 26, 2015, 9:38 PM
 */

#ifndef USART_H
#define	USART_H

#include "sys.h"

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

//Enumeration defining constants for USART mode
enum
{
    USART_ASYNC_EIGHT_BIT_MODE,
    USART_ASYNC_NINE_BIT_MODE,
    USART_SYNC_EIGHT_BIT_MASTER_MODE,
    USART_SYNC_EIGHT_BIT_SLAVE_MODE,
    USART_SYNC_NINE_BIT_MASTER_MODE,
    USART_SYNC_NINE_BIT_SLAVE_MODE
}usart_mode;

//USART related functions
void USART_init(uint8_t brg16_val, uint8_t brgh_val, uint8_t spbrg_val, uint8_t usart_mode);
void USART_putc(const uint8_t _char);
void USART_puts(const uint8_t *data_ptr, uint8_t length);
uint8_t USART_getc();
bool USART_gets(uint8_t *buffer_ptr, uint8_t size);

#endif	/* USART_H */