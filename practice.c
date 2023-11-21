#include <stdio.h>
#include <stdint.h>
#include <lpc214x.h>

#define lcd_port 0x00FF0000
#define rs 1 << 11
#define en 1 << 10
#define rw 1 << 20
#define lcd_shift 16

void lcd_cmd(unsigned char ch)
{
    IOCLR1 = lcd_port;
    IOSET1 = ch << lcd_shift;
    IOCLR0 = rs;
    IOCLR0 = rw;
    IOSET0 = en;
    delay();
    IOCLR0 = en;
}

void lcd_data(unsigned char ch)
{
    IOCLR1 = lcd_port;
    IOSET1 = ch << lcd_shift;
    IOSET0 = rs;
    IOCLR0 = rw;
    IOSET0 = en;
    delay();
    IOCLR0 = en;
}

void lcd_init()
{
    PINSEL0 = 0xFF0FFFFF;
    PINSEL1 = 0xFFFFFCFF;
    PINSEL2 = 0xFFFFFFF0;

    IODIR0 = rs | en | rw;
    IODIR1 = lcd_port;

    lcd_cmd(0x38);
    lcd_cmd(0x06);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

void lcd_disp(int row, int pos, unsigned char *ch)
{
    unsigned char temp;

    if (row == 1)
    {
        temp = 0x80 | (pos - 1);
    }
    else
    {
        temp = 0XC0 | (pos - 1);
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

unsigned int adc_read(unsigned int channel)
{
    AD0CR = 0x00200300 | (1 << channel);
    AD0CR = 1 << 24;

    while (!(AD0GDR & 0x80000000))
        ;
    return ((AD0GDR & 0x0000FFc0) >> 6);
}

void main()
{
    unsigned int temp;
    char buf[16];
    lcd_init();
    adc_init();
    while (1)
    {
        temp = adc_read(1);
        sprintf(buf, "ADC Result 0x%03X", temp);
        lcd_disp(1, 1, buf);
        delay();
    }
}