#include "sys.h"

void SYS_Init()
{
    OSCCON = 0x78;  //Configure internal oscillator 16MHz
}
