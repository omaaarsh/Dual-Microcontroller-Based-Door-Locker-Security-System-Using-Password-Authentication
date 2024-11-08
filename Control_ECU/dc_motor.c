/******************************************************************************
 *
 * Module: DC Motor Driver
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the DC Motor driver to control the fan speed and direction.
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "gpio.h"
#include "pwm.h"
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for Initialize the DC motor by:
 * 1. setting the direction for the motor pins
 * 2. stopping the motor at the beginning.
 */
void DcMotor_Init(void){
	/* Configure the pins connected to IN1 and IN2 as output pins  */
	GPIO_setupPinDirection(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,PIN_OUTPUT);
	/* Stop the motor at the beginning */
	GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for:
 * 1. Control the motor's state (Clockwise/Anti-Clockwise/Stop)
 * 2. adjusts the speed based on the input duty cycle.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed){
	if(state == CW){
		/* Rotate the motor clock wise */
		GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_HIGH);
	}else if(state == ACW){
		/* Rotate the motor anti-clock wise */
		GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
	}else{
		/* Stop the motor */
		GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
	}
	/* Control The DC Motor Speed using PWM */
	PWM_START(speed);
}

