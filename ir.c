#include "ir.h"
#include "pwm.h"

#define PULSE_POSITION_ENCODING     0x00
#define PULSE_LENGTH_ENCODING       0x01
#define BI_PHASE_ENCODING           0x02
#define ENCODING_TYPE_MASK          0x03

void IR_EncodePulsePosition();
void IR_EncodePulseLength();
void IR_EncodeBiPhase();

//IR frame parameters
uint8_t     Carrier;
uint8_t     Flags;
uint16_t    PreambleOnPeriod;
uint16_t    PreambleOffPeriod;
uint16_t    MarkPeriod;
uint16_t    SpacePeriod;
uint16_t    GapPeriod;
uint16_t    PostamblePeriod;
uint8_t     CommandBitLength; // command length in bits
uint8_t     CommandByteLength; // command length in bytes   
uint8_t     *CommandPtr;

//Custom delay method necessary as MPLab-X built-in delay function requires constant parameters/arguments
void IR_Delay(uint16_t Period)
{
    T1CONbits.TMR1ON = 0x01; //Switch On Timer1 
    
    Period = Period << 2; // multiply period by 4 since one instruction is 1/4 micro second @ 16MHz
    CCPR1H = Period >> 8; //Mask high byte
    CCPR1L = Period & 0x00FF; // Mask low byte
    TMR1 = 0; //Reset timer 1
    CCP1IF = 0; //Clear timer1 interrupt flag
    
    do
    {
        //do nothing until timer matches compare value i.e. specified period elapses 
    }while(CCP1IF == 0);
    
    T1CONbits.TMR1ON = 0x00; //Switch Off Timer1
}

void IR_Init(uint8_t DutyCycle, uint8_t Freq)
{
    uint16_t ccpr2l_dc2b_val = (Freq) << DutyCycle; //[PR] * 1 (for DC = 1/4) and [PR] * 2 (for DC = 1/2) - approximation 
    PWM_CCP2Init(ccpr2l_dc2b_val, Freq);
}

void IR_send_command(uint8_t *CommandFramePtr, uint8_t DutyCycle)
{
    //Set IR frame command parameters
    Carrier = CommandFramePtr[1];
    Flags = CommandFramePtr[2];
    PreambleOnPeriod = ((uint16_t)CommandFramePtr[3] & 0x00FF) + ((uint16_t)CommandFramePtr[4] << 8);
    PreambleOffPeriod = ((uint16_t)CommandFramePtr[5] & 0x00FF) + ((uint16_t)CommandFramePtr[6] << 8);
    MarkPeriod = ((uint16_t)CommandFramePtr[7] & 0x00FF) + ((uint16_t)CommandFramePtr[8] << 8);
    SpacePeriod = ((uint16_t)CommandFramePtr[9] & 0x00FF) + ((uint16_t)CommandFramePtr[10] << 8);
    GapPeriod = ((uint16_t)CommandFramePtr[11] & 0x00FF) + ((uint16_t)CommandFramePtr[12] << 8);
    PostamblePeriod = ((uint16_t)CommandFramePtr[13] & 0x00FF) + ((uint16_t)CommandFramePtr[14] << 8);
    CommandBitLength = CommandFramePtr[15];
    CommandByteLength = (CommandBitLength >> 3); //Divide bit length by eight to obtain byte length
    CommandPtr = &CommandFramePtr[16]; //Pointer to actual command data
     
    IR_Init(DutyCycle,Carrier); //Configure carrier frequency before sending command
    
    uint8_t EncodingType = Flags && ENCODING_TYPE_MASK; //Mask encoding type from flags byte
       
    //Transmit command
    if (EncodingType == PULSE_POSITION_ENCODING)
    {        
        IR_EncodePulsePosition();
    }else if (EncodingType == PULSE_LENGTH_ENCODING)
    {
        IR_EncodePulseLength();
    }else if (EncodingType == BI_PHASE_ENCODING)
    {
        IR_EncodeBiPhase();
    }
}    

