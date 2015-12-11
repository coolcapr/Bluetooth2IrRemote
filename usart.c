#include "usart.h"

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
    while(USART_TxBufferFull){;} //wait for TXREG to empty
    TXREG = _char;
    while(USART_TxBusy){;} //wait for TSR to empty
}

void USART_puts(const uint8_t *data_ptr, uint8_t length)
{
    uint8_t index;
    for(index = 0; index < length; index++)
    {
        while(USART_TxBufferFull){;} //wait for TXREG to empty
        TXREG = data_ptr[index];
    }
    
    while(USART_TxBusy){;} //wait for TSR to empty
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
        while(!USART_RxDataAvailable){};
        
        if(USART_FramingError || USART_OverunError) //If overflow error,reset UART module
        {
            USART_ContRxDisable(); 
            USART_ContRxEnable();
            
            return false;
        }
        
        buffer_ptr[index] = RCREG;
    }
    
    return true;
}