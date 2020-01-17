/*******************************************************************************
* Title                 :   Timer Implementation for STM32F411
* Filename              :   timer_stm32f411.c
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

/** @file timer_stm32f411.c
 *  @brief Microcontroller specific implementation of timer functionality
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "timer_interface.h"
#include "timer_cc_interface.h" /* Only included on MCUs with CC timers */
#include "stm32f411xe.h"

#ifndef NULL
#define NULL (void *)0
#endif


/**
 *	Array of poiners to Control Register 1 registers
 */
static volatile uint32_t *const TIM_CR1[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE, (uint32_t *)TIM2_BASE, (uint32_t *)TIM3_BASE,
	(uint32_t *)TIM4_BASE, (uint32_t *)TIM5_BASE, (uint32_t *)TIM9_BASE,
	(uint32_t *)TIM10_BASE, (uint32_t *)TIM11_BASE
};

/**
 * Array of pointers to Control Register 2 registers
 */
static volatile uint32_t *const TIM_CR2[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x04UL, (uint32_t *)TIM2_BASE + 0x04UL,
	(uint32_t *)TIM3_BASE + 0x04UL, (uint32_t *)TIM4_BASE + 0x04UL,
	(uint32_t *)TIM5_BASE + 0x04UL,
	(uint32_t *) NULL, (uint32_t *) NULL, (uint32_t *) NULL
};

/**
 * Array of pointers to Slave Mode Config registers
 */
static volatile uint32_t *const TIM_SMCR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x08UL, (uint32_t *)TIM2_BASE + 0x08UL,
	(uint32_t *)TIM3_BASE + 0x08UL, (uint32_t *)TIM4_BASE + 0x08UL,
	(uint32_t *)TIM5_BASE + 0x08UL, (uint32_t *)TIM9_BASE + 0x08UL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

/**
 * Array of pointers to DMA and Interrupt Enable registers
 */
static volatile uint32_t *const TIM_DIER[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x0CUL, (uint32_t *)TIM2_BASE + 0x0CUL,
	(uint32_t *)TIM3_BASE + 0x0CUL, (uint32_t *)TIM4_BASE + 0x0CUL,
	(uint32_t *)TIM5_BASE + 0x0CUL, (uint32_t *)TIM9_BASE + 0x0CUL,
	(uint32_t *)TIM10_BASE + 0x0CUL, (uint32_t *)TIM11_BASE + 0x0CUL
};

/**
 * Array of pointers to Status registers
 */
static volatile uint32_t *const TIM_SR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x10UL, (uint32_t *)TIM2_BASE + 0x10UL,
	(uint32_t *)TIM3_BASE + 0x10UL, (uint32_t *)TIM4_BASE + 0x10UL,
	(uint32_t *)TIM5_BASE + 0x10UL, (uint32_t *)TIM9_BASE + 0x10UL,
	(uint32_t *)TIM10_BASE + 0x10UL, (uint32_t *)TIM11_BASE + 0x10UL
};

/**
 * Array of pointers to Event Generation registers
 */
static volatile uint32_t *const TIM_EGR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x14UL, (uint32_t *)TIM2_BASE + 0x14UL,
	(uint32_t *)TIM3_BASE + 0x14UL, (uint32_t *)TIM4_BASE + 0x14UL,
	(uint32_t *)TIM5_BASE + 0x14UL, (uint32_t *)TIM9_BASE + 0x14UL,
	(uint32_t *)TIM10_BASE + 0x14UL, (uint32_t *)TIM11_BASE + 0x14UL
};

/**
 * Array of pointers to Capture and Compare Mode Register 1 registers
 */
static volatile uint32_t *const TIM_CCMR1[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x18UL, (uint32_t *)TIM2_BASE + 0x18UL,
	(uint32_t *)TIM3_BASE + 0x18UL, (uint32_t *)TIM4_BASE + 0x18UL,
	(uint32_t *)TIM5_BASE + 0x18UL, (uint32_t *)TIM9_BASE + 0x18UL,
	(uint32_t *)TIM10_BASE + 0x18UL, (uint32_t *)TIM11_BASE + 0x18UL
};

/**
 * Array of pointers to Capture and Compare Mode Register 2 registers
 */
static volatile uint32_t *const TIM_CCMR2[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x1CUL, (uint32_t *)TIM2_BASE + 0x1CUL,
	(uint32_t *)TIM3_BASE + 0x1CUL, (uint32_t *)TIM4_BASE + 0x1CUL,
	(uint32_t *)TIM5_BASE + 0x1CUL, (uint32_t *)NULL,
	(uint32_t *)NULL, (uint32_t *)NULL
};

/**
 *	Array of pointers to Capture and Compare Enable regsiters
 */
static volatile uint32_t *const TIM_CCER[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x20UL, (uint32_t *)TIM2_BASE + 0x20UL,
	(uint32_t *)TIM3_BASE + 0x20UL, (uint32_t *)TIM4_BASE + 0x20UL,
	(uint32_t *)TIM5_BASE + 0x20UL, (uint32_t *)TIM9_BASE + 0x20UL,
	(uint32_t *)TIM10_BASE + 0x20UL, (uint32_t *)TIM11_BASE + 0x20UL
};

/**
 * Array of pointers to Count registers
 */
static volatile uint32_t *const TIM_CNT[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x24UL, (uint32_t *)TIM2_BASE + 0x24UL,
	(uint32_t *)TIM3_BASE + 0x24UL, (uint32_t *)TIM4_BASE + 0x24UL,
	(uint32_t *)TIM5_BASE + 0x24UL, (uint32_t *)TIM9_BASE + 0x24UL,
	(uint32_t *)TIM10_BASE + 0x24UL, (uint32_t *)TIM11_BASE + 0x24UL
};

/**
 * Array of pointers to Prescaler registers
 */
