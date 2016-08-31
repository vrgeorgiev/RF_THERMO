#include "lcd.h"
//#include "iso.h"
#define MCU_XTAL 7372800UL

void lcd_xy(unsigned char x, unsigned char y) {
	
	if (y==0)
		lcd_cmd(0x80 + x);
	else if (y==1)
		lcd_cmd(0xC0 + x);
	else if (y==2)
		lcd_cmd(0x94 + x);
	else if (y==3)
		lcd_cmd(0xD4 + x);

	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_5ms);
}

void lcd_hi(unsigned char data) {
	PORTC = (PORTC & 0xf0) |((data >>4) & 0x0f);
}

void lcd_low(unsigned char data) {
	PORTC = (PORTC & 0xf0) | (data&0x0F);
}

void lcd_init() 
{
	DDRC = 0xff;
	PORTC = 0x00;
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_200ms);

	RS_LOW;
	lcd_hi(0x20);
	lcd_pulse_e();
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);
	
	lcd_cmd(0x28);
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_5ms);

	lcd_cmd(0x06);
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);

	lcd_cmd(0x0C);
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);
	
	lcd_cmd(0x01);
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);
}

void lcd_string(char *data) 
{
	while (*data != 0) 
	{
		lcd_char(*data);
		data++;
	}
}

void lcd_char(unsigned char data) 
{
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);
	RS_HI;
	lcd_hi(data);
	lcd_pulse_e();
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us); 
	lcd_low(data);
	lcd_pulse_e();
}

void lcd_cmd(unsigned char cmd) 
{
	RS_LOW;
	lcd_hi(cmd);
	lcd_pulse_e();
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);
	lcd_low(cmd);
	lcd_pulse_e();
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);
}

void lcd_pulse_e() {
	E_HI;
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us); 
	E_LOW;	
	timer1_start(T1_PRESCALER);
	while (TCNT1 < TIME_100us);
}

void lcd_clear(void)
{
	lcd_xy(0,0);
	for(int i = 0x00; i<0x68;i++)
	lcd_char(' ');
}

void lcd_clear_row(unsigned char row)
{
	if (row==0)
		lcd_xy(0,0);
	if (row==1)
		lcd_xy(0,1);
	if (row==2)
		lcd_xy(0,2);
	if (row==3)
		lcd_xy(0,3);
	lcd_string("                    \0");
}