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

void main()
{
    unsigned int temp;
    lcd_init();
    while (1)
    {
        lcd_display(1, 4, "EnTC DEPT");
        lcd_display(2, 4, "PCCOE PUNE");
        delay(20);
    }
}
