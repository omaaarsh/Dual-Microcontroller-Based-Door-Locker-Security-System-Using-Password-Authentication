/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR Timer driver
 *
 * Author: Omar Sherif
 *
 *******************************************************************************/

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile void (*g_Timer0_CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_CallBackPtr)(void) = NULL_PTR;

void Timer_init(const Timer_ConfigType * Config_Ptr)
{
    switch(Config_Ptr->timer_ID)
    {
        case TIMER0:
            /* Set initial value */
            TCNT0 = Config_Ptr->timer_InitialValue;

            if (Config_Ptr->timer_mode == NORMAL_MODE)
            {
                /* Normal Mode */
                TCCR0 = (1 << FOC0) | (Config_Ptr->timer_clock);
                TIMSK |= (1 << TOIE0); // Enable overflow interrupt
            }
            else if (Config_Ptr->timer_mode == COMPARE_MODE)
            {
                /* Compare Mode */
                TCCR0 = (1 << FOC0) | (1 << WGM01) | (Config_Ptr->timer_clock);
                OCR0 = Config_Ptr->timer_compare_MatchValue;
                TIMSK |= (1 << OCIE0); // Enable compare interrupt
            }
            break;

        case TIMER1:
            /* Set initial value */
            TCNT1 = Config_Ptr->timer_InitialValue;

            if (Config_Ptr->timer_mode == NORMAL_MODE)
            {
                /* Normal Mode */
                TCCR1A = (1 << FOC1A) | (1 << FOC1B);
                TCCR1B = Config_Ptr->timer_clock;
                TIMSK |= (1 << TOIE1); // Enable overflow interrupt
            }
            else if (Config_Ptr->timer_mode == COMPARE_MODE)
            {
                /* Compare Mode */
                TCCR1A = (1 << FOC1A) | (1 << FOC1B);
                TCCR1B = (1 << WGM12) | (Config_Ptr->timer_clock);
                OCR1A = Config_Ptr->timer_compare_MatchValue;
                TIMSK |= (1 << OCIE1A); // Enable compare interrupt
            }
            break;

        case TIMER2:
            /* Set initial value */
            TCNT2 = Config_Ptr->timer_InitialValue;

            if (Config_Ptr->timer_mode == NORMAL_MODE)
            {
                /* Normal Mode */
                TCCR2 = (1 << FOC2) | (Config_Ptr->timer_clock);
                TIMSK |= (1 << TOIE2); // Enable overflow interrupt
            }
            else if (Config_Ptr->timer_mode == COMPARE_MODE)
            {
                /* Compare Mode */
                TCCR2 = (1 << FOC2) | (1 << WGM21) | (Config_Ptr->timer_clock);
                OCR2 = Config_Ptr->timer_compare_MatchValue;
                TIMSK |= (1 << OCIE2); // Enable compare interrupt
            }
            break;
    }
}

void Timer_deInit(Timer_ID_Type timer_type)
{
    switch(timer_type)
    {
        case TIMER0:
            TCCR0 = 0;
            TIMSK &= ~(1 << TOIE0) & ~(1 << OCIE0);
            break;
        case TIMER1:
            TCCR1A = 0;
            TCCR1B = 0;
            TIMSK &= ~(1 << TOIE1) & ~(1 << OCIE1A);
            break;
        case TIMER2:
            TCCR2 = 0;
            TIMSK &= ~(1 << TOIE2) & ~(1 << OCIE2);
            break;
    }
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
    switch(a_timer_ID)
    {
        case TIMER0:
            g_Timer0_CallBackPtr = a_ptr;
            break;
        case TIMER1:
            g_Timer1_CallBackPtr = a_ptr;
            break;
        case TIMER2:
            g_Timer2_CallBackPtr = a_ptr;
            break;
    }
}

/*******************************************************************************
 *                      Interrupt Service Routines                             *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
    if(g_Timer0_CallBackPtr != NULL_PTR)
    {
        (*g_Timer0_CallBackPtr)();
    }
}

ISR(TIMER0_COMP_vect)
{
    if(g_Timer0_CallBackPtr != NULL_PTR)
    {
        (*g_Timer0_CallBackPtr)();
    }
}

ISR(TIMER1_OVF_vect)
{
    if(g_Timer1_CallBackPtr != NULL_PTR)
    {
        (*g_Timer1_CallBackPtr)();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if(g_Timer1_CallBackPtr != NULL_PTR)
    {
        (*g_Timer1_CallBackPtr)();
    }
}

ISR(TIMER2_OVF_vect)
{
    if(g_Timer2_CallBackPtr != NULL_PTR)
    {
        (*g_Timer2_CallBackPtr)();
    }
}

ISR(TIMER2_COMP_vect)
{
    if(g_Timer2_CallBackPtr != NULL_PTR)
    {
        (*g_Timer2_CallBackPtr)();
    }
}
