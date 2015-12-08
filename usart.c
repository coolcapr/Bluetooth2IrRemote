#include "usart.h"

#define OFF             0
#define ON              1
#define CLEAR           0
#define SET             1
#define ENABLE_UART     0xC0

void USART_init(uint8_t brg16_val, uint8_t brgh_val, uint8_t spbrg_val, uint8_t usart_mode)
{
    BAUDCONbits.BRG16 = brg16_val;
    BRGH = brgh_val; //Baud-rate generator high speed mode
    SPBRGL = spbrg_val; //Baud-rate generator period
    
    // Set USART mode
    if (usart_mode == USART_ASYNC_EIGHT_BIT_MODE)
    {
        SYNC = CLEAR;
        SPEN = SET;
        TX9 = CLEAR;
        RX9 = CLEAR;
        ADDEN = CLEAR;
    }else if (usart_mode == USART_ASYNC_NINE_BIT_MODE)
    {
        SYNC = CLEAR;
        SPEN = SET;
        TX9 = SET;
        RX9 = SET;
    }else if (usart_mode == USART_SYNC_EIGHT_BIT_MASTER_MODE)
    {
        SYNC = SET;
        SPEN = SET;
        TX9 = CLEAR;
        RX9 = CLEAR;
    }else if (usart_mode == USART_SYNC_EIGHT_BIT_SLAVE_MODE)
    {
        SYNC = SET;
        SPEN = SET;
        TX9 = CLEAR;
        RX9 = CLEAR;
    }else if (usart_mode == USART_SYNC_NINE_BIT_MASTER_MODE)
    {
        SYNC = SET;
        SPEN = SET;
        TX9 = SET;
        RX9 = SET;
    } else if (usart_mode == USART_SYNC_NINE_BIT_SLAVE_MODE)
    {
        SYNC = SET;
        SPEN = SET;
        TX9 = SET;
        RX9 = SET;
    }
    
    TRISC = ENABLE_UART; //Set bits TRISC6 and TRISC7 for UART to work as per spec
    
    ANSELAbits.ANSA0 = CLEAR; //Set RA0(TX_ind) as digital pin
    ANSELAbits.ANSA1 = CLEAR; //Set RA1(RX_ind) as digital pin
    TRISAbits.TRISA0 = CLEAR; //Set RX_IND as output
    TRISAbits.TRISA1 = SET; //Set RX_IND as input 
    
    CREN = ON; //Switch UART receiver ON
    TXEN = ON; //Switch UART Transmitter ON
}

void USART_send_data(uint8_t *data_to_send_ptr, uint8_t length)
{   
    USART_INDbits.USART_TX_IND = ON;
    __delay_us(20); //Wait for receiver to be ready
    
    for(uint8_t tx_byte = 0; tx_byte < length; tx_byte++)
    {
        while(TXIF == CLEAR){;} //wait for TXREG to empty
        TXREG = data_to_send_ptr[tx_byte];
    }

    while(TXIF == CLEAR){;} //wait for TXREG to empty
    while(TRMT == CLEAR){;} //wait for TSR to empty
    
    USART_INDbits.USART_TX_IND = OFF;
}

void USART_receive_data(uint8_t *receive_buffer_ptr, uint8_t buffer_size)
{
    while(USART_INDbits.USART_RX_IND == ON)
    {
        if(PIR1bits.RCIF == SET)
        {          
            *receive_buffer_ptr = RCREG;
            receive_buffer_ptr++;
            buffer_size--;
            
            if(OERR == SET) //If overflow error,reset UART module
            {
                CREN = CLEAR;
                CREN = SET;
            }

            if (buffer_size <= 0){ //received data greater than buffer, break out of receive loop
                break;
            }
        }
    }
}