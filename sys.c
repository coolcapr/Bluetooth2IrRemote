#include "sys.h"

void sys_init()
{
    OSCCON = 0x78;  //Configure internal oscillator 16MHz
}
