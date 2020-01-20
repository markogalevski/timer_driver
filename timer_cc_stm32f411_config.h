/*******************************************************************************
* Title                 :   Timer Capture and Compare Config Header for STM32F411
* Filename              :   timer_cc_stm32f411_config.h
* Author                :   Marko Galevski
* Origin Date           :   17/01/2020
* Version               :   1.0.0
* Compiler              :   GCC
* Target                :   STM32F411VE (ARM Cortex M4)
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

/** @file timer_cc_stm32f411_config.h
 *  @brief Chip specific header containing requisite enums and structs for
 *  		proper configuration
 */
/******************************************************************************
* Includes
*******************************************************************************/
#ifndef _TIMER_STM32F411_CC_CONFIG
#define _TIMER_STM32F411_CC_CONFIG

#include "stm32f411xe.h"
#include <stdint.h>
#include "timer_stm32f411_config.h"

/**
 * Contains all of the capture and compare channel on chip
 */
typedef enum
{
	/*TIM1*/
	TIMER1_CCR1,
	TIMER1_CCR2,
	TIMER1_CCR3,
	TIMER1_CCR4,
	/*TIM2*/
	TIMER2_CCR1,
	TIMER2_CCR2,
	TIMER2_CCR3,
	TIMER2_CCR4,
	/*TIM3*/
	TIMER3_CCR1,
	TIMER3_CCR2,
	TIMER3_CCR3,
	TIMER3_CCR4,
	/*TIM4*/
	TIMER4_CCR1,
	TIMER4_CCR2,
	TIMER4_CCR3,
	TIMER4_CCR4,
	/*TIM5*/
	TIMER5_CCR1,
	TIMER5_CCR2,
	TIMER5_CCR3,
	TIMER5_CCR4,
	/*TIM9*/
	TIMER9_CCR1,
	TIMER9_CCR2,
	/*TIM10*/
	TIMER10_CCR1,
	/*TIM11*/
	TIMER11_CCR1,

	NUM_CCRS
}timer_cc_t;

/**
 * Contains the modes a cc channel can be in
 */
typedef enum
{
	CC_OUTPUT, 				/**< The CC channel acts as an output */
	CC_INPUT_TI_SELF,		/**< The CC channel acts an input, which uses the signal
								from the same channel. e.g TIM3_CH pin (TI3) is processed,
								and then used within CC3 operations */
	CC_INPUT_TI_OPPOSITE,	/**< The CC channel acts as an input, which uses the signal from
								its partner channel. e.g. TIM3_CH pin (TI3) is processed,
								and used by CC4 for its operations */
	CC_INPUT_TRC			/**< The CC channel acts as an input, and uses the trigger
									defined in timer_trigger_t */
}timer_cc_mode_t;

/**
 * Contains self explanatory polarity options for the output of a particular channel
 */
typedef enum
{
	OUTPUT_ACTIVE_HIGH,
	OUTPUT_ACTIVE_LOW
}timer_cc_output_polarity_t;

/**
 * Contains options for output fast enable, an option which allows outputs
 * to respond faster (3 vs 5 clock cycles) to a trigger event in PWM mode.
 */
typedef enum
{
	CC_FAST_ENABLE_OFF,
	CC_FAST_ENABLE_ON
} timer_cc_output_fe_t;

/**
 * Contains options for output preload buffering. Works identically to auto-reload preload.
 * When enabled, writes to TIMx_CCRy are only reflected after an update event
 */
typedef enum
{
	CC_PRELOAD_BUFFER_DISABLED,
	CC_PRELOAD_BUFFER_ENABLED
}timer_cc_output_pe_t;

/**
 * Contains the options for output clearing. When activated, the output channel immediately
 * drops to its inactive level upon an ETRF input
 */
typedef enum
{
	CC_OUTPUT_CLEAR_DISABLED,
	CC_OUTPUT_CLEAR_EDNABLED
}timer_cc_output_ce_t;

/**
 * Contains the various output modes a CC channel can be in.
 */
typedef enum
{
	FROZEN,			/**<The cc output pin doesn't respond to matches between TIMx_CNT and TIMx_CCRy*/
	ACTIVE_MATCH,	/**<The cc output pin is driven to its active level upon a CNT & CCRy match*/
	INACTIVE_MATCH,	/**<The cc output pin is driven to its inactive level upon a CNT & CCRy match*/
	TOGGLE_MATCH,	/**<The cc output pin's state is toggled upon CNT & CCRy matches */
	FORCE_INACTIVE,	/**<The cc output pin is forced to its inactive level*/
	FORCE_ACTIVE,	/**<The cc output pin is forced to its active level */
	PWM_MODE_ONE, 	/**<The cc output pin operates in PWM Non-Inverted Duty Cycles*/
	PWM_MODE_TWO 	/**<The cc outptu pin operates in PWM Inverted Duty Cycles */
}timer_cc_output_mode_t;

/**
 * Contains the options for the input capture channels edge detection unit.
 */
typedef enum
{
	RISING_EDGE,	/**< The TIMx_CCy edge detector (ED) responds to rising edges in TIy*/
	FALLING_EDGE,	/**< The TIMx_CCy edge detector (ED) responds to falling edges in TIy*/
	BOTH_EDGES		/**< The TIMx_CCy edge detector (ED) responds to both edges in TIy*/
}timer_cc_input_polarity_t;

/**
 * Contains the options for the sampling frequency and consecutive event (low pass) filter
 * for the TIy input pin. See @see timer_external_trigger_filter_t for the options
 */
typedef timer_external_trigger_filter_t timer_cc_input_filter_t;

/**
 * Contains the options for prescaling of the filtered events
 */
typedef enum
{
	CAPTURE_DIV_1, 	/**< A capture occurs every detected event */
	CAPTURE_DIV_2,	/**< A capture occurs every second event */
	CAPTURE_DIV_4,	/**< A capture occurs every fourth event */
	CAPTURE_DIV_8	/**< A capture occurs every eigth event */
}timer_cc_input_prescaler_t;

/**
 * Dual-purpose CC config structure which is parsed differently depending
 * on the cc_mode member.
 */
typedef struct
{
	timer_cc_mode_t cc_mode;
	/*********OUTPUT MODE*********/
	timer_cc_output_polarity_t output_polarity;
	timer_cc_output_fe_t output_fast_enable;
	timer_cc_output_pe_t output_preload_enable;
	timer_cc_output_mode_t output_mode;
	timer_cc_output_ce_t output_clear_enable;
	/*******END OUTPUT MODE*******/

	/*********INPUT MODE**********/
	timer_cc_input_prescaler_t input_event_prescaler;
	timer_cc_input_filter_t input_event_filter;
	timer_cc_input_polarity_t input_polarity;
	/*******END INPUT MODE*******/

}timer_cc_config_t;

const timer_cc_config_t *timer_cc_config_get(void);

#endif
