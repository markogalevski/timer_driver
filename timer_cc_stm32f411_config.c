/*******************************************************************************
* Title                 :   Timer Capture and Compare STM32F411 Configuration Data
* Filename              :   timer_cc_stm32f411_config.c
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

/** @file timer_cc_stm32f411_config.c
 *  @brief Chip specific configuration table for each planned CC channel
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "timer_cc_stm32f411_config.h"
/**
 * Configuration table containing settings for every CCR channel
 */
static const timer_cc_config_t config_table[NUM_CCRS] =
{
	/*TIM1 CCR1*/   {},
	/*TIM1 CCR2*/	{},
	/*TIM1 CCR3*/	{},
	/*TIM1 CCR4*/	{},

	/*TIM2 CCR1*/	{},
	/*TIM2 CCR2*/	{},
	/*TIM2 CCR3*/	{},
	/*TIM2 CCR4*/	{},

	/*TIM3 CCR1*/	{},
	/*TIM3 CCR2*/	{},
	/*TIM3 CCR3*/	{},
	/*TIM3 CCR4*/	{},

	/*TIM4 CCR1*/	{},
	/*TIM4 CCR2*/	{},
	/*TIM4 CCR3*/	{},
	/*TIM4 CCR4*/	{},

	/*TIM5 CCR1*/	{},
	/*TIM5 CCR1*/	{},
	/*TIM5 CCR1*/	{},
	/*TIM5 CCR1*/	{},

	/*TIM9 CCR1*/	{},
	/*TIM9 CCR2*/	{},

	/*TIM10 CCR1*/	{},

	/*TIM11 CCR1*/  {}
};

/******************************************************************************
* Function: timer_cc_config_get()
*//**
* \b Description:
*
* This function is used to obtain the configuration data for the CC channels of the timers
*
* PRE-CONDITION: Configuration table needs to populated (sizeof > 0) <br>
*
* POST-CONDITION: The timer CC channels are configured and ready for use.
*
*
* @return 		const timer_cc_config_t *
*
* \b Example:
* @code
* 	const timer_config_t *timer_config = timer_config_get();
* 	timer_init(timer_config);
* 	const timer_cc_config_t *timer_cc_config = timer_cc_config_get();
*	timer_cc_init(timer_cc_config);
* @endcode
*
* @see timer_init
* @see timer_config_get
* @see timer_cc_init
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
const timer_cc_config_t *timer_cc_config_get(void)
{
	return(config_table);
}
