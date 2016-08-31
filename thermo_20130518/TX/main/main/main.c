/*
 * main.c
 *
 * Created: 14.5.2013 г. 20:36:50 ч.
 *  Author: Bibi
 */ 


#include <avr/io.h>
#include "rf_dll.h"
#include "adc.h"
#include "main.h"


int main(void)
{
	init_adc();
	init_relay();
	
	
	uint16_t adc_result_0, adc_result_1, voltage_0, voltage_1;
	char temp_0, temp_1;
	unsigned char i;

	 
		
	while(1)
	{
		adc_result_0 = 0;
		adc_result_1 = 0;
			
		for(i=0; i<10; i++)
			adc_result_0 += read_adc(0);
		for(i=0; i<10; i++)
			adc_result_1 += read_adc(1);
	
		adc_result_0 /= 10;
		adc_result_1 /= 10;
				
		voltage_0 = adc_result_0 * MULT;
		voltage_1 = adc_result_1 * MULT;
		
		temp_0 = (char) (voltage_0/10 - 273);
		temp_1 = (char) (voltage_1/10 - 273);
		
		if(temp_1 > temp_0)
			relay_on();
		else
			relay_off();
		
	
		unsigned char msg[] = {temp_0, temp_1};
		for(i=0; i<10; i++)
		{
			rf_send_msg(msg, sizeof(msg));
			timer1_start(T1_PRESCALER);
			while(TCNT1 < TIME_25ms);			
		}		
	
		for(i=0; i<1; i++)
		{
			timer1_start(T1_PRESCALER);
			while(TCNT1 < TIME_1s);				
		}			
	}
	
	return 0;
}