static volatile uint16_t *const TIM_PSC[NUM_TIMERS] =
{
	(uint16_t *)TIM1_BASE + 0x28UL, (uint16_t *)TIM2_BASE + 0x28UL,
	(uint16_t *)TIM3_BASE + 0x28UL, (uint16_t *)TIM4_BASE + 0x28UL,
	(uint16_t *)TIM5_BASE + 0x28UL, (uint16_t *)TIM9_BASE + 0x28UL,
	(uint16_t *)TIM10_BASE + 0x28UL, (uint16_t *)TIM11_BASE + 0x28UL
};

/**
 * Array of pointers to Auto Reload registers
 */
static volatile uint32_t *const TIM_ARR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x2CUL, (uint32_t *)TIM2_BASE + 0x2CUL,
	(uint32_t *)TIM3_BASE + 0x2CUL, (uint32_t *)TIM4_BASE + 0x2CUL,
	(uint32_t *)TIM5_BASE + 0x2CUL, (uint32_t *)TIM9_BASE + 0x2CUL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

/**
 * Array of registers to Repetition Counter registers
 */
static volatile uint32_t *const TIM_RCR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x30UL, (uint32_t *)NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

/**
 * Composite array of pointers to all Capture and Compare registers
 */
static volatile uint32_t *const TIM_CCR[NUM_CCRS] =
{
	/*TIM1*/ (uint32_t *)TIM1_BASE + 0x34UL, (uint32_t *)TIM1_BASE + 0x38UL,
	(uint32_t *)TIM1_BASE + 0x3CUL, (uint32_t *)TIM1_BASE + 0x40UL,

	/*TIM2*/(uint32_t *)TIM2_BASE + 0x34UL, (uint32_t *)TIM2_BASE + 0x38UL,
	(uint32_t *)TIM2_BASE + 0x3CUL, (uint32_t *)TIM2_BASE + 0x40UL,

	/*TIM3*/(uint32_t *)TIM3_BASE + 0x34UL, (uint32_t *)TIM3_BASE + 0x38UL,
	(uint32_t *)TIM3_BASE + 0x3CUL, (uint32_t *)TIM3_BASE + 0x40UL,

	/*TIM4*/(uint32_t *)TIM4_BASE + 0x34UL, (uint32_t *)TIM4_BASE + 0x38UL,
	(uint32_t *)TIM4_BASE + 0x3CUL, (uint32_t *)TIM4_BASE + 0x40UL,

	/*TIM5*/(uint32_t *)TIM5_BASE + 0x34UL, (uint32_t *)TIM5_BASE + 0x38UL,
	(uint32_t *)TIM5_BASE + 0x3CUL, (uint32_t *)TIM5_BASE + 0x40UL,

	/*TIM9*/(uint32_t *)TIM9_BASE + 0x34UL, (uint32_t *)TIM9_BASE + 0x38UL,

	/*TIM10*/(uint32_t *)TIM10_BASE + 0x34UL,

	/*TIM11*/(uint32_t *)TIM11_BASE + 0x34UL
};

/**
 * Array of pointers to Break and Dead Time registers
 */
static volatile uint32_t *const TIM_BDTR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x44UL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

/**
 * Array of pointers to DMA Control registers
 */
static volatile uint32_t *const TIM_DCR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x48UL, (uint32_t *)TIM2_BASE + 0x48UL,
	(uint32_t *)TIM3_BASE + 0x48UL, (uint32_t *)TIM4_BASE + 0x48UL,
	(uint32_t *)TIM5_BASE + 0x48UL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

/**
 * Array of pointers to DMA Address registers
 */
static volatile uint32_t *const TIM_DMAR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x4C, (uint32_t *)TIM2_BASE + 0x4C,
	(uint32_t *)TIM3_BASE + 0x4C, (uint32_t *)TIM4_BASE + 0x4C,
	(uint32_t *)TIM5_BASE + 0x4C, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

/**
 * Array of pointers to Option registers
 */
static volatile uint32_t *const TIM_OR[NUM_TIMERS] =
{
	(uint32_t *) NULL, (uint32_t *)TIM2_BASE + 0x50UL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *)TIM5_BASE + 0x50UL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) TIM11_BASE + 0x50UL
};


static void timer_init_external_mode_1(timer_t timer, timer_config_t *config);
static void timer_init_external_mode_2(timer_t timer, timer_config_t *config);
static void timer_init_slave_mode(timer_t timer, timer_config_t *config);

