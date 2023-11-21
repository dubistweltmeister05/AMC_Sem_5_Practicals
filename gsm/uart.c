#include "stdio.h"
#include "lpc214x.h"
#include "UART.h"

void UART1_isr(void) //__irq
{
    UART0_PutChar(UART1_GetChar());
    VICVectAddr = 0;
}

void Uart0Init(unsigned int baudrate)
{
    unsigned int FDiv;
    PINSEL0 |= 0x00000005;             // Enable RxD0 and TxD0
    U0LCR = 0x83;                      // 8 bits, no Parity, 1 Stop bit
    FDiv = (15000000 / 16) / baudrate; //
    U0DLM = FDiv / 256;                // 0x00;
    U0DLL = FDiv % 256;                // 0x97;
    U0LCR = 0x03;                      // DLAB = 0
}

void Uart1Init(unsigned int baudrate)
{
    unsigned int FDiv;
    PINSEL0 |= 0x00050000;             // Enable RxD1 and TxD1
    U1LCR = 0x83;                      // 8 bits, no Parity, 1 Stop bit
    FDiv = (15000000 / 16) / baudrate; //
    U1DLM = FDiv / 256;                // 0x00;
    U1DLL = FDiv % 256;                // 0x97;
    U1LCR = 0x03;                      // DLAB = 0

    U1IER = 0x01;
    VICVectCntl1 = 0x20 | 7;
    VICVectAddr1 = (unsigned int)UART1_isr;
    VICIntEnable |= 1 << 7;
}

unsigned char UART0_GetChar(void)
{
    while (!(U0LSR & 0x01))
        ;
    return (U0RBR);
}

unsigned char UART1_GetChar(void)
{
    while (!(U1LSR & 0x01))
        ;
    return (U1RBR);
}

unsigned char UART0_PutChar(unsigned char Ch)
{
    while (!(U0LSR & 0x20))
        ;
    //	if(Ch == '\n')
    //		Ch = 0x0D;
    U0THR = Ch;
    return Ch;
}

unsigned char UART1_PutChar(unsigned char Ch)
{
    while (!(U1LSR & 0x20))
        ;
    U1THR = Ch;
    return Ch;
}

void UART1_PutS(unsigned char *Ch)
{
    while (*Ch)
        UART1_PutChar(*Ch++);
}

int fputc(int ch, FILE *f)
{
    return (UART0_PutChar(ch));
}

struct __FILE
{
    int handle; /* Add whatever you need here */
};
FILE __stdout;