void IR_EncodePulsePosition()
{
    //Transmit Preamble
    PWM_CCP2Start();
    IR_Delay(PreambleOnPeriod);
    PWM_CCP2Stop();
    IR_Delay(PreambleOffPeriod);
    
    //Transmit IR Frame
    for(uint8_t CommandByte = 0; CommandByte < CommandByteLength; CommandByte++)
    {
        for (uint8_t CommandBit=0; CommandBit < 8; CommandBit++) //Iterate 8 times since there are 8 bits in a byte  
        {
            if(CommandBitLength == 0){break;} //This is necessary if the numbers of bits do not match to byte boundaries eg. 14 bits

            //Transmit bit values Starting from the least significant bit of each byte
            uint8_t BitValue = (CommandPtr[CommandByte]) >> CommandBit & 0x01;

            if (BitValue == 1) //If bit equals 1
            {
                PWM_CCP2Start();
                IR_Delay(GapPeriod);
                PWM_CCP2Stop();
                IR_Delay(MarkPeriod);
                CommandBitLength = CommandBitLength - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }else if(BitValue == 0) //If bit equals 0
            {
                PWM_CCP2Start();
                IR_Delay(GapPeriod);
                PWM_CCP2Stop();
                IR_Delay(SpacePeriod);
                CommandBitLength = CommandBitLength - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }   
        }
    }
        
    //Transmit Post-amble
    PWM_CCP2Start();
    IR_Delay(PostamblePeriod);
    PWM_CCP2Stop();
    IR_Delay(GapPeriod);
}

void IR_EncodePulseLength()
{
    //Transmit Preamble
    PWM_CCP2Start();
    IR_Delay(PreambleOnPeriod);
    PWM_CCP2Stop();
    IR_Delay(PreambleOffPeriod);
    
    //Transmit IR Frame
    for(uint8_t CommandByte=0; CommandByte < CommandByteLength; CommandByte++)
    {
        for (uint8_t CommandBit=0; CommandBit < 8; CommandBit++) //Iterate 8 times since there are 8 bits in a byte  
        {
            if(CommandBitLength == 0){break;} //This is necessary if the numbers of bits do not match to byte boundaries eg. 14 bits

            //Transmit bit values Starting from the least significant bit of each byte
            uint8_t BitValue = (CommandPtr[CommandByte]) >> CommandBit & 0x01;

            if (BitValue == 1) //If bit equals 1
            {
                PWM_CCP2Start();
                IR_Delay(MarkPeriod);
                PWM_CCP2Stop();
                IR_Delay(GapPeriod);
                CommandBitLength = CommandBitLength - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }else if (BitValue == 0) //If bit equals 0
            {
                PWM_CCP2Start();
                IR_Delay(SpacePeriod);
                PWM_CCP2Stop();
                IR_Delay(GapPeriod);
                CommandBitLength = CommandBitLength - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }   
        }
    }
        
    //Transmit Post-amble
    PWM_CCP2Start();
    IR_Delay(PostamblePeriod);
    PWM_CCP2Stop();
    IR_Delay(GapPeriod);
}

void IR_EncodeBiPhase()
{
    //Transmit Preamble
    PWM_CCP2Start();
    IR_Delay(PreambleOnPeriod);
    PWM_CCP2Stop();
    IR_Delay(PreambleOffPeriod);
   
    //Transmit IR Frame
    for(uint8_t CommandByte=0; CommandByte < CommandByteLength; CommandByte++)
    {
        for (uint8_t CommandBit=0; CommandBit < 8; CommandBit++) //Iterate 8 times since there are 8 bits in a byte  
        {
            if(CommandBitLength == 0){break;} //This is necessary if the numbers of bits do not match to byte boundaries eg. 14 bits

            //Transmit bit values Starting from the least significant bit of each byte
            uint8_t BitValue = (CommandPtr[CommandByte]) >> CommandBit & 0x01;

            if (BitValue == 1) //If bit equals 1
            {
                PWM_CCP2Stop();
                IR_Delay(MarkPeriod);
                PWM_CCP2Start();
                IR_Delay(MarkPeriod);
                CommandBitLength = CommandBitLength - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }else if (BitValue == 0) //If bit equals 0
            {
                PWM_CCP2Start();
                IR_Delay(SpacePeriod);
                PWM_CCP2Stop();
                IR_Delay(SpacePeriod);
                CommandBitLength = CommandBitLength - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }   
        }
    }
    
    //Transmit Post-amble
    PWM_CCP2Start();
    IR_Delay(PostamblePeriod);
    PWM_CCP2Stop();
    IR_Delay(GapPeriod);
}