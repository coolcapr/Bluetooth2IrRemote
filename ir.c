#include "ir.h"
#include "pwm.h"

#define CLEAR                       0
#define SET                         1
#define PULSE_POSITION_ENCODING     0
#define PULSE_LENGTH_ENCODING       1
#define BI_PHASE_ENCODING           2

void IR_encode_pulse_position();
void IR_encode_pulse_length();
void IR_encode_bi_phase();

//IR frame parameters
uint8_t carrier;
uint8_t flags;
uint16_t pre_amble_on_period;
uint16_t pre_amble_off_period;
uint16_t mark_period;
uint16_t space_period;
uint16_t gap_period;
uint16_t post_amble_period;
uint8_t command_bit_length; // command length in bits
uint8_t command_byte_length; // command length in bytes   
uint8_t *command_ptr;

//Custom delay method necessary as MPLab-X built-in delay function requires constant parameters/arguments
void IR_delay(uint16_t period)
{
    period = period << 2; // multiply period by 4 since one instruction is 1/4 micro second
    CCPR1H = period >> 8; //Mask high byte
    CCPR1L = period & 0x00FF; // Mask low byte
    TMR1 = CLEAR; //Reset timer 1
    CCP1IF = CLEAR; //Clear timer1 interrupt flag
    
    do
    {
        //do nothing until timer matches compare value i.e. specified period elapses 
    }while(CCP1IF == 0);
}

void IR_init(uint8_t IR_freq)
{
    IR_set_carrier(IR_freq);
    T1CON = 0x01; //Configure Timer1
    CCP1CON = 0x08; //Configure Timer1   
}

//PWM frequency configuration values based on 16MHz oscillator
void IR_set_carrier(uint8_t IR_freq)
{ 
    if (IR_freq == IR_FREQ_30_0)
    {
        PWM_init(132, 1, 266);
    }
    else if (IR_freq == IR_FREQ_30_5)
    {
        PWM_init(130, 1, 262);
    } 
    else if (IR_freq == IR_FREQ_31_0)
    {
        PWM_init(128, 1, 258);
    }
    else if (IR_freq == IR_FREQ_31_5)
    {
        PWM_init(126, 1, 254);
    }
    else if (IR_freq == IR_FREQ_32_0)
    {
        PWM_init(124, 1, 250);
    }
    else if (IR_freq == IR_FREQ_32_5)
    {
        PWM_init(122, 1, 246);
    }
    else if (IR_freq == IR_FREQ_33_0)
    {
        PWM_init(120, 1, 242);
    }
    else if (IR_freq == IR_FREQ_33_5)
    {
        PWM_init(118, 1, 238);
    }
    else if (IR_freq == IR_FREQ_34_0)
    {
        PWM_init(116, 1, 234);
    }
    else if (IR_freq == IR_FREQ_34_5)
    {
        PWM_init(114, 1, 230);
    }
    else if (IR_freq == IR_FREQ_35_0)
    {
        PWM_init(113, 1, 228);
    }
    else if (IR_freq == IR_FREQ_35_5)
    {
        PWM_init(111, 1, 224);
    }
    else if (IR_freq == IR_FREQ_36_0)
    {
        PWM_init(110, 1, 222);
    }
    else if (IR_freq == IR_FREQ_36_5)
    {
        PWM_init(108, 1, 218);
    }
    else if (IR_freq == IR_FREQ_37_0)
    {
        PWM_init(107, 1, 216);
    }
    else if (IR_freq == IR_FREQ_37_5)
    {
        PWM_init(105, 1, 212);
    }
    else if (IR_freq == IR_FREQ_38_0)
    {
        PWM_init(104, 1, 210);
    }
    else if (IR_freq == IR_FREQ_38_5)
    {
        PWM_init(102, 1, 206);
    }
    else if (IR_freq == IR_FREQ_39_0)
    {
        PWM_init(101, 1, 204);
    }
    else if (IR_freq == IR_FREQ_39_5)
    {
        PWM_init(100, 1, 202);
    }
    else if (IR_freq == IR_FREQ_40_0)
    {
        PWM_init(99, 1, 200);
    }
    else if (IR_freq == IR_FREQ_40_5)
    {
        PWM_init(97, 1, 196);
    }
    else if (IR_freq == IR_FREQ_41_0)
    {
        PWM_init(96, 1, 194);
    }
    else if (IR_freq == IR_FREQ_41_5)
    {
        PWM_init(95, 1, 192);
    }
    else if (IR_freq == IR_FREQ_42_0)
    {
        PWM_init(94, 1, 190);
    }
    else if (IR_freq == IR_FREQ_42_5)
    {
        PWM_init(93, 1, 188);
    }
}

