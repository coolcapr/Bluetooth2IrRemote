#include "usart.h"

#define TxBusy              (!TXSTAbits.TRMT)
#define TxBufferFull        (!PIR1bits.TXIF)
#define RxDataAvailable     (PIR1bits.RCIF)
#define OverunError         (RCSTAbits.OERR)
#define FramingError        (RCSTAbits.FERR)
#define ContRxEnable        (RCSTAbits.SPEN)

void USART_init(const uint8_t baudcon_val, const uint8_t rcsta_val, const uint8_t txsta_val, const uint16_t spbrg_val)
{
    BAUDCON = baudcon_val;
    SPBRGL = (uint8_t)(0x00FF & spbrg_val);
    SPBRGH = (uint8_t)(spbrg_val >> 8);
    
    RCSTA = rcsta_val;
    TXSTA = txsta_val;   
}

void USART_putch(const uint8_t _char)
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

uint8_t USART_getch()
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