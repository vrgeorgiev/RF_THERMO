/*
 * adc.c
 *
 * Created: 14.5.2013 г. 20:39:00 ч.
 *  Author: Bibi
 */ 

#include <avr/io.h>
#include "adc.h"

void init_adc(void)
{
	ADMUX=(1<<REFS0);                         // For Aref=AVcc;
	ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
	
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Rrescalar div factor =128
}

uint16_t read_adc(uint8_t ch)
{
	int ADCval;

	//Select ADC Channel ch must be 0-7
	ch=ch&0b00000111;
	ADMUX|=ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(ADCSRA & (1 << ADSC));

	ADCval = ADCL;
	ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

	return ADCval;
	
}