void IR_send_command(uint8_t *command_frame_ptr)
{
    //Set IR frame command parameters
    carrier=command_frame_ptr[1];
    flags = command_frame_ptr[2];
    pre_amble_on_period = ((uint16_t)command_frame_ptr[3] & 0x00FF) + ((uint16_t)command_frame_ptr[4] << 8);
    pre_amble_off_period = ((uint16_t)command_frame_ptr[5] & 0x00FF) + ((uint16_t)command_frame_ptr[6] << 8);
    mark_period = ((uint16_t)command_frame_ptr[7] & 0x00FF) + ((uint16_t)command_frame_ptr[8] << 8);
    space_period = ((uint16_t)command_frame_ptr[9] & 0x00FF) + ((uint16_t)command_frame_ptr[10] << 8);
    gap_period = ((uint16_t)command_frame_ptr[11] & 0x00FF) + ((uint16_t)command_frame_ptr[12] << 8);
    post_amble_period = ((uint16_t)command_frame_ptr[13] & 0x00FF) + ((uint16_t)command_frame_ptr[14] << 8);
    command_bit_length = command_frame_ptr[15];
    command_byte_length = (command_bit_length >> 3); //Divide bit length by eight to obtain byte length
    command_ptr = &command_frame_ptr[16]; //Pointer to actual command data
     
    IR_set_carrier(carrier); //Configure carrier frequency before sending command
    
    uint8_t IR_encoding = flags && 0x03; //Mask encoding type from flags byte
       
    //Transmit command
    if (IR_encoding == PULSE_POSITION_ENCODING)
    {        
        IR_encode_pulse_position();
    }else if (IR_encoding == PULSE_LENGTH_ENCODING)
    {
        IR_encode_pulse_length();
    }else if (IR_encoding == BI_PHASE_ENCODING)
    {
        IR_encode_bi_phase();
    }
}    

void IR_encode_pulse_position()
{
    //Transmit Preamble
    PWM_start();
    IR_delay(pre_amble_on_period);
    PWM_stop();
    IR_delay(pre_amble_off_period);
    
    //Transmit IR Frame
    for(uint8_t command_byte=0; command_byte < command_byte_length; command_byte++)
    {
        for (uint8_t command_bit=0; command_bit < 8; command_bit++) //Iterate 8 times since there are 8 bits in a byte  
        {
            if(command_bit_length == 0){break;} //This is necessary if the numbers of bits do not match to byte boundaries eg. 14 bits

            //Transmit bit values starting from the least significant bit of each byte
            uint8_t bit_val = (command_ptr[command_byte]) >> command_bit & 0x01;

            if (bit_val == SET) //If bit equals 1
            {
                PWM_start();
                IR_delay(gap_period);
                PWM_stop();
                IR_delay(mark_period);
                command_bit_length = command_bit_length - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }else if(bit_val == CLEAR) //If bit equals 0
            {
                PWM_start();
                IR_delay(gap_period);
                PWM_stop();
                IR_delay(space_period);
                command_bit_length = command_bit_length - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }   
        }
    }
        
    //Transmit Post-amble
    PWM_start();
    IR_delay(post_amble_period);
    PWM_stop();
    IR_delay(gap_period);
}

void IR_encode_pulse_length()
{
    //Transmit Preamble
    PWM_start();
    IR_delay(pre_amble_on_period);
    PWM_stop();
    IR_delay(pre_amble_off_period);
    
    //Transmit IR Frame
    for(uint8_t command_byte=0; command_byte < command_byte_length; command_byte++)
    {
        for (uint8_t command_bit=0; command_bit < 8; command_bit++) //Iterate 8 times since there are 8 bits in a byte  
        {
            if(command_bit_length == 0){break;} //This is necessary if the numbers of bits do not match to byte boundaries eg. 14 bits

            //Transmit bit values starting from the least significant bit of each byte
            uint8_t bit_val = (command_ptr[command_byte]) >> command_bit & 0x01;

            if (bit_val == SET) //If bit equals 1
            {
                PWM_start();
                IR_delay(mark_period);
                PWM_stop();
                IR_delay(gap_period);
                command_bit_length = command_bit_length - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }else if (bit_val == CLEAR) //If bit equals 0
            {
                PWM_start();
                IR_delay(space_period);
                PWM_stop();
                IR_delay(gap_period);
                command_bit_length = command_bit_length - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }   
        }
    }
        
    //Transmit Post-amble
    PWM_start();
    IR_delay(post_amble_period);
    PWM_stop();
    IR_delay(gap_period);
}

void IR_encode_bi_phase()
{
    //Transmit Preamble
    PWM_start();
    IR_delay(pre_amble_on_period);
    PWM_stop();
    IR_delay(pre_amble_off_period);
   
    //Transmit IR Frame
    for(uint8_t command_byte=0; command_byte < command_byte_length; command_byte++)
    {
        for (uint8_t command_bit=0; command_bit < 8; command_bit++) //Iterate 8 times since there are 8 bits in a byte  
        {
            if(command_bit_length == 0){break;} //This is necessary if the numbers of bits do not match to byte boundaries eg. 14 bits

            //Transmit bit values starting from the least significant bit of each byte
            uint8_t bit_val = (command_ptr[command_byte]) >> command_bit & 0x01;

            if (bit_val == SET) //If bit equals 1
            {
                PWM_stop();
                IR_delay(mark_period);
                PWM_start();
                IR_delay(mark_period);
                command_bit_length = command_bit_length - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }else if (bit_val == CLEAR) //If bit equals 0
            {
                PWM_start();
                IR_delay(space_period);
                PWM_stop();
                IR_delay(space_period);
                command_bit_length = command_bit_length - 1; //Keep a count of bits in case they do not align to byte boundaries e.g. 14 bits
            }   
        }
    }
    
    //Transmit Post-amble
    PWM_start();
    IR_delay(post_amble_period);
    PWM_stop();
    IR_delay(gap_period);
}