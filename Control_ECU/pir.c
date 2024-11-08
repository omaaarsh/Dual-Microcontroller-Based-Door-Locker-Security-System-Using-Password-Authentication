/******************************************************************************
 *
 * Module: PIR Sensor Driver
 *
 * File Name: pir.c
 *
 * Description: Source file for the PIR sensor driver
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#include "pir.h"
#include "avr/io.h"
#include "common_macros.h" // For BIT_IS_CLEAR and BIT_IS_SET macros
#include "gpio.h"
void PIR_init(void)
{
	GPIO_setupPinDirection(PIR_SENSOR_PORT,PIR_SENSOR_PIN ,PIN_OUTPUT);// Set the PIR sensor pin as an input
	GPIO_writePin(PIR_SENSOR_PORT, PIR_SENSOR_PIN, LOGIC_LOW); // Clear the bit to set as input
}

uint8 PIR_getState(void)
{
    // Read the state of the PIR sensor
    // Return 1 if motion is detected, otherwise return 0
    if (GPIO_readPin(PIR_SENSOR_PORT,PIR_SENSOR_PIN)) // Check if the PIR pin is high
    {
        return 1; // Motion detected
    }
    return 0; // No motion detected
}