/******************************************************************************
* Function: timer_init()
*//**
* \b Description:
*
* This function is used to initialise the timer based on the configuration table
*  defined in the timer_stm32f411_config.c
*
* PRE-CONDITION: Configuration table needs to populated (sizeof > 0) <br>
* PRE-CONDITION (conditional): If using advanced features, the advanced pointer for the appropriate
* 					timer must be non-null
* PRE-CONDITION (conditional): If using external triggers or TI1/TI2 triggers, the gpio pins must
* 								me configured with appropriate AF settings
* PRE-CONDITION (condtional): If using external triggers, the external_trigger pointer in the advanced
* 								structure must be non-null
* PRE-CONDITION: The RCC clocks for all planned timers must be configured and enabled.
*
* POST-CONDITION: The timers are ready for use.
*
* @param  		config_table is a pointer to the configuration table that contains
*				the initialisation structures for each timer.
*
* @return 		void
*
* \b Example:
* @code
* 	const timer_config_t *timer_config = timer_config_get();
*		timer_init(timer_config);
* @endcode
*
* @see timer_config_get
* @see timer_init_external_mode_1
* @see timer_init_external_mode_2
* @see timer_init_slave_mode
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_init(timer_config_t *config_table)
{
	for (int timer; timer < NUM_TIMERS; timer++)
	{
		*TIM_CR1[timer] &= ~(TIM_CR1_CEN_Msk);

		if (sizeof(config_table[timer]) != 0)
		{
			*TIM_SMCR[timer] &= ~(TIM_SMCR_SMS_Msk);



			if (config_table[timer].advanced != NULL)
			{
				if (config_table[timer].advanced->clock_source == EXTERNAL_MODE_1)
				{
					timer_init_external_mode_1(timer, &config_table[timer]);
				}
				else if (config_table[timer].advanced->clock_source == EXTERNAL_MODE_2)
				{
					timer_init_external_mode_2(timer, &config_table[timer]);
				}
				timer_init_slave_mode(timer, &config_table[timer]);
			}

			*TIM_CR1[timer] &= ~(TIM_CR1_CMS_Msk);
			*TIM_CR1[timer] |= (config_table[timer].alignment) << TIM_CR1_CMS_Pos;

			*TIM_CR1[timer] &= ~(TIM_CR1_DIR_Msk);
			*TIM_CR1[timer] |= config_table[timer].direction << TIM_CR1_DIR_Pos;

			*TIM_PSC[timer] = config_table[timer].prescaler;

			*TIM_ARR[timer] = config_table[timer].auto_reload;

			*TIM_CR1[timer] &= ~(TIM_CR1_ARPE_Msk);
			*TIM_CR1[timer] |= (config_table[timer].auto_reload_preload_en) << TIM_CR1_ARPE_Pos;

			*TIM_CR1[timer] &= ~(TIM_CR1_OPM_Msk);
			*TIM_CR1[timer] &= ~(TIM_CR1_UDIS_Msk);
			if (config_table[timer].advanced != NULL)
			{
			*TIM_CR1[timer] |= (config_table[timer].advanced->one_pulse_mode) << TIM_CR1_OPM_Pos;
			*TIM_CR1[timer] |= (config_table[timer].advanced->update_event_dis) << TIM_CR1_UDIS_Pos;
			}
		}
	}

}

/******************************************************************************
* Function: timer_init_external_mode_1()
*//**
* \b Description:
*
* This function is statically called to set the timer's clock source to the selected
* 	on-chip trigger source or TIMx_CH1 (TI1) TIMx_CH2(TI2), as well as changing
* 	the master/slave bit, if so desired.
*
* PRE-CONDITION: The advanced pointer for the appropriate timer must be non-null
* PRE-CONDITION (conditional): If using TI1/TI2 triggers, the gpio pins must
* 								me configured with appropriate AF settings

*
* POST-CONDITION: The timer is now clocked by the selected trigger
*
*
* @param		timer refers to any timer present on-chip
*
* @param  		config_table is a pointer to the config struct that matches
* 					timer
*
* @return 		void
*
* \b Example:
* Automatically called by timer_init if
* @code
*	config_table[timer]->advanced->clock_source == EXTERNAL_MODE_1
* @endcode
*
* @see timer_init
* @see timer_init_external_mode_2
* @see timer_init_slave_mode
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
static void timer_init_external_mode_1(timer_t timer, timer_config_t *config)
{

	*TIM_SMCR[timer] |= TIM_SMCR_SMS_Msk;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_TS_Msk);
	*TIM_SMCR[timer] |= config->advanced->trigger_source << TIM_SMCR_TS_Pos;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_MSM_Msk);
	*TIM_SMCR[timer] |= config->advanced->msm << TIM_SMCR_MSM_Pos;
}

/******************************************************************************
* Function: timer_init_external_mode_2()
*//**
* \b Description:
*
* This function is statically called to set the timer's clock source to the selected
* 	off-chip trigger source (ETR pin).
*
* PRE-CONDITION: The advanced pointer for the appropriate timer must be non-null
* PRE-CONDITION: The external_trgger pointer in advanced must be non-null
* PRE-CONDITION: The desired ETR pin has the correct AF configuration through gpio_init
*
* POST-CONDITION: The timer is now clocked by the selected external trigger with
* 					the desired filtration and sampling characeristics.
*
*
* @param		timer refers to any timer present on-chip
*
* @param  		config_table is a pointer to the config struct that matches
* 					timer
*
* @return 		void
*
* \b Example:
* Automatically called by timer_init if
* @code
*	config_table[timer]->advanced->clock_source == EXTERNAL_MODE_2
* @endcode
*
* @see timer_init
* @see timer_init_external_mode_1
* @see timer_init_slave_mode
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
static void timer_init_external_mode_2(timer_t timer, timer_config_t *config)
{
	assert(config->advanced->external_trigger != NULL);
	*TIM_SMCR[timer] |= TIM_SMCR_ECE_Msk;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_ETP_Msk);
	*TIM_SMCR[timer] |= config->advanced->external_trigger->polarity << TIM_SMCR_ETP_Pos;

	*TIM_CR1[timer] &= ~(TIM_CR1_CKD_Msk);
	*TIM_CR1[timer] |= config->advanced->external_trigger->dts << TIM_CR1_CKD_Pos;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_ETPS_Msk);
	*TIM_SMCR[timer] |= config->advanced->external_trigger->prescaler << TIM_SMCR_ETPS_Pos;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_ETF_Msk);
	*TIM_SMCR[timer] |= config->advanced->external_trigger->filter << TIM_SMCR_ETF_Pos;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_MSM_Msk);
	*TIM_SMCR[timer] |= config->advanced->msm << TIM_SMCR_MSM_Pos;
}

/******************************************************************************
* Function: timer_init_slave_mode()
*//**
* \b Description:
*
* This function is statically called to set the timer's slave behaviour to that defined
* 	in the advanced pointer. Includes various encoder modes, reset, gated, and trigger modes.
*
* \b Note:
*
* 	The encoders' behaviours are hard-coded for the time being. The inclusion of yet another
* 		encoder_settings sub-structure may be necessary, but as of now it's not included.
*
* PRE-CONDITION: The advanced pointer for the appropriate timer must be non-null
* PRE-CONDITION (conditional): If the trigger source is ETRF, the desired ETR pin has
* 						the correct AF configuration through gpio_init
* PRE-CONDITION (conditional): If encoder modes or TI1 and TI2 are explicitly selected as trigger,
* 								the must be correctly AF configured through gpio_init
*
* POST-CONDITION: The timer is now in the desired slave mode.
*
*
* @param		timer refers to any timer present on-chip
*
* @param  		config_table is a pointer to the config struct that matches
* 					timer
*
* @return 		void
*
* \b Example:
* Automatically called by timer_init if
* @code
* 	config->advanced != NULL
* @endcode
*
* @see timer_init
* @see timer_init_external_mode_1
* @see timer_init_external_mode_2
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
static void timer_init_slave_mode(timer_t timer, timer_config_t *config)
{
	if (config->advanced->slave_mode >= ENCODER_MODE_1 && config->advanced->slave_mode <= ENCODER_MODE_3)
		/**< The entire Encoder Mode Behaviour is hard-coded. For flexibility,
		 * read RM0383 12.3.16 and modify as needed
		 * */
	{
		*TIM_CCMR1[timer] |= 0x01UL << TIM_CCMR1_CC1S_Pos;
		*TIM_CCMR1[timer] |= 0x01UL << TIM_CCMR1_CC2S_Pos;

		*TIM_CCER[timer]  &= ~(TIM_CCER_CC1P_Msk | TIM_CCER_CC1NP_Msk
								| TIM_CCER_CC2P_Msk | TIM_CCER_CC2NP_Msk);

		*TIM_CCMR1[timer] &= ~(TIM_CCMR1_IC1F_Msk | TIM_CCMR1_IC2F_Msk);
	}
	else
	{
		*TIM_SMCR[timer] &= ~(TIM_SMCR_TS_Msk);
		*TIM_SMCR[timer] |= config->advanced->trigger_source << TIM_SMCR_TS_Pos;
	}

	*TIM_SMCR[timer] |= config->advanced->slave_mode << TIM_SMCR_SMS_Pos;
}

