#ifndef UART_H
#define UART_H

void Uart0Init(unsigned int);
void Uart1Init(unsigned int);
unsigned char UART0_GetChar(void);
unsigned char UART0_PutChar(unsigned char);
unsigned char UART1_GetChar(void);
unsigned char UART1_PutChar(unsigned char);
void UART1_PutS(unsigned char *);

#endif
