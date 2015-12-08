#include "usart.h"

#define TxBusy              (!TXSTAbits.TRMT)
#define TxBufferFull        (!PIR1bits.TXIF)
#define RxDataAvailable     (PIR1bits.RCIF)
#define OverunError         (RCSTAbits.OERR)
#define FramingError        (RCSTAbits.FERR)
#define ContRxEnable        (RCSTAbits.SPEN)

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
    
    CREN = ON; //Switch UART receiver ON
    TXEN = ON; //Switch UART Transmitter ON
}

void USART_putc(const uint8_t _char)
{
    while(TxBufferFull){;} //wait for TXREG to empty
    TXREG = _char;
    while(TxBusy){;} //wait for TSR to empty
}

void USART_puts(const uint8_t *data_ptr, uint8_t length)
{
    uint8_t index;
    for(index = 0; index < length; index++)
    {
        while(TxBufferFull){;} //wait for TXREG to empty
        TXREG = data_ptr[index];
    }
    
    while(TxBusy){;} //wait for TSR to empty
}

uint8_t USART_getc()
{
    return RCREG;
}

bool USART_gets(uint8_t *buffer_ptr, uint8_t length)
{
    uint8_t index;
    for(index = 0; index < length; index++)
    {
        while(!RxDataAvailable){};
        
        if(FramingError || OverunError) //If overflow error,reset UART module
        {
            ContRxEnable = false; 
            ContRxEnable = true;
            return false;
        }
        
        buffer_ptr[index] = RCREG;
    }
    
    return true;
}