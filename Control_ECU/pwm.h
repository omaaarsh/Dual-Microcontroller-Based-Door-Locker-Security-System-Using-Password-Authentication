/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the ATmega32 PWM driver using Timer0
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h" /* For standard types like uint8 */

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/*
 * Description:
 * Function to initialize Timer0 in Fast PWM mode and start PWM signal generation.
 * The function sets the required duty cycle.
 *
 * Parameters:
 * - duty_cycle: A value from 0 to 100 representing the percentage of the duty cycle.
 */
void PWM_Timer0_Start(uint8 duty_cycle);
#endif /* PWM_H_ */
