/* 
 * File:   usart.h
 * Author: Sibusiso.Hlatshwayo
 *
 * Created on March 26, 2015, 9:38 PM
 */

#ifndef USART_H
#define	USART_H

#include "sys.h"

// USART Flags
#define USART_AutoBaudOverflow              (BAUDCONbits.ABDOVF)
#define USART_ReceiveIdle                   (BAUDCONbits.RCIDL)
#define USART_RxDataAvailable               (PIR1bits.RCIF)
#define USART_TxBufferFull                  (!PIR1bits.TXIF)
#define USART_FramingError                  (RCSTAbits.FERR)
#define USART_OverrunError                   (RCSTAbits.OERR)
#define USART_TxBusy                        (!TXSTAbits.TRMT)

//BAUDCON values
#define USART_BAUDCON_DEFAULT               0b00000000
#define USART_CLK_IDLE_STATE_HIGH           0b00010000
#define USART_BAUDRATE_GEN_16BIT            0b00001000
#define USART_AUTO_BAUDRATE_DETECT_ENABLE   0b00000001

//RCSTA values
#define USART_RCSTA_DEFAULT                 0b00000000
#define USART_SERIAL_PORT_ENABLE            0b10000000
#define USART_RX_9BIT_MODE                  0b01000000
#define USART_SINGLE_RX_ENABLE              0b00100000
#define USART_CONT_RX_ENABLE                0b00010000
#define USART_ADDRESS_DETECT_ENABLE         0b00001000

//TXSTA values
#define USART_TXSTA_DEFAULT                 0b00000000
#define USART_INTERNAL_CLK_GENERATION       0b10000000
#define USART_TX_9BIT_MODE                  0b01000000
#define USART_TX_ENABLE                     0b00100000
#define USART_SYNCH_MODE_ENABLE             0b00010000
#define USART_HIGH_BAUDRATE                 0b00000100

//USART_function defines
#define USART_WakeUpModeEnable()            (BAUDCONbits.WUE = 1)
#define USART_WakeUpModeDisable()           (BAUDCONbits.WUE = 0)
#define USART_SerialPortEnable()            (RCSTAbits.SPEN = 1)
#define USART_SerialPortDisable()           (RCSTAbits.SPEN = 0)
#define USART_ContRxEnable()                (RCSTAbits.CREN = 1)
#define USART_ContRxDisable()               (RCSTAbits.CREN = 0)
#define USART_SingleRxEnable()              (RCSTAbits.SREN = 1)
#define USART_SingleRxDisable()             (RCSTAbits.SREN = 0)
#define USART_TxEnable()                    (TXSTAbits.TXEN = 1)
#define USART_TxDisable()                   (TXSTAbits.TXEN = 0)
#define USART_SendBreakCharacter()          (TXSTAbits.SENDB = 1)

//USART related functions
void USART_Init(const uint8_t BaudconVal, const uint8_t RcstaVal, const uint8_t TxstaVal, const uint16_t SpbrgVal);
void USART_Putch(const uint8_t _Char);
void USART_Puts(const uint8_t *DataPtr, uint8_t Length);
uint8_t USART_Getch();
bool USART_Gets(uint8_t *BufferPtr, uint8_t Length);
void USART_Write(const uint8_t *DataPtr, const char Delim);
bool USART_Read(uint8_t *BufferPtr, const char Delim);
#endif	/* USART_H */