//2013 Vladimir Georgiev
//email: georgiev(dot)vladimir(at)gmail(dot).com

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 7372800
#endif

#define BAUDRATE	1200
#define UBRRVAL		((F_CPU/(BAUDRATE*16UL))-1)
#define SYNC		0xFF //sync signal is b10101010 (0xAA) 0xFF after Manchester encodings ends to 0xAA, 0xAA. 
#define RXADDR		0x02
#define TXADDR		0x01
#define SYNCLEN		0x0A
#define TRY			0x0A
#define RX_ADDR_ERR	0x01
#define TX_ADDR_ERR	0x02
#define CHK_SUM_ERR 0x03




void rf_send_byte(unsigned char byte);
void rf_send_msg(unsigned char *msg_buf, unsigned char nbytes);
void rf_tx_init(void);
unsigned char rf_recv_byte (void);
void rf_rx_init(void);
unsigned int rf_recv_msg(unsigned char *msg_buf);
unsigned char get_byte(void);

unsigned char man_encode (unsigned char in_nibble);
unsigned char man_decode (unsigned char in_byte);
unsigned char nibbles_to_byte(unsigned char high_nibble, unsigned char low_nibble);
unsigned char check_valid_data(unsigned char byte);
