/*
 ================================================================================================
 Name        : HMI_ECU.c
 Author      : Omar Sherif
 Description : HMI_ECU
 ================================================================================================
 */

#include "UART.h"
#include "LCD.h"
#include "Keypad.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_SIZE        5
#define MAX_TRIES            3
#define TIMER_DELAY          15  // Delay in seconds

// Control Command Definitions
#define PASSWORD_SAVED       0x12
#define TRUE_PASSWORD        0x14
#define WRONG_PASSWORD       0x15
#define UNLOCK_DOOR          0x18
#define LOCKING_DOOR         0x17
#define CHANGE_PASSWORD      0x20
#define CONTROL_ECU_READY    0x16

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static volatile uint8 g_ticks = 0;
/* Create configuration structure for timer driver */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void createPassword(void);
void getPassword(uint8* pass, uint8 size);
uint8 checkPassword(void);
void alarmMode(void);
void Timer1_DelaySecond(uint8 time);
void countOneSecond(void);
void displayDoorOptions(void);
void handleDoorUnlock(void);
void handlePasswordChange(void);

/*******************************************************************************
 *                                    Main                                     *
 *******************************************************************************/

int main(void) {
    uint8 key;

    UART_ConfigType uartConfig = {EIGHT_BITS, NO_PARITY, ONE_STOP_BIT, 9600};
    sei();  // Enable Global Interrupt
    UART_init(&uartConfig);  // Initialize UART
    LCD_init();  // Initialize LCD

    LCD_displayString("Door Lock System");
    _delay_ms(500);
    createPassword();  // Create initial password
    LCD_clearScreen();

    for (;;) {
        displayDoorOptions();

        key = KEYPAD_getPressedKey();
        _delay_ms(500);

        if (key == '+') {
            handleDoorUnlock();
        } else if (key == '-') {
            handlePasswordChange();
        }
    }
}

/*******************************************************************************
 *                         Functions Definitions                               *
 *******************************************************************************/

/* Display options for the user to interact with the door system */
void displayDoorOptions(void) {
    LCD_displayString("+ : Open Door");
    LCD_displayStringRowColumn(1, 0, "- : Change Pass");
}

/* Handle door unlocking process */
void handleDoorUnlock(void) {
    uint8 isPassTrue = checkPassword();

    if (isPassTrue == TRUE_PASSWORD) {
        UART_sendByte(UNLOCK_DOOR);  // Send unlock signal
        LCD_clearScreen();
        LCD_displayString("Door Unlocking");
        LCD_displayStringRowColumn(1, 0, "Please wait...");
        Timer1_DelaySecond(TIMER_DELAY);
        LCD_clearScreen();
        LCD_displayString("Wait for people");
        LCD_displayStringRowColumn(1, 0, "to enter");

        // Wait for the door locking signal
        while (UART_recieveByte() != LOCKING_DOOR)
            _delay_ms(50);

        LCD_clearScreen();
        LCD_displayStringRowColumn(0, 0, "Door Locked");
        Timer1_DelaySecond(TIMER_DELAY);
        LCD_clearScreen();
    } else if (isPassTrue == WRONG_PASSWORD) {
        alarmMode();
    }
}

/* Handle password change process */
void handlePasswordChange(void) {
    uint8 isPassTrue = checkPassword();

    if (isPassTrue == TRUE_PASSWORD) {
        UART_sendByte(CHANGE_PASSWORD);
        createPassword();
        LCD_clearScreen();
    } else if (isPassTrue == WRONG_PASSWORD) {
    		alarmMode();
    }
}

/* Check the entered password against the saved password */
uint8 checkPassword(void) {
    uint8 pass[PASSWORD_SIZE + 2];
    uint8 attempts;

    for (attempts = 0; attempts < MAX_TRIES; attempts++) {
        LCD_clearScreen();
        LCD_displayString("Enter Password:");
        LCD_moveCursor(1, 0);

        getPassword(pass, PASSWORD_SIZE + 2);  // Capture user input
        while (KEYPAD_getPressedKey() != '=');
        _delay_ms(500);

        while (UART_recieveByte() != CONTROL_ECU_READY);
        UART_sendString(pass);  // Send password for verification

        uint8 flag = UART_recieveByte();
        if (flag == TRUE_PASSWORD) {
            return TRUE_PASSWORD;
        }// Correct password
    }
    return WRONG_PASSWORD;  // If max attempts reached
}

/* Alarm Mode - Locks system for 1 minute after 3 failed password attempts */
void alarmMode(void) {
    LCD_clearScreen();
    LCD_displayString("System LOCKED");
    LCD_displayStringRowColumn(1, 0, "Wait for 1 min");
    Timer1_DelaySecond(60);  // Lock the system for 60 seconds
    LCD_clearScreen();
}

/* Create a new password (new password is confirmed by re-entering) */
void createPassword(void) {
    uint8 pass1[PASSWORD_SIZE + 2], pass2[PASSWORD_SIZE + 2];
    uint8 isSaved;

    for (;;) {
        LCD_clearScreen();
        LCD_displayStringRowColumn(0, 0, "Enter New Pass: ");
        LCD_moveCursor(1, 0);

        getPassword(pass1, PASSWORD_SIZE + 2);
        while (KEYPAD_getPressedKey() != '=');
        _delay_ms(500);

        LCD_clearScreen();
        LCD_displayStringRowColumn(0, 0, "Re-enter Pass: ");
        LCD_moveCursor(1, 0);
        getPassword(pass2, PASSWORD_SIZE + 2);
        while (KEYPAD_getPressedKey() != '=');
        _delay_ms(500);

        while (UART_recieveByte() != CONTROL_ECU_READY);
        UART_sendString(pass1);
        UART_sendString(pass2);

        isSaved = UART_recieveByte();
        if (isSaved == PASSWORD_SAVED) {
        	LCD_clearScreen();
        	LCD_displayStringRowColumn(0, 0, "successfully");
        	_delay_ms(500);
            return;  // Return if password is successfully saved
        }
    	LCD_clearScreen();
    	LCD_displayStringRowColumn(0, 0, "Mismatch");
    	_delay_ms(500);
    }
}

/* Capture password input from user and append necessary characters */
void getPassword(uint8* pass, uint8 size) {
    uint8 i;
    for (i = 0; i < size - 2; i++) {
        pass[i] = KEYPAD_getPressedKey() + 48;  // Convert to ASCII
        LCD_displayCharacter('*');
        _delay_ms(500);
    }
    pass[i++] = '#';
    pass[i] = '\0';
}

/* Timer callback function to count 1 second */
void countOneSecond(void) {
    g_ticks++;  // Increment ticks every 1 second
}

/* Delay function to wait for a specified number of seconds */
void Timer1_DelaySecond(uint8 time) {
    Timer_ConfigType timerConfig = {0, 31249, TIMER1, CLOCK_256, COMPARE_MODE};
    Timer_setCallBack(countOneSecond, TIMER1);
    Timer_init(&timerConfig);  // Initialize timer

    // Wait for the specified time (in seconds)
    while (g_ticks < time);
    g_ticks = 0;  // Reset tick counter
    Timer_deInit(TIMER1);  // Stop the timer
}
