/*
 * rf_dll.h
 *
 * Created: 14.5.2013 г. 20:41:32 ч.
 *  Author: Bibi
 */ 


#ifndef RF_DLL_H_
#define RF_DLL_H_

#ifndef F_CPU
#define F_CPU 7372800
#endif

#define BAUD_RATE    600	
#define UART_BAUDRATE   ((unsigned int)((unsigned long)MCU_XTAL/((unsigned long)BAUD_RATE*16)-1))	// calculate baud rate value for UBBR 
#define RXADDR		0xFF
#define TXADDR		0x01
#define SYNCLEN		0x0A
#define TRY			0x0A
#define RX_ADDR_ERR	0x01
#define TX_ADDR_ERR	0x02
#define CHK_SUM_ERR 0x03
#define RX_TIME_OUT 0x04
#define CRRPTD_DATA	0x05

#define SETBIT(x,y) (x |= (y)) 		// Set bit y in byte x
#define CLEARBIT(x,y) (x &= (~y)) // Clear bit y in byte x

#define MAN_DATA_OK	0
#define MAN_DATA_KO	1

#define START_STOP_BYTES	8

void rf_send_byte(unsigned char byte);
void rf_send_msg(unsigned char *msg_buf, unsigned char nbytes);
void rf_tx_enable(void);
void rf_tx_disable(void);
unsigned char rf_recv_byte (void);
void rf_rx_init(void);
unsigned int rf_recv_msg(unsigned char *msg_buf);
unsigned char get_byte(void);

unsigned char man_encode (unsigned char in_nibble);
unsigned char man_decode (unsigned char in_byte);
unsigned char nibbles_to_byte(unsigned char high_nibble, unsigned char low_nibble);
unsigned char check_valid_data(unsigned char byte);
void preamble(void);



#endif /* RF_DLL_H_ */