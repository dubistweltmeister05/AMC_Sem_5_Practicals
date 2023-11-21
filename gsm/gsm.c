#include "lpc214x.h"
#include "UART.h"
#include "stdio.h"
#include "string.h"

void delay(unsigned int time)
{
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 5000; j++)
            ;
}

void GSM_cmd(unsigned char *cmd)
{
    printf("\r\ncommand:  %s", cmd);
    printf("response: ");
    UART1_PutS(cmd);
    delay(100);
}

int main()
{
    unsigned char *msg = "MicroEmbedded Technologies\r";

    Uart0Init(9600);
    Uart1Init(9600);
    printf("GSM program\r\n");

    GSM_cmd("ATE0\r\n"); // Turn OFF echo
    delay(3000);

    GSM_cmd("ATD9860422236;\r\n"); // Call	  (CHANGE THIS NUMBER)
    delay(20000);
    GSM_cmd("ATH0\r"); // disconnect call
    delay(3000);

    GSM_cmd("AT+CMGF=1\r\n"); // Send SMS: Select Text mode
    delay(1000);
    GSM_cmd("AT+CSCS=\"GSM\"\r\n"); // GSM Character Set
    delay(1000);
    GSM_cmd("AT+CMGS=\"9860422236\"\r\n"); // Number	  (CHANGE THIS NUMBER)
    delay(1000);
    UART1_PutS(msg); // Send string
    delay(1000);
    UART1_PutChar(0x1A); // SUBSTITUTE character <CTRL+Z>

    while (1)
        ;
    return 0;
}