//2013 Vladimir Georgiev
//email: georgiev(dot)vladimir(at)gmail(dot).com

#include "tx_dll.h"

void rf_send_byte(unsigned char byte)
{
	unsigned char man_tx;
	unsigned char nibble=byte&0xf0;
	nibble>>4;	
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
	for(unsigned char i=0; i<SYNCLEN; i++)
	rf_send_byte(SYNC); 
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
}

void rf_tx_init(void)
{
	UBRRL=(uint8_t)UBRRVAL;	
	UBRRH=(UBRRVAL>>8);	
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);		
	UCSRB=(1<<TXEN);

}

unsigned char rf_recv_byte (void)
{
    while((UCSRA&(1<<RXC)) == 0);
    return UDR;
}

void rf_rx_init(void)
{
	UBRRL=(uint8_t)UBRRVAL;		
	UBRRH=(UBRRVAL>>8);	
	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);	
	UCSRB=(1<<RXEN);
}
unsigned int rf_recv_msg(unsigned char *msg_buf)
{
	
	unsigned char sync_bytes = 0;
	unsigned char try = 0;
	unsigned char sync_finished = 0;
	unsigned char rcvd_byte, high_nibble, low_nibble, rx_addr, tx_addr, msg_len, check_sum;
	do
	{
	try++;
	rcvd_byte = rf_recv_byte();
	high_nibble = man_decode (rcvd_byte);
	if (high_nibble == SYNC&0x0F)
		sync_bytes++;
	else
		{
		if (sync_bytes>2)
			sync_finished = 1
		}
	}while ( (try < TRY) && (sync_finished == 0))
	
	rcvd_byte = rf_recv_byte();
	low_nibble = man_decode (rcvd_byte);
	rx_addr = nibbles_to_byte(high_nibble, low_nibble);
	
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
		check_sum += rcvd_byte
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
	unsigned char out_byte = 0;
	for(unsigned char i=0; i<4; i++)
	{
		out_byte << 2;		
		out_byte = out_byte +  in_nibble & 0x01 + 0x01;
		in_nibble >> 1;
	}
	return out_byte;
}

unsigned char man_decode (unsigned char in_byte)
{
	unsigned char out_nibble = 0;
	for(unsigned char i=0; i<3; i++)
	{
		out_nibble = out_nibble + in_byte & 0x03 - 1;
		out_nibble < 1;		
		in_byte >> 2;
	}
	out_nibble = out_nibble + in_byte & 0x03 - 1;
}

unsigned char nibbles_to_byte(unsigned char high_nibble, unsigned char low_nibble)
{
	unsigned char result=0;
	result = high_nibble;
	result << 4;
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
	return MAN_DATA_OK
		
}