/******************************************************************************
* Function: timer_control()
*//**
* \b Description:
*
* This function is used to start or stop the counter.
*
* PRE-CONDITION: The timer has been successfully initiated through timer_init()
*
* POST-CONDITION: The timer has started/stop, as per the signal
*
*
* @param		timer refers to any timer present on-chip
*
* @param		signal determines whether the timer stops or starts
*
* @return 		void
*
* \b Example:
* @code
* 	timer_init(config);
* 	timer_control(TIMER4, TIMER_START);
* @endcode
*
* @see timer_init
* @see timer_read
* @see timer_interrupt_control
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_control(timer_t timer, timer_control_t signal)
{
	if (signal == TIMER_START)
	{
		*TIM_CR1[timer] |= TIM_CR1_CEN_Msk;
	}
	else if (signal == TIMER_STOP)
	{
		*TIM_CR1[timer] &= ~(TIM_CR1_CEN_Msk);
	}

}

/******************************************************************************
* Function: timer_read()
*//**
* \b Description:
*
* This function returns the contents of a timer's CNT register
*
* \b Note:
* Technically the function works without starting the timer, but that's useless
*
* PRE-CONDITION: The timer has been successfully initiated through timer_init()
*
* POST-CONDITION: The function has returned the current contents of CNT
*
*
* @param		timer refers to any timer present on-chip
*
*
* @return 		uint32_t
*
* \b Example:
* @code
* 	uint32_t curr_value = timer_read(TIMER9);
* @endcode
*
* @see timer_control
* @see timer_interrupt_control
* @see timer_prescaler_set
* @see timer_prescaler_get
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
uint32_t timer_read(timer_t timer)
{
	return (*TIM_CNT[timer]);
}

/******************************************************************************
* Function: timer_interrupt_control()
*//**
* \b Description:
*
* 	Activates or deactivates the selected interrupt
*
*
* PRE-CONDITION: The timer has been successfully initiated through timer_init()
* PRE-CONDITION: The requested interrupt is actually available on the selected timer.
*
* POST-CONDITION: The selected interrupt is enabled/disabled
*
*
* @param		timer refers to any timer present on-chip
*
* @param		interrupt refers to the selected interrupt type
*
* @param		signal decides whether the interrupt is enabled or disabled
*
* @return 		void
*
* \b Example:
* @code
* 	timer_init(config);
* 	timer_interrupt_control(TIMER1, UPDATE_INTERRUPT, INTERRUPT_ENABLE);
* 	timer_control(TIMER1, TIMER_START);
* @endcode
*
* @see timer_control
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_interrupt_control(timer_t timer, timer_interrupt_t interrupt, timer_interrupt_control_t signal)
{
	/**check for invalid combinations */

	/** Only timer 1 has BREAK and COM */
	assert(!(interrupt == BREAK_INTERRUPT && timer != TIMER1));
	assert(!(interrupt == COM_INTERRUPT && timer != TIMER1));

	/**Timers 9/10/11 don't have CCs 3 and 4 */
	assert(!((interrupt == CC3_INTERRUPT || interrupt == CC4_INTERRUPT)
				&& (timer >= TIMER9)));
	/** Timers 10/11 don't have CC2 either */
	assert(!(interrupt == CC2_INTERRUPT && timer >= TIMER10));

	/** Timers 10 and 11 don't have trigger interrrupts*/
	assert(!(interrupt == TRIGGER_INTERRUPT && timer >= TIMER10));

	if (signal == INTERRUPT_ENABLE)
	{
		*TIM_DIER[timer] |= 0x01UL << interrupt;
	}
	else if (signal == INTERRUPT_DISABLE)
	{
		*TIM_DIER[timer] &= ~(0x01UL << interrupt);
	}
}

