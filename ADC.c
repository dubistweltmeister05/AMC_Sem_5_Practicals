#include <stdio.h>
#include <stdint.h>
#include <lpc214x.h>

#define lcd_port 0X00FF0000
#define rs 1 << 11
#define en 1 << 10
#define rw 1 << 20
#define lcd_shift 16
void delay(unsigned int time)
{
    for (int i = 0; i < time; i++)
        for (int j = 0; j < 100; j++)
            ;
}
void lcd_cmd(unsigned char ch);
void lcd_data(unsigned char ch);
void lcd_init();
void lcd_display(int row, int pos, char *ch);

void lcd_cmd(unsigned char ch)
{
    IOCLR1 = lcd_port;
    IOSET1 = ch << lcd_shift;
    IOCLR0 = rs;
    IOCLR0 = rw;
    IOSET0 = en;
    delay(100);
    IOCLR0 = en;
}
void lcd_data(unsigned char ch)
{
    IOCLR1 = lcd_port;
    IOSET1 = ch << lcd_shift;
    IOSET0 = rs;
    IOCLR0 = rw;
    IOSET0 = en;
    delay(100);
    IOCLR0 = en;
}
void lcd_init()
{
    PINSEL0 = 0xFF0FFFFF;
    PINSEL1 = 0xFFFFFCFF;
    PINSEL2 = 0XFFFFFFF3;

    IODIR0 = rs | rw | en;
    IODIR1 = lcd_port;

    lcd_cmd(0x38);
    lcd_cmd(0x06);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

void lcd_display(int row, int pos, char *ch)
{
    unsigned char temp;
    if (row == 1)
    {
        temp = (0x80 | (pos - 1));
    }
    else
    {
        temp = (0xC0 | (pos - 1));
    }
    lcd_cmd(temp);
    while (*ch)
    {
        lcd_data(*ch++);
    }
}

void adc_init()
{
    PINSEL1 = 0x05000000;
}

unsigned int adc_read(unsigned char channel)
{
    static unsigned int ad1_data;
    AD0CR = 0x00200300 | (1 << channel);
    AD0CR = 1 << 24;

    while (!(AD0GDR & 0x80000000))
        ;
    ad1_data = (AD0GDR & 0x0000FFC0) >> 6;

    return ad1_data;
}

// void adc_isr() //__irq
// {
//     unsigned int temp;
//     char buf[16];

//     LCD_init();
//     ADCInit();
//     temp = ADC_Read(1);
//     sprintf(buf, " ADC result:0x%03X", temp);
//     LCD_display(1, 1, buf);
//     delay(20);

//     AD0INTEN = 0;
//     VICVectAddr = 0;
// }

// int main(void)
// {
//     LCD_init();
//     ADCInit();

//     AD0INTEN = 0x00000002;
//     VICVectAddr0 = (unsigned int)adc_isr;;
//     VICVectCntl0 = 0x20 | 18;
//     VICIntEnable = 1 << 18;
//     delay(20);

//     while (1)
//         ;
//     return 0;
// }

void main()
{
    unsigned int temp;
    char buf[16];

    LCD_init();
    ADCInit();
    while (1)
    {
        temp = ADC_Read(1);
        sprintf(buf, " ADC result:0x%03X", temp);
        LCD_display(1, 1, buf);
        delay(20);
    }
}