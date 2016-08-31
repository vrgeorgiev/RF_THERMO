/*
 * main.h
 *
 * Created: 14.5.2013 г. 20:37:12 ч.
 *  Author: Bibi
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define MCU_XTAL 7372800UL
#define VREF	(uint16_t)4096 //mV
#define MULT	(uint16_t)(VREF/(uint16_t)1024)


#define PRESCALER_1    1
#define PRESCALER_8    2
#define PRESCALER_64   3
#define PRESCALER_256  4
#define PRESCALER_1024 5

#define T1_PRESCALER		PRESCALER_1024  // Timer1 resolution = 138us @ 7.3728MHz


// convert microseconds to counter values
#define us2cnt(us) ((unsigned int)((unsigned long)(us) / (1000000L / (float)((unsigned long)MCU_XTAL / 1024UL))))

// convert milliseconds to counter values
#define ms2cnt(ms)   ((unsigned int) us2cnt((unsigned long)(ms) * 1000))

// convert seconds to counter values
#define s2cnt(s)   ((unsigned int) ms2cnt((unsigned long)(s) * 1000))

#define TIME_25ms ms2cnt(25)
#define TIME_1s s2cnt(1)
#define TIME_5s s2cnt(5)


int main(void);

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


static inline void init_relay(void)
{
	DDRD |= (1 << DDD2);
	PORTD &= ~(1 << PORTD2);
}

static inline void relay_on(void)
{
	PORTD |= (1 << PORTD2);
}

static inline void relay_off(void)
{
	PORTD &= ~(1 << PORTD2);
}

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

#endif /* MAIN_H_ */