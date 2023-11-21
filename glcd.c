#include <stdio.h>
#include <stdint.h>
#include <lpc214x.h>

#define lcd_port 0X00FF0000
#define grst 1 << 24
#define cs1 1 << 26
#define cs2 1 << 27
#define rs 1 << 25
#define en 1 << 10
#define rw 1 << 11
#define lcd_shift 16

void glcd_data(unsigned char ch);
void glcd_cmd(unsigned char ch);
void glcd_init();
void glcd_disp(const unsigned char *temp1);

unsigned char temp[1024] = {};

void delay(unsigned int time)
{
    for (int i = 0; i < time; i++)
        for (int j = 0; j < 50; j++)
            ;
}

void delay(unsigned int time)
{
    for (int i = 0; i < time; i++)
        for (int j = 0; j < 5000; j++)
            ;
}

void glcd_strobe()
{
    IOSET0 = en;
    delay(5);
    IOCLR0 = en;
}

void glcd_data(unsigned char ch)
{
    IOCLR1 = lcd_port;
    IOSET1 = ch << lcd_shift;
    IOSET1 = rs;
    glcd_strobe();
}

void glcd_cmd(unsigned char ch)
{
    IOCLR1 = lcd_port;
    IOSET1 = ch << lcd_shift;
    IOCLR1 = rs;
    glcd_strobe();
}

void glcd_init()
{
    PINSEL0 = PINSEL1 = PINSEL2 = 0;
    IODIR0 = en | rs;
    IODIR1 = cs1 | cs2 | grst | lcd_port | rs;

    IOSET1 = cs1 | cs2 | grst;
    IOCLR1 = lcd_port | rs;
    IOCLR0 = en | rw;

    for (int i = 0; i < 10; i++)
    {
        glcd_cmd(0x3F);
        glcd_cmd(0x40);
        glcd_cmd(0xB8);
    }
}

void glcd_disp(const unsigned char *temp1)
{
    for (int i = 0; i < 8; i++)
    {
        IOSET1 = cs1;
        IOCLR1 = cs2;
        glcd_cmd(0xB8 | i);
        glcd_cmd(0x40);
        for (int j = 0; j < 64; j++)
        {
            glcd_data(temp1[(128 * i + j) + j]);
        }
        IOCLR1 = cs1;
        IOSET1 = cs2;
        glcd_cmd(0xB8 | i);
        glcd_cmd(0x40);
        for (int j = 64; j < 128; j++)
        {
            glcd_data(temp1[(128 * i) + j]);
        }
    }
}
int main()
{
    GLCD_init();
    GLCD_disp(temp);
    ldelay(4000);
    while (1)
    {
        GLCD_disp(temp);
    }
}