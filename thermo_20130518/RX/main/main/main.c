#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "rf_dll.h"
#include "main.h"
#include "lcd.h"



int main(void)
{
	unsigned char msg_buf[MSG_BUF_SIZE];
	unsigned int cnt;
	
	rf_rx_init();
	lcd_init();

	
	char temp_0, temp_1, buffer[30];
	unsigned char 	rx_cnt = 0;
	lcd_xy(0,0);
	//sprintf(buffer,"start %d\0",1);
	//lcd_string("start!");


	
	while(1)
	{
		cnt = rf_recv_msg(msg_buf);
		if (!(cnt & 0x8000))
		{
			
			rx_cnt++;
			lcd_xy(0,0);
			sprintf(buffer,"cnt: %03d",rx_cnt);
			lcd_string(buffer);
			
			temp_0 = msg_buf[0];
			temp_1 = msg_buf[1];
			
						
			lcd_xy(0,1);
			sprintf(buffer,"T0:%0d%cC T1:%0d%cC", temp_0,0xDF ,temp_1, 0xDF);
			lcd_string(buffer);
			
			sprintf(buffer,"T0:%03d T1:%03d",temp_0, temp_1);	
			usart_send_msg(buffer);
			
		}
		
	}
	
	return 0;
}

void usart_send_msg(unsigned char *msg_buf)
{
	while (*msg_buf != 0)
		serial_putc(*msg_buf++);  
	serial_putc('\r');
	serial_putc('\n');
}

void serial_put_byte2ascii(unsigned char val)
{
	unsigned char ascii1=val;
	serial_putc( ((ascii1>>4) < 10) ? (ascii1>>4) + 48 : (ascii1>>4) + 55 );  // upper nibble
	serial_putc( ((val&0x0f) < 10) ? (val&0x0f) + 48 : (val&0x0f) + 55 );  // lower nibble
}

void serial_putc(unsigned char data)
{
	while((UCSRA&(1<<UDRE)) == 0);
	UDR = data; 								
}; 
