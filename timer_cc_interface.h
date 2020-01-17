/*******************************************************************************
* Title                 :   Timer Capture and Compare Interface
* Filename              :   timer_cc_interface.h
* Author                :   Marko Galevski
* Origin Date           :   17/01/2020
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

/** @file timer_cc_interface.h
 *  @brief General interface covering user accesses to a timer's capture and
 *  		compare functionality
 */
/******************************************************************************
* Includes
*******************************************************************************/
#ifndef _TIMER_CC_H
#define _TIMER_CC_H

#include "timer_cc_stm32f411_config.h"
#include <stdint.h>

void timer_cc_init(timer_cc_config_t *cc_config_table);
void timer_cc_control(timer_cc_t timer_cc, timer_control_t signal);
uint32_t timer_cc_read(timer_cc_t timer_cc);
void timer_cc_write(timer_cc_t timer_cc, uint32_t value);

void timer_cc_pwm_duty_cycle_set(timer_cc_t timer_cc, uint32_t duty_cycle);

#endif /* TIMER_CC interface */
