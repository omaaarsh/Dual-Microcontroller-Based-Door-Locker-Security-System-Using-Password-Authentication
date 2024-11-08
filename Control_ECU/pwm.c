/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: Source file for the ATmega32 PWM driver using Timer0
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#include "avr/io.h"
#include "common_macros.h" /* To use macros like SET_BIT */
#include "pwm.h"

/******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void PWM_Timer0_Start(uint8 duty_cycle)
{
    /* Set OC0 pin (PB3) as output pin */
    SET_BIT(DDRB, PB3);  // OC0 pin is PB3 on ATmega32

    /*
     * Configure Timer0 for Fast PWM mode:
     * WGM00 = 1, WGM01 = 1 for Fast PWM mode
     * COM01 = 1 for Non-inverting mode (output on OC0)
     * COM00 = 0 (no toggle on compare match)
     *
     * Set prescaler to F_CPU/64
     * CS02 = 0, CS01 = 1, CS00 = 1
     */
    TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS01) | (1 << CS00);

    /* Set duty cycle based on the input percentage (0-100%) */
    OCR0 = (duty_cycle * 255) / 100; // Convert percentage to 8-bit value (0-255)
}
