#include <stdio.h>
#include <stdint.h>
#include <lpc214x.h>

void delay(unsigned int time)
{
    for (int i = 0; i < time; i++)
        for (int j = 0; j < 200; j++)
            ;
}

void main()
{
    PINSEL2 = 0X00080000;
    unsigned int value = 0;
    while (1)
    {
        value = 1023;
        DACR = (1 << 16 | (value << 6));
        delay(500);
        value = 0;
        DACR = (1 << 16 | (value << 6));
        delay(500);
    }
}