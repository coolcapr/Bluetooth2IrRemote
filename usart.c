#include "usart.h"

#define TimeoutOccured              (PIR1bits.TMR1IF)
#define RECEIVE_TIMEOUT             10000 //10 milliseconds

void USART_Init(const uint8_t baudcon_val, const uint8_t rcsta_val, const uint8_t txsta_val, const uint16_t spbrg_val)
{
    BAUDCON = baudcon_val;
    SPBRGL = (uint8_t)(0x00FF & spbrg_val);
    SPBRGH = (uint8_t)(spbrg_val >> 8);
    
    RCSTA = rcsta_val;
    TXSTA = txsta_val;   
}

void SetTimeout(uint16_t Period)
{
    T1CONbits.TMR1ON = 0x01; //Switch On Timer1 
    
    Period = Period << 2; // multiply period by 4 since one instruction is 1/4 micro second @ 16MHz
    CCPR1H = Period >> 8; //Mask high byte
    CCPR1L = Period & 0x00FF; // Mask low byte
    TMR1 = 0; //Reset timer 1
    PIR1bits.TMR1IF = 0; //Clear timer1 interrupt flag
}

void RemoveTimeout()
{
    T1CONbits.TMR1ON = 0x00; //Switch Off Timer1
}

void USART_Putch(const uint8_t _char)
{
    while(USART_TxBufferFull){;} //wait for TXREG to empty
    TXREG = _char;
    while(USART_TxBusy){;} //wait for TSR to empty
}

void USART_Puts(const uint8_t *data_ptr, uint8_t length)
{
    uint8_t index;
    for(index = 0; index < length; index++)
    {
        while(USART_TxBufferFull){;} //wait for TXREG to empty
        TXREG = data_ptr[index];
    }
    
    while(USART_TxBusy){;} //wait for TSR to empty
}

uint8_t USART_Getch()
{
    return RCREG;
}

bool USART_Gets(uint8_t *buffer_ptr, uint8_t length)
{
    uint8_t index;
    for(index = 0; index < length; index++)
    {
        SetTimeout(RECEIVE_TIMEOUT);
        while(!USART_RxDataAvailable)
        {
            if(TimeoutOccured)
            {
                return false;
            }
        }
        RemoveTimeout();  
        
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

void USART_Write(const uint8_t *data_ptr, const char delim)
{
    while(((char)*data_ptr) != delim)
    {
        while(USART_TxBufferFull){;} //wait for TXREG to empty
        TXREG = *data_ptr++;
    }
}

bool USART_Read(uint8_t *buffer_ptr, const char delim)
{
    do
    {
        SetTimeout(RECEIVE_TIMEOUT);
        while(!USART_RxDataAvailable)
        {
            if(TimeoutOccured)
            {
                return false;
            }
        }   
        RemoveTimeout(); 
        
        
        if(USART_FramingError || USART_OverunError) //If overflow error,reset UART module
        {
            USART_ContRxDisable(); 
            USART_ContRxEnable();
            
            return false;
        }
        
        *buffer_ptr = RCREG;
    }while(delim != ((char)*buffer_ptr++));
    
    return true;
}