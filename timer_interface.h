/*******************************************************************************
* Title                 :   Timer Interface
* Filename              :   timer_interface.h
* Author                :   Marko Galevski
* Origin Date           :   15/01/2020
* Version               :   1.0.0
* Compiler              :   None
* Target                :   None
* Notes                 :   None
*
*
*******************************************************************************/
/****************************************************************************
* Doxygen C Template
* Copyright (c) 2013 - Jacob Beningo - All Rights Reserved
*
* Feel free to use this Doxygen Code Template at your own risk for your own
* purposes.  The latest license and updates for this Doxygen C template can be
* found at www.beningo.com or by contacting Jacob at jacob@beningo.com.
*
* For updates, free software, training and to stay up to date on the latest
* embedded software techniques sign-up for Jacobs newsletter at
* http://www.beningo.com/814-2/
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Template.
*
*****************************************************************************/

/** @file timer_interface.h
 *  @brief General interface covering user accesses to a timer's timebase functionality
 */
/******************************************************************************
* Includes
*******************************************************************************/
#ifndef _TIMER_H
#define _TIMER_H

#include "timer_stm32f411_config.h"
#include <stdint.h>
#include <assert.h>

/**
 * DISABLED and ENABLED macros find their way wherever only a single bit is required
 * to define a mode. Will probalby be phased out for small typedefs in the future.
 */
#ifndef DISABLED
#define DISABLED 0
#endif

/**
 * DISABLED and ENABLED macros find their way wherever only a single bit is required
 * to define a mode. Will probalby be phased out for small typedefs in the future.
 */
#ifndef ENABLED
#define ENABLED 1
#endif

/**
 * Universal start/stop signal. Won't change from platform to platform, although
 * the underlying code might
 */
typedef enum
{
	TIMER_STOP, /**< Stops the timer*/
	TIMER_START /**< Starts the timer*/
} timer_control_t;

/**
 * Universal interrupt enable/disable signal. Won't change from platform to platform
 */
typedef enum
{
	INTERRUPT_DISABLE, 	/**< Disables the selected interrupt */
	INTERRUPT_ENABLE	/**> Enables the selected interrupt */
} timer_interrupt_control_t;

void timer_init(timer_config_t *config_table);
void timer_control(timer_t timer, timer_control_t signal);
uint32_t timer_read(timer_t timer);
void timer_prescaler_set(timer_t timer, timer_prescaler_t prescaler);
timer_prescaler_t timer_prescaler_get(timer_t timer);
void timer_interrupt_control(timer_t timer, timer_interrupt_t interrupt, timer_interrupt_control_t signal);

void timer_register_write(uint32_t timer_register, uint32_t value);
uint32_t timer_register_read(uint32_t timer_register);



#endif
