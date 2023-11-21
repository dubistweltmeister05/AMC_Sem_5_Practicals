#include <stdio.h>
#include <stdint.h>
#include <lpc214x.h>

void uart_init(unsigned int br)
{
    unsigned int fdiv = (15000000 / 16) / br;
    PINSEL0 &= 0XFFFFFFF0;
    PINSEL0 |= 0X00000005;
    U0LCR = 0x83;
    U0DLM = fdiv / 256;
    U0DLL = fdiv % 256;
    U0LCR = 0x03;
    U0TER = 0x80;
}

unsigned char uart_getchar()
{
    while (!(U0LSR & 0x01))
        ;
    return (U0RBR);
}

int uart_putchar(unsigned char ch)
{
    while (!(U0LSR & 0x20))
        ;
    return (U0THR = ch);
}

int fputc(int ch, FILE *f)
{
    return uart_putchar(ch);
}

struct __FILE
{
    int handle;
};
FILE __stdout;

void main()
{
    uart_init(9600);
    printf("Uart demp \r\n");
    printf("Hit a key on the keyboard\r\n");
    while (1)
    {
        uart_putchar(uart_getchar() + 1);
    }
}