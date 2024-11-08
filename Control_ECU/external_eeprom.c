/******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory operations using
 *              the TWI (I2C) protocol.
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#include "external_eeprom.h"
#include "twi.h"

// Function to write a single byte to EEPROM
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
    // Start the TWI communication
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    // Send the EEPROM device address (write operation)
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    // Send the memory address (lower 8 bits)
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    // Write the byte of data to the EEPROM
    TWI_writeByte(u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    // Stop the TWI communication
    TWI_stop();

    return SUCCESS;
}

// Function to read a single byte from EEPROM
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
    // Start the TWI communication
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    // Send the EEPROM device address (write operation)
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700) >> 7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    // Send the memory address (lower 8 bits)
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    // Send a repeated start to prepare for read operation
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;

    // Send the EEPROM device address (read operation)
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700) >> 7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    // Read the byte from EEPROM without sending ACK
    *u8data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    // Stop the TWI communication
    TWI_stop();

    return SUCCESS;
}

// Function to write multiple bytes to EEPROM
uint8 EEPROM_writeData(uint16 u16addr, uint8 *u8data, uint8 size)
{
    uint8 i;

    // Start the TWI communication
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    // Send the EEPROM device address (write operation)
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    // Send the memory address (lower 8 bits)
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    // Write each byte of data to EEPROM
    for (i = 0; i < size; i++) {
        TWI_writeByte(u8data[i]);
        if (TWI_getStatus() != TWI_MT_DATA_ACK)
            return ERROR;
    }

    // Stop the TWI communication
    TWI_stop();

    return SUCCESS;
}

// Function to read multiple bytes from EEPROM
uint8 EEPROM_readData(uint16 u16addr, uint8 *u8data, uint8 size)
{
    uint8 i;

    // Start the TWI communication
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    // Send the EEPROM device address (write operation)
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700) >> 7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    // Send the memory address (lower 8 bits)
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    // Send a repeated start for read operation
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;

    // Send the EEPROM device address (read operation)
    TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700) >> 7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    // Read all but the last byte (send ACK after each read)
    for (i = 0; i < size - 1; i++) {
        u8data[i] = TWI_readByteWithACK();
        if (TWI_getStatus() != TWI_MR_DATA_ACK)
            return ERROR;
    }

    // Read the last byte (send NACK to indicate end of data)
    u8data[i] = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    // Stop the TWI communication
    TWI_stop();

    return SUCCESS;
}
