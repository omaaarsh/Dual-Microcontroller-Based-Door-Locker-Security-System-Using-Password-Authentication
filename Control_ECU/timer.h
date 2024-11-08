/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR Timer driver
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                      Types Definitions                                      *
 *******************************************************************************/

typedef enum {
    TIMER0,
    TIMER1,
    TIMER2
} Timer_ID_Type;

typedef enum {
    NO_CLOCK, CLOCK_1, CLOCK_8, CLOCK_64, CLOCK_256, CLOCK_1024
} Timer_ClockType;

typedef enum {
    NORMAL_MODE, COMPARE_MODE
} Timer_ModeType;

typedef struct {
    uint16 timer_InitialValue;
    uint16 timer_compare_MatchValue; /* Used in compare mode only */
    Timer_ID_Type timer_ID;
    Timer_ClockType timer_clock;
    Timer_ModeType timer_mode;
} Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Timer_init(const Timer_ConfigType * Config_Ptr);
void Timer_deInit(Timer_ID_Type timer_type);
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_H_ */
