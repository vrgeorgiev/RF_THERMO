#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
//#include "iso.h"


#define TIME_100us	us2cnt(100)

#define TIME_1ms	ms2cnt(1)
#define TIME_5ms	ms2cnt(5)
#define TIME_15ms	ms2cnt(15)
#define TIME_200ms	ms2cnt(200)

#define TIME_1s		ms2cnt(1000)
#define TIME_3s		ms2cnt(3000)

#define RS_PIN	5
#define E_PIN	4

#define RS_HI	PORTC |= _BV(RS_PIN)
#define RS_LOW	PORTC &= ~_BV(RS_PIN)

#define E_HI	PORTC |= _BV(E_PIN)
#define E_LOW	PORTC &= ~_BV(E_PIN)

#define LCD_CLEAR	0x02

#define PRESCALER_1    1
#define PRESCALER_8    2
#define PRESCALER_64   3
#define PRESCALER_256  4
#define PRESCALER_1024 5

// convert microseconds to counter values
#define us2cnt(us) ((unsigned int)((unsigned long)(us) / (1000000L / (float)((unsigned long)MCU_XTAL / 64L))))

// convert milliseconds to counter values
#define ms2cnt(ms)   ((unsigned int) us2cnt((unsigned long)(ms) * 1000))

#define T0_PRESCALER		PRESCALER_8  // Timer0 resolution = 1us @ 7.3728MHz
#define T1_PRESCALER		PRESCALER_64  // Timer1 resolution = 8us @ 7.3728MHz

void lcd_low(unsigned char data);
void lcd_hi(unsigned char data);
void lcd_pulse_e();
void lcd_cmd(unsigned char cmd);
void lcd_char(unsigned char data);
void lcd_string(char *data);
void lcd_xy(unsigned char x, unsigned char y);
void lcd_init(void);
void lcd_clear(void);
void lcd_clear_row(unsigned char row);
void lcd_clear_row(unsigned char row);

static inline void timer0_start(unsigned char val)
{
	TCCR0 = val;
	TCNT0 = 0;
}

static inline void timer0_stop(void)
{
	TCCR0 = 0x00;
}

static inline void timer0_set(unsigned char val)
{
	TCNT0 = val;
}

static inline void timer1_start(unsigned char val)
{
	TCCR1B = val;
	TCNT1 = 0;
}

static inline void timer1_stop(void)
{
	TCCR1B = 0x00;
}

static inline void timer1_set(unsigned int val)
{
	TCNT1 = val;
}

#endif /* LCD_H_ */