/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/* Define types for configuration */
typedef enum {
    NO_PARITY,
    EVEN_PARITY,
    ODD_PARITY
} UART_ParityType;

typedef enum {
    ONE_STOP_BIT,
    TWO_STOP_BITS
} UART_StopBitType;

typedef enum {
    FIVE_BITS,
    SIX_BITS,
    SEVEN_BITS,
    EIGHT_BITS,
    NINE_BITS
} UART_BitDataType;

typedef uint32 UART_BaudRateType;

/* Configuration structure */
typedef struct {
    UART_BitDataType bit_data;    // Number of data bits
    UART_ParityType parity;        // Parity bit type
    UART_StopBitType stop_bit;     // Number of stop bits
    UART_BaudRateType baud_rate;   // Baud rate for communication
} UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the UART device by:
 * 1. Setting up the frame format like number of data bits, parity bit type, and number of stop bits.
 * 2. Enabling the UART.
 * 3. Setting up the UART baud rate.
 *
 * Parameters:
 *  Config_Ptr: Pointer to the configuration structure
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #
#endif /* UART_H_ */

