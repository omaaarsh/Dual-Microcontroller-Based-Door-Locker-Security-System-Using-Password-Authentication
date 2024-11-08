/******************************************************************************
 *
 * Module: DC Motor Driver
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for the DC Motor driver.
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

#define PWM_START(SPEED)	PWM_Timer0_Start(SPEED)

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MOTOR_IN1_PORT_ID			PORTD_ID
#define MOTOR_IN1_PIN_ID			PIN6_ID

#define MOTOR_IN2_PORT_ID			PORTD_ID
#define MOTOR_IN2_PIN_ID			PIN7_ID

#define MOTOR_ENABLE1_PORT_ID		PORTB_ID
#define MOTOR_ENABLE_PIN_ID			PIN3_ID


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	STOP,CW,ACW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for Initialize the DC motor by:
 * 1. setting the direction for the motor pins
 * 2. stopping the motor at the beginning.
 */
void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for:
 * 1. Control the motor's state (Clockwise/Anti-Clockwise/Stop)
 * 2. adjusts the speed based on the input duty cycle.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* MOTOR_H_ */
