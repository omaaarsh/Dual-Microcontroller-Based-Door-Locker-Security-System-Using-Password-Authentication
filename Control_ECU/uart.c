 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr) {
    uint16_t ubrr_value = 0;

    /* Enable double transmission speed */
    UCSRA = (1 << U2X);

    /* Enable receiver and transmitter */
    UCSRB = (1 << RXEN) | (1 << TXEN);

    /* Set UCSRC configuration */
    UCSRC = (1 << URSEL); // Required for setting UCSRC

    /* Set data bits */
    if (Config_Ptr->bit_data == EIGHT_BITS) {
        UCSRC |= (1 << UCSZ0) | (1 << UCSZ1);
        UCSRB &= ~(1 << UCSZ2);
    } else if (Config_Ptr->bit_data == NINE_BITS) {
        UCSRC |= (1 << UCSZ0) | (1 << UCSZ1);
        UCSRB |= (1 << UCSZ2);
    }

    /* Set parity mode */
    switch (Config_Ptr->parity) {
        case NO_PARITY:
            UCSRC &= ~((1 << UPM0) | (1 << UPM1));
            break;
        case EVEN_PARITY:
            UCSRC |= (1 << UPM1);
            break;
        case ODD_PARITY:
            UCSRC |= (1 << UPM0) | (1 << UPM1);
            break;
    }

    /* Set stop bit configuration */
    if (Config_Ptr->stop_bit == TWO_STOP_BITS) {
        UCSRC |= (1 << USBS);
    } else {
        UCSRC &= ~(1 << USBS);
    }

    /* Set baud rate */
    ubrr_value = (uint16_t)((F_CPU / (Config_Ptr->baud_rate * 8UL)) - 1);
    UBRRH = (uint8_t)(ubrr_value >> 8);
    UBRRL = (uint8_t)ubrr_value;
}
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}		
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
