/******************************************************************************
 *
 * Module: PIR Sensor Driver
 *
 * File Name: pir.h
 *
 * Description: Header file for the PIR sensor driver
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"
#define PIR_SENSOR_PORT PORTC_ID // Define the port connected to the PIR sensor
#define PIR_SENSOR_PIN PIN2_ID  // Define the pin connected to the PIR sensor
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void PIR_init(void);
uint8 PIR_getState(void);

#endif /* PIR_H_ */
