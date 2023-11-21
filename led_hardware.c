#include <lpc214x.h>
#include <stdio.h>
#include <stdint.h>

void main()
{
    PINSEL2 = 0;
    IODIR1 = 0xFFFFFFFF;
    while (1)
    {
        IOSET1 = 0xFFFFFFFF;
        delay_hw_nm(500);
        IOCLR1 = 0xFFFFFFFF;
        delay_hw_wm(500);
    }
}

void delay_hw_nm(unsigned int time)
{
    T0CTCR = 0;
    T0TCR = 0;
    T0PR = 59999;
    T0TCR = 0x01;
    while (T0TC < time)
        ;
    T0TCR = 0;
}

void delay_hw_wm(unsigned int time)
{
    T0CTCR = 0;
    T0TCR = 0;
    T0PR = 59999;
    T0MR0 = 1000;
    T0MCR = 0x0002;
    T0TCR = 0x02;
    T0TCR = 0x01;
    while (T0TC != T0MR0)
        ;
    T0TCR = 0;
}