/******************************************************************************
* Function: timer_prescaler_set()
*//**
* \b Description:
*
* 	Sets the prescaler value "on the fly"
*
*
* PRE-CONDITION: The timer has been successfully initiated through timer_init()
*
* POST-CONDITION: The timer's clock is divided by the new prescaler
*
*
* @param		timer refers to any timer present on-chip
*
* @param		prescaler is a uint16_t value
*
* @return 		void
*
* \b Example:
* @code
*	//On the fly changes of the prescaler
* 	timer_init(config);
* 	timer_prescaler_set(TIMER5, 3200);
* 	timer_control(TIMER5, TIMER_START);
* 	while (timer_read(TIMER5) < 300);
* 	timer_prescaler_set(TIMER5, 5000);
* @endcode
*
* @see timer_control
* @see timer_prescaler_get
* @see timer_interrupt_control
* @see timer_read
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_prescaler_set(timer_t timer, timer_prescaler_t prescaler)
{
	*TIM_PSC[timer] = prescaler;
}

/******************************************************************************
* Function: timer_prescaler_get()
*//**
* \b Description:
*
* 	Gets the prescaler value
*
*
* PRE-CONDITION: The timer has been successfully initiated through timer_init()
*
* POST-CONDITION: The timer's current prescaler value (TIMx_PSC) is returned
*
*
* @param		timer refers to any timer present on-chip
*
*
* @return 		timer_prescaler_t (uint16_t)
*
* \b Example:
* @code
*	timer_prescaler_t curr_prescaler_timer3 = timer_prescaler_get(TIMER3);
* @endcode
*
* @see timer_control
* @see timer_prescaler_set
* @see timer_interrupt_control
* @see timer_read
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
timer_prescaler_t timer_prescaler_get(timer_t timer)
{
	return (*TIM_PSC[timer]);
}

/******************************************************************************
* Function: timer_register_write()
*//**
* \b Description:
*
* 	Writes the desired value into the desired timer address space register. It is the
* 	user's own responisibility to consult the RM0383 to ensure that no reserved bits are
* 	overwritten, etc.
* 	Intended to be used alongside timer_register_read() to create composite advanced user functions
*
*
* PRE-CONDITION: The address does in fact lie in the address space of any timer.
*
* POST-CONDITION: The desired register's contents now reflect "value"
*
*
* @param		timer_register is a uint32_t which is cast as a 32bit address
*
* @param		value is an (up to) uint32_t value which is written to the desired register
*
* @return 		void
*
* \b Example:
* @code
*	uint32_t dier_timer3 = timer_register_read(TIM3_BASE + 0x0C); //get current value
*	dier_timer3 &= ~(0x01UL << TIM_DIER_CC3DE_Pos); //clear the DMA request on CC3 bit
*	timer_register_write(TIM3_BASE + 0x0C, dier_timer3);
*
* @endcode
*
* @see timer_register_read
* @see timer_read
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_register_write(uint32_t timer_register, uint32_t value)
{
	assert(    (timer_register >= TIM2_BASE && timer_register < RTC_BASE)
			|| (timer_register >= TIM1_BASE && timer_register < USART1_BASE)
			|| (timer_register >= TIM9_BASE && timer_register < SPI5_BASE));

	*((uint32_t *)timer_register) = value;
}

/******************************************************************************
* Function: timer_register_reads()
*//**
* \b Description:
*
* 	Read the current value of the register in timer address. It is the
* 	user's own responisibility to consult the RM0383 to ensure that no reserved bits are
* 	overwritten, etc.
* 	Intended to be used alongside timer_register_write() to create composite advanced user functions
*
*
* PRE-CONDITION: The address does in fact lie in the address space of any timer.
*
* POST-CONDITION: The register's current contents are returned
*
*
* @param		timer_register is a uint32_t which is cast as a 32bit address
*
*
* @return 		uint32_t timer_register's contents
*
* \b Example:
* @code
*	uint32_t dier_timer3 = timer_register_read(TIM3_BASE + 0x0C); //get current value
*	dier_timer3 &= ~(0x01UL << TIM_DIER_CC3DE_Pos); //clear the DMA request on CC3 bit
*	timer_register_write(TIM3_BASE + 0x0C, dier_timer3);
*
* @endcode
*
* @see timer_register_write
* @see timer_read
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
uint32_t timer_register_read(uint32_t timer_register)
{
	assert(    (timer_register >= TIM2_BASE && timer_register < RTC_BASE)
				|| (timer_register >= TIM1_BASE && timer_register < USART1_BASE)
				|| (timer_register >= TIM9_BASE && timer_register < SPI5_BASE));

	return(*((uint32_t *) timer_register));
}


#ifdef _TIMER_CC_H

static void timer_cc_init_output(timer_cc_t timer_cc, timer_cc_config_t *config);
static void timer_cc_init_input(timer_cc_t timer_cc, timer_cc_config_t *config);
static inline void timer_cc_parse_ccr(timer_cc_t timer_cc, uint32_t *timer_cc_port, uint32_t *timer_cc_channel);



/******************************************************************************
* Function: timer_cc_init()
*//**
* \b Description:
*
* This function is used to initialise the capture compare units based on the configuration table
*  defined in the timer_cc_stm32f411_config.c
*
* PRE-CONDITION: CC configuration table needs to populated (sizeof > 0) <br>
* PRE-CONDITION: timer_init() has been successfully carried out on the timers planned for use.
* PRE-CONDITION: The timer channel pins (TIMx_CHy) planned for use have been multiplexed
* 					appropriately with gpio_init()
*
* POST-CONDITION: The timer capture/compare channels are ready for use.
*
* @param  		config_table is a pointer to the configuration table that contains
*				the initialisation structures for each cc channel.
*
* @return 		void
*
* \b Example:
* @code
* 	const timer_config_t *timer_config = timer_config_get();
*	timer_init(timer_config);
*	const timer_cc_config_t *timer_cc_config = timer_cc_config_get();
*	timer_cc_init(timer_cc_config);
* @endcode
*
* @see timer_cc_config_get
* @see timer_cc_init_output
* @see timer_cc_init_input
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_cc_init(timer_cc_config_t *config_table)
{
	for (int timer_cc = 0; timer_cc < NUM_CCRS; timer_cc++)
	{
		if (sizeof(config_table[timer_cc]) != 0)
		{
			if (config_table[timer_cc].cc_mode == CC_OUTPUT)
			{
				timer_cc_init_output(timer_cc, &config_table[timer_cc]);
			}
			else
			{
				timer_cc_init_input(timer_cc, &config_table[timer_cc]);
			}

		}
	}
}

/******************************************************************************
* Function: timer_cc_init_output()
*//**
* \b Description:
*
* This function is used to initialise the current CC channel in output mode
*
* PRE-CONDITION: The current cc config table element must be non-zero
* PRE-CONDITION: timer_init() has been successfully carried out on the timer planned for use.
* PRE-CONDITION: The timer channel pins (TIMx_CHy) planned for use have been multiplexed
* 					appropriately with gpio_init()
*
* POST-CONDITION: The timer cc channel is configured in output mode
*
* @param 		timer_cc is a cc channel present on chip
*
* @param  		config is a pointer to the configuration table element that contains
*				the initialisation structure for timer_cc.
*
* @return 		void
*
* \b Example:
* Automatically called by timer_cc_init when
* @code
*		cc_config->cc_mode == CC_OUTPUT;
* @endcode
*
* @see timer_cc_init
* @see timer_cc_init_input
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
static void timer_cc_init_output(timer_cc_t timer_cc, timer_cc_config_t *config)
{
	uint32_t timer_cc_port;
	uint32_t timer_cc_channel;
	timer_cc_parse_ccr(timer_cc, &timer_cc_port, &timer_cc_channel);

	//0. Disable CCR on that number

	*TIM_CCER[timer_cc_port] &= ~(0x01UL << (4 * timer_cc_channel));

	//upper
	if (timer_cc_channel > 1)
	{
	//1. Set CCR to be output
		*TIM_CCMR2[timer_cc_port] &= ~( 0x03UL << (8 * (timer_cc_channel - 2)) );
	//2. Write FE on CCR
		*TIM_CCMR2[timer_cc_port] &= ~( (0x01UL << 2) << (8 * (timer_cc_channel -2)) );
		*TIM_CCMR2[timer_cc_port] |=  (config->output_fast_enable << 2) << (8 * (timer_cc_channel - 2));
	//3. Write PE on CCR
		*TIM_CCMR2[timer_cc_port] &= ~( (0x01UL << 3) << (8 * (timer_cc_channel -2)) );
		*TIM_CCMR2[timer_cc_port] |=  (config->output_preload_enable << 3)  << (8 * (timer_cc_channel - 2));
	//4. Write output mode on CCR
		*TIM_CCMR2[timer_cc_port] &= ~( (0x07UL << 4) << (8 * (timer_cc_channel - 2)) );
		*TIM_CCMR2[timer_cc_port] |= (config->output_mode << 4) << (8 * (timer_cc_channel - 2));
	//5. Write CE on CCR
		*TIM_CCMR2[timer_cc_port] &= ~( (0x01UL << 7) << (8 * (timer_cc_channel -2)) );
		*TIM_CCMR2[timer_cc_port] |=  (config->output_fast_enable << 7)  << (8 * (timer_cc_channel - 2));

	}
	//lower
	else
	{
		//1. Set CCR to be output
			*TIM_CCMR1[timer_cc_port] &= ~( 0x03UL << (8 *timer_cc_channel) );
		//2. Write FE on CCR
			*TIM_CCMR1[timer_cc_port] &= ~( (0x01UL << 2) << (8 * timer_cc_channel) );
			*TIM_CCMR1[timer_cc_port] |=  (config->output_fast_enable << 2) << (8 * timer_cc_channel);
		//3. Write PE on CCR
			*TIM_CCMR1[timer_cc_port] &= ~( (0x01UL << 3) << (8 * timer_cc_channel) );
			*TIM_CCMR1[timer_cc_port] |=  (config->output_preload_enable << 3)  << (8 * timer_cc_channel);
		//4. Write output mode on CCR
			*TIM_CCMR1[timer_cc_port] &= ~( (0x07UL << 4) << (8 * timer_cc_channel) );
			*TIM_CCMR1[timer_cc_port] |= (config->output_mode << 4) << (8 * timer_cc_channel);
		//5. Write CE on CCR
			*TIM_CCMR1[timer_cc_port] &= ~( (0x01UL << 7) << (8 * timer_cc_channel) );
			*TIM_CCMR1[timer_cc_port] |=  (config->output_fast_enable << 7)  << (8 * timer_cc_channel);
	}

    //6. Output polarity
		if (config->output_polarity == OUTPUT_ACTIVE_HIGH)
		{
			*TIM_CCER[timer_cc_port] |= (0x02UL << (4 * timer_cc_channel));
		}
		else if (config->output_polarity == OUTPUT_ACTIVE_LOW)
		{
			*TIM_CCER[timer_cc_port] &= ~(0x02UL << (4 * timer_cc_channel));

		}
}

/******************************************************************************
* Function: timer_cc_init_input()
*//**
* \b Description:
*
* This function is used to initialise the current CC channel in input mode
*
* PRE-CONDITION: The current cc config table element must be non-zero
* PRE-CONDITION: timer_init() has been successfully carried out on the timer planned for use.
* PRE-CONDITION: The timer channel pins (TIMx_CHy) planned for use have been multiplexed
* 					appropriately with gpio_init()
*
* POST-CONDITION: The timer cc channel is configured in input mode
*
* @param 		timer_cc is a cc channel present on chip
*
* @param  		config is a pointer to the configuration table element that contains
*				the initialisation structure for timer_cc.
*
* @return 		void
*
* \b Example:
* Automatically called by timer_cc_init when
* @code
*		cc_config->cc_mode != CC_OUTPUT;
* @endcode
*
* @see timer_cc_init
* @see timer_cc_init_output
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
static void timer_cc_init_input(timer_cc_t timer_cc, timer_cc_config_t *config)
{
	uint32_t timer_cc_port;
	uint32_t timer_cc_channel;

	timer_cc_parse_ccr(timer_cc, &timer_cc_port, &timer_cc_channel);

	//0. Disable CCR on that number

	*TIM_CCER[timer_cc_port] &= ~(0x01UL << (4 * timer_cc_channel));

	//upper
	if (timer_cc_channel > 1)
	{
	//1. Set CCR to be input
		*TIM_CCMR2[timer_cc_port] &= ~(0x03UL << (8 * (timer_cc_channel - 2)) );
		*TIM_CCMR2[timer_cc_port] |= (config->cc_mode << (8 * (timer_cc_channel - 2)) );
	//2. Set CCR input prescaler
		*TIM_CCMR2[timer_cc_port] &= ~((0x03UL << 2) << (8 * (timer_cc_channel - 2)) );
		*TIM_CCMR2[timer_cc_port] |= ((config->input_event_prescaler << 2) << (8 * (timer_cc_channel - 2)) );
	//3. Set CCR input filter
		*TIM_CCMR2[timer_cc_port] &= ~((0x0FUL << 4) << (8 * (timer_cc_channel - 2)) );
		*TIM_CCMR2[timer_cc_port] |= ((config->input_event_filter << 4) << (8 * (timer_cc_channel - 2)) );
	}
	//lower
	else
	{
		//1. Set CCR to be input
		*TIM_CCMR1[timer_cc_port] &= ~(0x03UL << (8 * timer_cc_channel) );
		*TIM_CCMR1[timer_cc_port] |= (config->cc_mode << (8 * timer_cc_channel) );
		//2. Set CCR input prescaler
		*TIM_CCMR1[timer_cc_port] &= ~((0x03UL << 2) << (8 * timer_cc_channel) );
		*TIM_CCMR1[timer_cc_port] |= ((config->input_event_prescaler << 2) << (8 * timer_cc_channel) );
		//3. Set CCR input filter
		*TIM_CCMR1[timer_cc_port] &= ~((0x0FUL << 4) << (8 * timer_cc_channel) );
		*TIM_CCMR1[timer_cc_port] |= ((config->input_event_filter << 4) << (8 * timer_cc_channel) );
	}
	//6. Set CCR input polarity

	if (config->input_polarity == RISING_EDGE)
	{
		*TIM_CCER[timer_cc_port] &= ~(0x05UL << (4 * timer_cc_channel));
	}
	else if (config->input_polarity == FALLING_EDGE)
	{
		*TIM_CCER[timer_cc_port] &= ~(0x05UL << (4 * timer_cc_channel));
		*TIM_CCER[timer_cc_port] |= (0x01UL << (4 * timer_cc_channel));
	}
	/**
	 * CC4 doesn't support "both edges" sensitivity
	 */
	else if (config->input_polarity == BOTH_EDGES && timer_cc_channel != 3)
	{
		*TIM_CCER[timer_cc_port] |= (0x05UL << (4 * timer_cc_channel));
	}
}

