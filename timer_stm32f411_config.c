/*******************************************************************************
* Title                 :   Timer Config Table (STM32F411 implementation)
* Filename              :   timer_stm32f411_config.c
* Author                :   Marko Galevski
* Origin Date           :   15/01/2020
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

/** @file timer_stm32f411_config.c
 *  @brief Collection of configuration tables used to configure a timer. Config_table
 *  is the one you'll probably need the most. The _advanced and _trigger tables
 *  are only used in their respective contexts, and will remain empty unless using
 *  external triggering, one shot mode, or disabling update events.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "timer_stm32f411_config.h"

/**
 * Array containing structures configuring the characteristics of
 * external triggering for a particular timer. Look into the chapters
 * regarding timers in RM0383 for more info.
 */
static const timer_external_trigger_t config_trigger[NUM_TIMERS] =
{				//FILTER		//PRESCALER			//POLARITY
	/*TIM1*/	{},
	/*TIM2*/	{},
	/*TIM3*/	{},
	/*TIM4*/	{},
	/*TIM5*/	{},
	/*TIM9*/	{},
	/*TIM10*/	{},
	/*TIM11*/	{}
};

/**
 * Array containing config data for update event disabling, one shot mode,
 * and external triggering.
 */
static const timer_advanced_t config_advanced[NUM_TIMERS] =
{
				//ALT_CLOCK_SOURCE		//OPM		//UDIS		//SLAVE MODE    	//TRIGGER		//MASTER/SLAVE			//EXTERNAL

/*TIM1_ADV*/	{INTERNAL_CLOCK, TIMER_OPM_DISABLED, TIMER_UDIS_DISABLED, SLAVE_MODE_DISABLED, TI1_EDGE_DETECTOR, MASTER_SLAVE_DISABLED, &config_trigger[0]},
/*TIM2_ADV*/	{},
/*TIM3_ADV*/	{},
/*TIM4_ADV*/	{},
/*TIM5_ADV*/	{},
/*TIM9_ADV*/	{},
/*TIM10_ADV*/	{},
/*TIM11_ADV*/	{}
};

/**
 * Main config array. Used to configure the clock source, tick characteristics, prescaler, autoreload, etc.
 * @see timer_config_t
 * for more details
 */
static const timer_config_t config_table[NUM_TIMERS] =
{ 			//ALIGNMENT	  	//DIRECTION	   //PRESCALER	 //AUTORELOAD	//AUTO_RELOAD BUFFER		//ADVANCED PTR
/*TIM1*/	{EDGE_ALIGNED,	UPCOUNTER,   	400,	  	16,			1,    &config_advanced[0]},
/*TIM2*/	{},
/*TIM3*/	{},
/*TIM4*/	{},
/*TIM5*/	{},
/*TIM9*/	{},
/*TIM10*/	{},
/*TIM11*/	{}
};


/******************************************************************************
 * Function : timer_config_get()
 *//**
 * \b Description:
 *	Retrieves the config table for the timer peripheral, normally hidden statically within the
 *	config.c file.
 *
 * PRE-CONDITION: The config table has been populated/exists with a size greater than 0.
 *
 * POST-CONDITION: The returned value points to the base of the config table
 *
 * @return			const timer_config_t *
 *
 *
 * \b Note:
 * 	To configure a timer, visit the config_table array in timer_stm32f411_conig.c and set all the elements
 * 	 of the corresponding structure. e.g.
 * @code
 * static const timer_config_t config_table[NUM_TIMERS] =
 * {
 * ...
 * TIM4 {EXTERNAL_MODE1, ENCODER_MODE_1, CENTER_ALIGNED_3, 0, 15934, 775, ENABLED, 0},
 * ...
 * }
 * @endcode
 * \b Example:
 *
 * @code
 * const timer_config_t *timer_config_table = timer_config_get(void);
 * timer_init(timer_config_table);
 * @endcode
 *
 * @see timer_init
 *
 * <br><b> - CHANGE HISTORY - </b>
 *
 * <table align="left" style="width:800px">
 * <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
 * </table><br><br>
 * <hr>
 *
 */
const timer_config_t *timer_config_get(void)
{
	return(config_table);
}
