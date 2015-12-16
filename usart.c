#include "usart.h"

#define TimeoutOccured              (PIR1bits.TMR1IF)
#define RECEIVE_TIMEOUT             10000 //10 milliseconds

void USART_Init(const uint8_t BaudconVal, const uint8_t RcstaVal, const uint8_t TxstaVal, const uint16_t SpbrgVal)
{
    BAUDCON = BaudconVal;
    SPBRGL = (uint8_t)(0x00FF & SpbrgVal);
    SPBRGH = (uint8_t)(SpbrgVal >> 8);
    
    RCSTA = RcstaVal;
    TXSTA = TxstaVal;   
    
    //uhjkjls
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

void USART_Putch(const uint8_t _Char)
{
    while(USART_TxBufferFull){;} //wait for TXREG to empty
    TXREG = _Char;
    while(USART_TxBusy){;} //wait for TSR to empty
}

void USART_Puts(const uint8_t *DataPtr, uint8_t Length)
{
    uint8_t Index;
    for(Index = 0; Index < Length; Index++)
    {
        while(USART_TxBufferFull){;} //wait for TXREG to empty
        TXREG = DataPtr[Index];
    }
    
    while(USART_TxBusy){;} //wait for TSR to empty
}

uint8_t USART_Getch()
{
    return RCREG;
}

bool USART_Gets(uint8_t *BufferPtr, uint8_t Length)
{
    uint8_t Index;
    for(Index = 0; Index < Length; Index++)
    {
        SetTimeout(RECEIVE_TIMEOUT);
        while(!USART_RxDataAvailable)
        {
            if(TimeoutOccured)
            {
                RemoveTimeout();
                return false;
            }
        }
        RemoveTimeout();  
        
        if(USART_FramingError || USART_OverrunError) //if Overrun error or Framing error,reset UART module
        {
            USART_ContRxDisable(); 
            USART_ContRxEnable();
            
            return false;
        }
        
        BufferPtr[Index] = RCREG;
    }
    
    return true;
}

void USART_Write(const uint8_t *DataPtr, const char Delim)
{
    while(((char)*DataPtr) != Delim)
    {
        while(USART_TxBufferFull){;} //wait for TXREG to empty
        TXREG = *DataPtr++;
    }
}

bool USART_Read(uint8_t *BufferPtr, const char Delim)
{
    do
    {
        SetTimeout(RECEIVE_TIMEOUT);
        while(!USART_RxDataAvailable)
        {
            if(TimeoutOccured)
            {
                RemoveTimeout();
                return false;
            }
        }   
        RemoveTimeout(); 
        
        
        if(USART_FramingError || USART_OverrunError) //If overflow error,reset UART module
        {
            USART_ContRxDisable(); 
            USART_ContRxEnable();
            
            return false;
        }
        
        *BufferPtr = RCREG;
    }while(Delim != ((char)*BufferPtr++));
    
    return true;
}