/******************************************************************************
* Function: timer_cc_parse_ccr()
*//**
* \b Description:
*
* This function is statically called by functions to convert the raw cc_channel identifier to a mapping
* 	of port and relative channel.
*
* PRE-CONDITION: The current cc config table element must me non-zero
* PRE-CONDITION: timer_init() has been successfully carried out on the timer planned for use.
* PRE-CONDITION: The timer channel pins (TIMx_CHy) planned for use have been multiplexed
* 					appropriately with gpio_init()
*
* POST-CONDITION: The timer cc channel is configured in output mode
*
* @param 		timer_cc is a cc channel present on chip
*
* @param  		*timer_cc_port is a pointer to a variable which will hold the calculated port value
*
* @param		*timer_cc_channel is a pointer to a variable which will hold the calculated channel value
*
* @return 		void
*
* \b Example:
*	within function_x
* @code
*
* uint32_t timer_cc_port, timer_cc_channel;
* timer_cc_parse_ccr(timer_cc, timer_cc_port, timer_cc_channel);
* @endcode
*
* @see timer_cc_init
* @see timer_cc_control
* @see timer_cc_read
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
static inline void timer_cc_parse_ccr(timer_cc_t timer_cc, uint32_t *timer_cc_port, uint32_t *timer_cc_channel)
{
		if (timer_cc < TIMER9_CCR1)
		{
			*timer_cc_channel = timer_cc % 4;
			*timer_cc_port = timer_cc / 4;
		}
		else if (timer_cc < TIMER10_CCR1)
		{
			*timer_cc_channel = timer_cc % 2;
			*timer_cc_port = TIMER9;
		}
		else if (timer_cc == TIMER10_CCR1)
		{
			*timer_cc_channel = 0;
			*timer_cc_port = TIMER10;
		}
		else if (timer_cc == TIMER11_CCR1)
		{
			*timer_cc_channel = 0;
			*timer_cc_port = TIMER11;
		}
}

/******************************************************************************
* Function: timer_cc_control()
*//**
* \b Description:
*
* This function activates or deactivates a channel.
*
* PRE-CONDITION: timer_cc_init has been called and finished succesfully
*
* POST-CONDITION: The timer cc channel has been activated/deactivated according to the signal
*
* @param 		timer_cc is a cc channel present on chip
*
* @param  		signal commands the channel to start or stop
*
* @return 		void
*
* \b Example:
*
* @code
* 	timer_cc_init(timer_cc_config);
* 	timer_cc_control(TIMER2_CC3, TIMER_START);
* @endcode
*
* @see timer_cc_init
* @see timer_cc_read
*
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_cc_control(timer_cc_t timer_cc, timer_control_t signal)
{
	uint32_t timer_cc_port;
	uint32_t timer_cc_channel;
	timer_cc_parse_ccr(timer_cc, &timer_cc_port, &timer_cc_channel);

	if (signal == TIMER_STOP)
	{
		*TIM_CCER[timer_cc_port] &= ~(0x01 << (4 * timer_cc_channel));
	}
	else if (signal == TIMER_START)
	{
		*TIM_CCER[timer_cc_port] |= 0x01 << (4 * timer_cc_channel);
	}
}

/******************************************************************************
* Function: timer_cc_read()
*//**
* \b Description:
*
* This function returns the current contents (target value) of the appropriate CCR register
*
* PRE-CONDITION: timer_cc_init has been called and finished succesfully
*
* POST-CONDITION: The functon returns the contents of CCR
*
* @param 		timer_cc is a cc channel present on chip
*
*
* @return 		uint16_t
*
* \b Example:
*
* @code
* 	timer_cc_init(timer_cc_config);
* 	timer_cc_control(TIMER2_CC3, TIMER_START);
* 	...
* 	uint16_t timer2_cc3_value = timer_cc_read(TIMER2_CC3);
*
* @endcode
*
* @see timer_cc_control
* @see timer_cc_write
*
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
uint16_t timer_cc_read(timer_cc_t timer_cc)
{
	return(*TIM_CCR[timer_cc]);
}

/******************************************************************************
* Function: timer_cc_write()
*//**
* \b Description:
*
* This function writes the target value to the appropriate CCR register
*
* PRE-CONDITION: timer_cc_init has been called and finished succesfully
*
* POST-CONDITION: The functon writes the new CCR
*
* @param 		timer_cc is a cc channel present on chip
*
* @param		value is the new desired CC target
*
* @return 		void
*
* \b Example:
*
* @code
* 	timer_cc_init(timer_cc_config);
* 	timer_cc_control(TIMER2_CC3, TIMER_START);
* 	...
* 	uint16_t timer2_cc3_value = 0xDAB;
	timer_cc_write(TIMER2_CC3, timer2_cc3_value);
* @endcode
*
* @see timer_cc_control
* @see timer_cc_write
*
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_cc_write(timer_cc_t timer_cc, uint16_t value)
{
	*TIM_CCR[timer_cc] = value;
}

/******************************************************************************
* Function: timer_cc_pwm_duty_cycle_set()
*//**
* \b Description:
*
* This function is fed a natural number from 0 - 100 representing the duty cycle,
* 	and calculates an approximation for the required 16-32bit value in CCRy
*
* \b Note:
* This PWM calculation mode works with as small as 2% error for PWM frequencies
* of up to 100kHz when run on the APB2 bus timers at 96MHz, i.e. TIM1,9,10,11.
* This suits almost all DC motor applications, heating/slow response systems, and low
* frequency power supplies. If you want to use it for high quality audio (200kHz, an
* implementation using the FPU will keep the error low. The resolution at such
* high frequencies drops to about 0.2% of your output voltage.
*
* The error on this duty cycle calculation rises the fewer ticks you use
* for PWM generation. A 22kHz PWM on a 48MHz 16bit timer leads to a maximum
* error of <0.5% when generating a 50% signal. If "absolute precision" is required,
* I will look into using the FPU, but I rather wouldn't.
*
* PRE-CONDITION: timer_init for the appropriate timer has been called and finished successfully
* PRE-CONDITION: timer_cc_init has been called and finished succesfully
* PRE-CONDITION: the timer_cc channel in question has been configured in PWM MDOE 1 or 2
*
* POST-CONDITION: The functon writes the correct duty cycle value to TIMx_CCR
*
* @param 		timer_cc is a cc channel present on chip
*
* @param		duty_cycle_pcnt is the new duty cycle
*
* @return 		void
*
* \b Example:
*
* @code
*   timer_init(timer_config);
* 	timer_cc_init(timer_cc_config);
* 	timer_control(TIMER1, TIMER_START);
* 	timer_cc_pwm_duty_cycle_set(TIMER1_CC1, 70);
* 	timer_cc_control(TIMER1_CC1, TIMER_START);

* @endcode
*
* @see timer_cc_control
* @see timer_cc_write
* @see timer_cc_read
* @see timer_cc_pwm_duty_cycle_get
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void timer_cc_pwm_duty_cycle_set(timer_cc_t timer_cc, uint32_t duty_cycle_pcnt)
{

	assert(duty_cycle_pcnt <= 100);
	uint32_t timer_cc_port;
	uint32_t timer_cc_channel;
	timer_cc_parse_ccr(timer_cc, &timer_cc_port, &timer_cc_channel);

	uint32_t estimated_one_pcnt = *TIM_ARR[timer_cc_port]/100;
	if (*TIM_ARR[timer_cc_port] % 100 > 50)
	{
		estimated_one_pcnt++;
	}

	if (duty_cycle_pcnt <= 50)
	{
	*TIM_CCR[timer_cc] = estimated_one_pcnt*duty_cycle_pcnt;
	}
	else
	{
		*TIM_CCR[timer_cc] = *TIM_ARR[timer_cc_port] - (estimated_one_pcnt*(100-duty_cycle_pcnt));
	}
}

/******************************************************************************
* Function: timer_cc_pwm_duty_cycle_get()
*//**
* \b Description:
*
* This function returns a ~1% correct estimation of the current pwm duty cycle for
* 	CCy
*
* PRE-CONDITION: timer_init for the appropriate timer has been called and finished successfully
* PRE-CONDITION: timer_cc_init has been called and finished succesfully
* PRE-CONDITION: the timer_cc channel in question has been configured in PWM MDOE 1 or 2
*
* POST-CONDITION: The functon returns an estimate of the current duty cycle value to TIMx_CCR
*
* @param 		timer_cc is a cc channel present on chip
*
*
* @return 		uint32_t
*
* \b Example:
*
* @code
*   timer_init(timer_config);
* 	timer_cc_init(timer_cc_config);
* 	timer_control(TIMER1, TIMER_START);
* 	uint32_t current_dc = timer_cc_pwm_duty_cycle_get(TIMER1_CC1);
* @endcode
*
* @see timer_cc_control
* @see timer_cc_write
* @see timer_cc_read
* @see timer_cc_pwm_duty_cycle_set
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
uint32_t timer_cc_pwm_duty_cycle_get(timer_cc_t timer_cc)
{
	uint32_t timer_cc_port;
	uint32_t timer_cc_channel
	timer_cc_parse_ccr(timer_cc, &timer_cc_port, &timer_cc_channel);

	uint32_t estimated_one_pcnt = *TIM_ARR[timer_cc_port]/100;
	if (*TIM_ARR[timer_cc_port] % 100 > 50)
	{
		estimated_one_pcnt++;
	}
	uint32_t estimated_duty_cycle = *TIM_CCR[timer_cc]/estimated_one_pcnt;
	if (*TIM_CCR[timer_cc] % estimated_one_pcnt > estimated_one_pcnt/2)
	{
		estimated_duty_cycle++;
	}
	return(estimated_duty_cycle);
}

#endif
