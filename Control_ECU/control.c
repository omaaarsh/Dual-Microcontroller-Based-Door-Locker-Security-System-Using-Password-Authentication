/*
 ================================================================================================
 Name        : control.c
 Author      : Omar Sherif
 Description : Control_ECU code for password verification and door control
 ================================================================================================
 */

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "std_types.h"
#include "uart.h"
#include "buzzer.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "pir.h"
#include "twi.h"
#include "string.h"
#include "util/delay.h"
#include "timer.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_SIZE 				5
#define PASSWORD_SAVED 				0x12
#define DIFF_PASSWORDS				0x13
#define TRUE_PASSWORD				0x14
#define WRONG_PASSWORD				0x15
#define CONTROL_ECU_READY			0x16
#define LOCKING_DOOR				0x17
#define UNLOCK_DOOR    				0x18
#define ALARM_MODE					0x19
#define CHANGE_PASSWORD				0x20
#define MAX_TRIES                  3

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static volatile uint8 g_ticks = 0;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void getAndSavePassword(void);
void countOneSecond(void);
void Timer1_DelaySecond(uint8 time);
void initializeSystem(void);
void handleDoorControl(uint8 action);

/*******************************************************************************
 *                                    Main                                     *
 *******************************************************************************/

int main(void){
	uint8 size = PASSWORD_SIZE + 1;
	uint8 enteredPass[size], savedPass[size];
	uint8 action = 0;

	// Initialize the system components
	initializeSystem();

	/* Get the password from HMI_ECU and save it in the External EEPRPOM */
	getAndSavePassword();
	_delay_ms(10);

	for(;;){
		uint8 loop_counter;

		/* loop 3 times until the user enters the correct password */
		for(loop_counter = 0; loop_counter < MAX_TRIES; loop_counter++){
			/* Send CONTROL_ECU_READY byte to HMI_ECU to ask it to send the password */
			UART_sendByte(CONTROL_ECU_READY);
			/* Receive the password from HMI_ECU through UART */
			UART_receiveString(enteredPass);

			/* Get the password saved in the EEPROM */
			EEPROM_readData(0x0311, savedPass, PASSWORD_SIZE);
			savedPass[PASSWORD_SIZE] = '\0';

			/* Compare the received password with the saved password */
			if(!strcmp((char*)enteredPass, (char*)savedPass)){
				/* If the two passwords match, send TRUE_PASSWORD byte to HMI_ECU */
				UART_sendByte(TRUE_PASSWORD);
				/* Receive an action byte from HMI_ECU (Open Door or Change Password) */
				action = UART_recieveByte();
				break;
			}else{
				/* If the passwords don't match, send WRONG_PASSWORD byte to HMI_ECU */
				UART_sendByte(WRONG_PASSWORD);
			}
		}

		/* If the user entered the wrong password 3 times */
		if(loop_counter == MAX_TRIES){
			/* Activate the buzzer to alert the user */
			Buzzer_on();
			/* Wait for 60 seconds before deactivating the buzzer */
			Timer1_DelaySecond(60);
			Buzzer_off();
		}
		/* If the user entered the correct password */
		else{
			/* Handle the door control based on the action received */
			handleDoorControl(action);
		}
	}
}

/*
 * Description :
 * Function responsible for initializing all system components.
 */
void initializeSystem(void){
	/* Create configuration structure for UART driver */
	UART_ConfigType uartConfig = {EIGHT_BITS, NO_PARITY, ONE_STOP_BIT, 9600};
	/* Enable Global Interrupt */
	sei();
	/* Initialize the UART driver with:
	 * Baud-rate = 9600 bits/sec, one stop bit, No parity, 8-bit data
	 */
	UART_init(&uartConfig);

	/* Create configuration structure for TWI/I2C driver */
	TWI_ConfigType twiConfig = {0x01, 0x02};
	/* Initialize the TWI driver with:
	 * My address = 0x01, bit rate = 0x02, SCL frequency = 400 Hz
	 */
	TWI_init(&twiConfig);

	/* Initialize the Buzzer */
	Buzzer_init();
	/* Initialize the DC Motor */
	DcMotor_Init();
	/* Initialize the PIR Sensor */
	PIR_init();
}

/*
 * Description :
 * Function responsible for getting the password from HMI_ECU and saving it in the External EEPROM.
 */
void getAndSavePassword(void){
	uint8 size = PASSWORD_SIZE + 1;
	uint8 pass1[size], pass2[size];

	/* Loop until the user enters the same password twice for confirmation */
	for(;;){
		/* Send CONTROL_ECU_READY byte to HMI_ECU to ask it to send the two passwords */
		UART_sendByte(CONTROL_ECU_READY);
		/* Receive the password and the confirmation password from HMI_ECU */
		UART_receiveString(pass1);
		UART_receiveString(pass2);

		/* Compare the two passwords */
		if(!strcmp((char*)pass1, (char*)pass2)){
			/* If the two passwords are the same, save the password in EEPROM */
			EEPROM_writeData(0x0311, pass1, PASSWORD_SIZE);
			/* Send PASSWORD_SAVED byte to HMI_ECU */
			UART_sendByte(PASSWORD_SAVED);
			return;
		}else{
			/* If the two passwords are not the same, send DIFF_PASSWORDS byte to HMI_ECU */
			UART_sendByte(DIFF_PASSWORDS);
		}
	}
}

/*
 * Description :
 * Function to handle the door control logic (unlock and lock).
 * Action parameter determines if the door should be unlocked or if the password should be changed.
 */
void handleDoorControl(uint8 action){
	/* Process Open Door option */
	if(action == UNLOCK_DOOR){
		/* Rotate the motor clockwise to unlock the door */
		DcMotor_Rotate(ACW, 100);
		/* Wait until the door is unlocked for 15 seconds */
		Timer1_DelaySecond(15);
		/* Stop the motor to keep the door open */
		DcMotor_Rotate(STOP, 0);

		/* Wait until PIR sensor detects no motion (all people enter) */
		while(PIR_getState());

		/* Send LOCKING_DOOR byte to HMI_ECU */
		UART_sendByte(LOCKING_DOOR);
		/* Rotate the motor anti-clockwise to lock the door */
		DcMotor_Rotate(CW, 100);
		/* Wait until the door is locked for 15 seconds */
		Timer1_DelaySecond(15);
		/* Stop the motor */
		DcMotor_Rotate(STOP, 0);
	}
	/* Process Change Password option */
	else if(action == CHANGE_PASSWORD){
		/* Get the new password from HMI_ECU and save it in the External EEPROM */
		getAndSavePassword();
		_delay_ms(10);
	}
}

/*
 * Description :
 * Timer callback function that increments the global ticks variable every second.
 */
void countOneSecond(void) {
	/* Increment ticks every 1 second */
	g_ticks++;
}

/*
 * Description :
 * Function to delay for a specified time in seconds using Timer1.
 */
void Timer1_DelaySecond(uint8 time) {
	Timer_ConfigType timerConfig = {0, 31249, TIMER1, CLOCK_256, COMPARE_MODE};
	Timer_setCallBack(countOneSecond, TIMER1);
	/* Passing the configuration to initialize TIMER1 */
	Timer_init(&timerConfig);
	/* Wait for the specified number of seconds */
	while (g_ticks < time);
	/* Reset the global ticks variable */
	g_ticks = 0;
	/* Stop the Timer */
	Timer_deInit(TIMER1);
}
