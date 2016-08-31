/*
 * rf_dll.c
 *
 * Created: 14.5.2013 г. 20:41:16 ч.
 *  Author: Bibi
 */ 
#include <avr/io.h>
#include "rf_dll.h"
#define MCU_XTAL 7372800UL

void rf_send_byte(unsigned char byte)
{
	unsigned char man_tx;
	unsigned char nibble=byte&0xf0;
	nibble = nibble>>4;
	man_tx = man_encode (nibble);
	while((UCSRA&(1<<UDRE)) == 0);
	UDR = man_tx;
	
	nibble=byte&0x0f;
	man_tx = man_encode (nibble);
	while((UCSRA&(1<<UDRE)) == 0);
	UDR = man_tx;
}

void rf_send_msg(unsigned char *msg_buf, unsigned char nbytes)
{
	unsigned char i;
	rf_tx_enable();
	preamble();	
	rf_send_byte(RXADDR);
	rf_send_byte(TXADDR);
	rf_send_byte(nbytes);
	unsigned char checksum = RXADDR + TXADDR + nbytes;
	for(i=0 ; i < nbytes; i++)
	{
		checksum += *msg_buf;
		rf_send_byte(*msg_buf++);
	}
	rf_send_byte(checksum);
	rf_tx_disable();
}

void rf_tx_enable(void)
{
    UBRRH = UART_BAUDRATE>>8;     
    UBRRL = UART_BAUDRATE;
    UCSRB =(1<<TXEN); 
    UCSRC =((1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0));   
}

void rf_tx_disable(void)
{
	//UCSRB =0;
	//UCSRC =0x80;
	
	UCSRB &=~(1<<TXEN);
		
    DDRD |= (1 << DDD1);   
    PORTD &= ~(1 << PORTD1);     
}

unsigned char rf_recv_byte (void)
{
	while((UCSRA&(1<<RXC)) == 0);
	return UDR;
}

void rf_rx_init(void)
{
	UBRRH = UART_BAUDRATE>>8;    
    UBRRL = UART_BAUDRATE;   
    UCSRB =(1<<RXEN);
    UCSRC =((1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0));    
}

unsigned int rf_recv_msg(unsigned char *msg_buf)
{	
	
	unsigned char try = 0;
	unsigned char valid_man_data, rcvd_byte, high_nibble, low_nibble, rx_addr, tx_addr, msg_len, check_sum;	
	valid_man_data = MAN_DATA_KO;
	do
	{
		try++;
		rcvd_byte= rf_recv_byte();
		valid_man_data == check_valid_data(rcvd_byte);
		if (try > TRY) 
			return RX_TIME_OUT | 0x8000;		
	}while((valid_man_data == MAN_DATA_KO));
	
	high_nibble = man_decode (rcvd_byte);
	
	rcvd_byte = rf_recv_byte();
	if (MAN_DATA_OK == check_valid_data(rcvd_byte))
		{
			low_nibble = man_decode (rcvd_byte);
			rx_addr = nibbles_to_byte(high_nibble, low_nibble);
		}
	else
		return CRRPTD_DATA | 0x8000;
		
	
	if (rx_addr != RXADDR)
		return RX_ADDR_ERR | 0x8000;
	
	tx_addr = get_byte();
	if (tx_addr != TXADDR)
		return TX_ADDR_ERR | 0x8000;
	
	msg_len = get_byte();
	check_sum = rx_addr + tx_addr + msg_len;
	
	for(unsigned char i=0; i < msg_len; i++)
	{
		rcvd_byte = get_byte();
		*(msg_buf++) = rcvd_byte;
		check_sum += rcvd_byte;
	}
	
	if (check_sum != get_byte())
		return CHK_SUM_ERR | 0x8000;
	
	return msg_len;
}

unsigned char get_byte(void)
{
	unsigned char rcvd_byte, high_nibble, low_nibble, result;
	rcvd_byte = rf_recv_byte();
	high_nibble = man_decode (rcvd_byte);
	rcvd_byte = rf_recv_byte();
	low_nibble = man_decode (rcvd_byte);
	result = nibbles_to_byte(high_nibble, low_nibble);
	return result;
}

unsigned char man_encode (unsigned char in_nibble)
{
	volatile unsigned char out_byte = 0;
	for(unsigned char i=0; i<4; i++)
	{
		out_byte = out_byte << 2;
		out_byte = out_byte +  (in_nibble & 0x01) + 0x01;
		in_nibble = in_nibble >> 1;
	}
	return out_byte;
}

unsigned char man_decode (unsigned char in_byte)
{
	unsigned char out_nibble = 0;
	for(unsigned char i=0; i<3; i++)
	{
		out_nibble = out_nibble + (in_byte & 0x03) - 1;
		out_nibble = out_nibble < 1;
		in_byte = in_byte >> 2;
	}
	out_nibble = out_nibble + (in_byte & 0x03) - 1;
	return out_nibble;
}

unsigned char nibbles_to_byte(unsigned char high_nibble, unsigned char low_nibble)
{
	unsigned char result=0;
	result = high_nibble;
	result = result << 4;
	result += low_nibble;
	return result;
}

unsigned char check_valid_data(unsigned char byte)
{
	if ((byte && 0xC0) == 0xC0)
		return MAN_DATA_KO;
	if ((byte && 0x30) == 0x30)
		return MAN_DATA_KO;
	if ((byte && 0x0C) == 0x0C)
		return MAN_DATA_KO;
	if ((byte && 0x03) == 0x03)
		return MAN_DATA_KO;
	if ((byte && 0xC0) == 0x00)
		return MAN_DATA_KO;
	if ((byte && 0x30) == 0x00)
		return MAN_DATA_KO;
	if ((byte && 0x0C) == 0x00)
		return MAN_DATA_KO;
	if ((byte && 0x03) == 0x00)
		return MAN_DATA_KO;		
		
	return MAN_DATA_OK;
	
}

void preamble(void)
{
	for(unsigned char i=0; i < START_STOP_BYTES; i++)
	{
		while((UCSRA&(1<<UDRE)) == 0);
		UDR = 0xF0;
	}
}



/*
void SendData(BYTE txbyte)
{
	int i,j,b,me;
	b = txbyte;
	for (i=0; i<2; i++) 
	{
		me = 0; // manchester encoded txbyte
		for (j=0 ; j<4; j++) 
		{
			me >>=2;
			if (bit_test(b,0) )
				me |= 0b01000000; // 1->0
			else
				me |= 0b10000000; // 0->1
			b >>=1;
		}
		putc(me);
	}
}

BYTE DecodeData(BYTE encoded)
{
	BYTE i,dec,enc,pattern;
	enc = encoded;
	if (enc == 0xf0) // start/end condition encountered
		return 0xf0;
	dec = 0;
	for (i=0; i<4; i++) 
	{
		dec >>=1;
		pattern = enc & 0b11;
		if (pattern == 0b01) // 1
			bit_set(dec,3);
		else 
			if (pattern == 0b10)
				bit_clear(dec,3); // 0
			else
				return 0xff; // illegal code
		enc >>=2;
	}
	return dec;
}
*/