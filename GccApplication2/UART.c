/*
 * Door_code.c
 *
 * Created: 1/4/2018 10:27:59 AM
 * Author : Shawn
 */ 

#include <avr/interrupt.h>
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
void usart_data_transmit(unsigned char data );
void usart_string_transmit(char *string);
void UARTinitials (void);
volatile char Resive_data;


void UARTinitials (void)
{
	DDRD |= 1<<PIND0|1<<PIND1;
	UCSRB = (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes

	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}

void usart_data_transmit(unsigned char data )
{
	while ( !( UCSRA & (1<<UDRE)) )
	;
	UDR = data;
	_delay_ms(1);
}

void usart_string_transmit(char *string)
{
	while(*string)
	{
		usart_data_transmit(*string++);
	}
}

ISR(USART_RXC_vect)
{
 // Fetch the received byte value into the variable "ByteReceived"
	UDR=Resive_data;
	Resive_data = UDR;
}