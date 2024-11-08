/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.c
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(const TWI_ConfigType *Config_Ptr)
{
    /* Set the bit rate based on the provided configuration */
    TWBR = (F_CPU / (2 * Config_Ptr->bit_rate)) - 8; // Calculate TWBR for desired baud rate
    TWSR = 0x00; // Prescaler = 1

    /* Set the TWI address for the device */
    TWAR = (Config_Ptr->address << 1); // Set the address (shifted left for read/write bit)

    TWCR = (1 << TWEN); /* Enable TWI */
}

void TWI_start(void)
{
    /* 
     * Clear the TWINT flag before sending the start bit TWINT=1
     * send the start bit by TWSTA=1
     * Enable TWI Module TWEN=1
     */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    /* Wait for TWINT flag set in TWCR Register (start bit is sent successfully) */
    while (BIT_IS_CLEAR(TWCR, TWINT));
}

void TWI_stop(void)
{
    /* 
     * Clear the TWINT flag before sending the stop bit TWINT=1
     * send the stop bit by TWSTO=1
     * Enable TWI Module TWEN=1
     */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data)
{
    /* Put data on TWI data Register */
    TWDR = data;

    /* 
     * Clear the TWINT flag before sending the data TWINT=1
     * Enable TWI Module TWEN=1
     */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data is sent successfully) */
    while (BIT_IS_CLEAR(TWCR, TWINT));
}

uint8 TWI_readByteWithACK(void)
{
    /*
     * Clear the TWINT flag before reading the data TWINT=1
     * Enable sending ACK after reading or receiving data TWEA=1
     * Enable TWI Module TWEN=1
     */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while (BIT_IS_CLEAR(TWCR, TWINT));

    /* Read Data */
    return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
    /*
     * Clear the TWINT flag before reading the data TWINT=1
     * Enable TWI Module TWEN=1
     */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while (BIT_IS_CLEAR(TWCR, TWINT));

    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;

    /* Masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;

    return status;
}
