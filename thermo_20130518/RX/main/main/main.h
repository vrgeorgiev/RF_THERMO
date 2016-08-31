/*
 * main.h
 *
 * Created: 14.5.2013 ?. 20:37:12 ?.
 *  Author: Bibi
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define MCU_XTAL 7372800UL

#define MSG_BUF_SIZE	10



int main(void);
void usart_send_msg(unsigned char *msg_buf);
void serial_put_byte2ascii(unsigned char val);
void serial_putc(unsigned char data);

#endif /* MAIN_H_ */