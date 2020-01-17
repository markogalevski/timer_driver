/*******************************************************************************
* Title                 :   Timer Config Header for STM32F411
* Filename              :   timer_stm32f411_config.h
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

/** @file timer_stm32f411_config.h
 *  @brief Microcontroller specific header containing typedefs for all relevant
 *  config options.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#ifndef _TIMER_STM32F411_CONFIG
#define _TIMER_STM32F411_CONFIG
#include <stdint.h>

#ifndef NULL
#define NULL (void *)0
#endif

/**
 * Contains all on-chip timers. These are the handle by which the user
 * interfaces with the timers.
 */
typedef enum
{
	TIMER1, /**< Timer 1: Advanced-control 16-bit Timer - Chapter 12 RM0383 */
	TIMER2, /**< Timer 2: General Purpose 32-bit Timer - Chapter 13 RM0383 */
	TIMER3, /**< Timer 3: General Purpose 16-bit Timer - Chapter 13 RM0383 */
	TIMER4, /**< Timer 4: General Purpose 16-bit Timer - Chapter 13 RM0383 */
	TIMER5, /**< Timer 5: General Purpose 32-bit Timer - Chapter 13 RM0383 */
	TIMER9, /**< Timer 9: High Speed APB2 General Purpose 16-bit Timer - Chapter 14 RM0383 */
	TIMER10,/**< Timer 10: High Speed APB2 General Purpose 16-bit Timer - Chapter 14 RM0383 */
	TIMER11,/**< Timer 11: High Speed APB2 General Purpose 16-bit Timer - Chapter 14 RM0383 */
	NUM_TIMERS /**< total number of timers used in the generation of register arrays, init looping, etc. */
} timer_t;

/**
 * Defines all types of interrupts supported on the timers. Not all timers support all interrupts.
 */
typedef enum
{
	UPDATE_INTERRUPT,	/**< Generate interrupt on update event (timer overflow/underflow, etc). All Timers */
	CC1_INTERRUPT, 		/**< Generate interrupt on CC1 event. All Timers*/
	CC2_INTERRUPT,		/**< Generate interrupt on CC2 event. All Timers but 10 and 11 */
	CC3_INTERRUPT,		/**< Generate interrupt on CC3 event. Timers 1-5 */
	CC4_INTERRUPT,		/**< Generate interrupt on CC4 event. Timers 1-5 */
	COM_INTERRUPT,		/**< Generate interrupt on COM event. @see RM0383 pg 295, Bit 5. Timer 1 only. */
	TRIGGER_INTERRUPT,  /**< Generate interrupt on Trigger event. @see TIF bit in RM0383, TIMx_SR. All timers but 10 and 11 */
	BREAK_INTERRUPT		/**M Generate interrupt on Break event. @see RM0383 pg 295, Bit 7. Timer 1 only */
}timer_interrupt_t;

/**
 * Defines the available clock sources. For full detail @see RM0383 chapters 12, 13, or 14.
 */
typedef enum
{
	INTERNAL_CLOCK,		/**< Default option. Timer is clocked by CK_INT. Prevents alternative configurations */
	EXTERNAL_MODE_1, 	/**< Timer is clocked by an internal trigger source TRGI. See @timer_trigger_t*/
	EXTERNAL_MODE_2		/**< Timer is clocked by the external trigger source.
	 	 	 	 	 	 	 Identical to selecting ETRF as the trigger in External Mode 1*/
}timer_alt_clock_source_t;

/**
 * Contains all the waves a timer can be controller from an outside source.
 * For detailed information on encoder modes, see RM0383 Chapter 12.3.16
 */
typedef enum
{
	SLAVE_MODE_DISABLED, /**< Timer is clocked internally from Pclk*/
	ENCODER_MODE_1, /**< Timer is clocked by edge transitions on TI2 (TIMx_CH2) depending on the level of TI1 (TIMx_CH1) */
	ENCODER_MODE_2, /**< Timer is clocked by edge transitions on TI1 (TIMx_CH1) depending on the level of TI2 (TIMx_CH2)*/
	ENCODER_MODE_3, /**< Timer is clocked by transitions on TI1 (TIMx_CH1) and TI2 (TIMx_CH2) depending on the level f the opposite */
	RESET_MODE,		/**< Timer is reset on rising edge of TRGI (timer_trigger_t) */
	GATED_MODE,		/**< Timer only counts while TRGI is (timer_trigger_t) is high */
	TRIGGER_MODE	/**< Timer starts counting upon rising edge of TRGI (timer_trigger_t) */
}timer_slave_mode_t;

/**
 * Contains all of the possible internal trigger sources (TRGI), used to control various slave modes.
 * Internal Trigger Sources (ITR0-3) vary from timer to timer. See Table 49 and similar for examples
 */
typedef enum
{
	ITR0, /**< TRGI is controlled by the trigger output (TRGO) of the first timer in the table */
	ITR1, /**< TRGI is controlled by the trigger output (TRGO) of the second timer in the table */
	ITR2, /**< TRGI is controlled by the trigger output (TRGO) of the third timer in the table */
	ITR3, /**< TRGI is controlled by the trigger output (TRGO) of the fourth timer in the table */
	TI1_EDGE_DETECTOR, /**< TRGI takes on the output of the raw edge detector, without filtering */
	TI1_FP1, /**< TRGI takes on the value of the filtered edge detection on input TI1 (TIMx_CH1) */
	TI2_FP2, /**< TRGI takes on the value of the filtered edge detection on input TI2 (TIMx_CH2) */
	ETRF	/**< TRGI takes on the value of the external trigger source */
}timer_trigger_t;


/**
 * Contains the prescaler options on the raw external trigger input
 */
typedef enum
{
	TRIGGER_DIV_1, 	/**<ETR is purely sampled */
	TRIGGER_DIV_2, 	/**<Every second ETR event is sampled */
	TRIGGER_DIV_4, 	/**<Every fourth ETR event is sampled */
	TRIGGER_DIV_8 	/**<Every eighth ETR event is sampled */
}timer_external_trigger_prescaler_t;


/**
 * Contains the frequency of the digital event filter.
 */
typedef enum
{
	CK_INT_TIMES_1,	/**<fDTS = fCK_INT*/
	CK_INT_TIMES_2,	/**<fDTS = 2*fCK_INT*/
	CK_INT_TIMES_4	/**<fDTS = 4*fCK_INT*/
}
timer_digital_filter_clock_div_t;

/**
 * Contains the options for digital filtration of external trigger events.
 * The selected sampling frequency is (fDTS or fCK_INT) divided by a factor of 2- 32,
 * and N consecutive events must occur before they are acknowledged
 */
typedef enum
{
	DTS, 			/**< Input sampled at fDTS (fs = fDTS), 1 input triggers the event (N = 1)*/
	CK_INT_N_2, 	/**<fs = CK_INT, N = 2 */
	CK_INT_N_4, 	/**<fs = fCK_INT, N = 4 */
	CK_INT_N_8, 	/**<fs = fCK_INT, N = 8 */
	DTS_DIV_2_N_6,	/**<fs = fDTS/2, N = 6 */
	DTS_DIV_2_N_8,	/**<fs = fDTS/2, N = 8 */
	DTS_DIV_4_N_6,	/**<fs = fDTS/4, N = 6 */
	DTS_DIV_4_N_8,	/**<fs = fDTS/4, N = 8 */
	DTS_DIV_8_N_6,	/**<fs = fDTS/8, N = 6 */
	DTS_DIV_8_N_8,	/**<fs = fDTS/8, N = 8 */
	DTS_DIV_16_N_5,	/**<fs = fDTS/16, N = 5 */
	DTS_DIV_16_N_6,	/**<fs = fDTS/16, N = 6 */
	DTS_DIV_16_N_8,	/**<fs = fDTS/16, N = 8 */
	DTS_DIV_32_N_5,	/**<fs = fDTS/32, N = 5 */
	DTS_DIV_32_N_6,	/**<fs = fDTS/32, N = 6 */
	DTS_DIV_32_N_8	/**<fs = fDTS/32, N = 8 */
}timer_external_trigger_filter_t;

/**
 * Master/Slave mode is used for multiple timer synchronisation. With MSM ON, the master device will
 * delay its counting by one cycle, giving the slave time to receive and parse the start command.
 */
typedef enum
{
	MASTER_SLAVE_DISABLED,		/**<Master/Slave mode is not on */
	MASTER_SLAVE_ENABLED		/**<Master/Slave mode is activated */
}
timer_master_slave_mode_t;


/**
 * Contains the options for counter alignment
 */
typedef enum
{
	EDGE_ALIGNED, 		/**< The counter counts purely up or down,
						depending on the selected direciton @see timer_direction_t */
	CENTER_ALIGNED_1,	/**< The counter counts up, then down. Capture Compare match
						events only occur on the way down */
	CENTER_ALIGNED_2,	/**< The counter counts up, then down. Capture Compare match
						events only occur on the way up */
	CENTER_ALIGNED_3	/**< The counter counts up, then down. Capture Compare match
						events occur on both ways */
}timer_alignment_t;

/**
 * The options for counter direction. Only relevant in edge-aligned mode
 */
typedef enum
{
	UPCOUNTER,	/**<The counter counts from 0 up to the reload value */
	DOWNCOUNTER	/**<The counter counts from the reload value down to 0 */
}timer_direction_t;


/**
 * The main prescaler must be a 16bit number
 */
typedef uint16_t timer_prescaler_t;

/**
 * Contains the option for one pulse mode. A timer in one pulse mode must be
 * restarted after every update event. It can also be reconfigured inbetween starts.
 */
typedef enum
{
	TIMER_OPM_DISABLED, /**<One Pulse Mode is off */
	TIMER_OPM_ENABLED	/**<One Pulse Mode is on */
} timer_opm_t;

/**
 * Auto Reload Preload Buffer options. When enabled, new ARR values are only
 * active after the next update event. When disabled, new ARR values are immediately
 * transferred
 */
typedef enum
{
	TIMER_ARPE_DISABLED,/**< Disabled ARR buffer */
	TIMER_ARPE_ENABLED	/**< Enabled ARR buffer */
}timer_arpe_t;

/**
 * Updated Disable options. When UDIS = 1, update events are not generated under any
 * circumstances
 */
typedef enum
{
	TIMER_UDIS_DISABLED,	/**<Update events are generated */
	TIMER_UDIS_ENABLED		/**<Update events are not generated */
} timer_udis_t;

/**
 * Options for the external trigger's polarity.
 */
typedef enum
{
	TRIGGER_POLARITY_NON_INVERTED, 	/**<External trigger is active high and/or rising edge */
	TRIGGER_POLARITY_INVERTED		/**<External trigger is active low and/or falling edge */
}timer_external_trigger_polarity_t;


/**
 * Used to configure the behaviour of the timer when reading external triggers. Embedded
 * within the advanced sub-structure.
 */
typedef struct
{
	timer_digital_filter_clock_div_t dts;			/**<Sampling frequency settings */
	timer_external_trigger_prescaler_t prescaler;	/**<Sampling frequency prescaler settings when using ETR*/
	timer_external_trigger_filter_t filter;			/**<Filter settings when using ETR*/
	timer_external_trigger_polarity_t polarity;		/**<ETR polarity settings */
}timer_external_trigger_t;

/**
 * Advanced structure contains access to features such as one pulse mode, update event disables,
 * and most importantly, slave modes and exernal clock sources through
 * slave_mode, trigger_surce, and *external_trigger
 */
typedef struct
{
	timer_alt_clock_source_t clock_source; /**<Determines alternate clock usage */
	timer_opm_t one_pulse_mode; 	/**<Determines whether the timer restarts after completing a count cycle */
	timer_udis_t update_event_dis;	/**<Determines whether the timer generates update events after count cycles */
	timer_slave_mode_t slave_mode;	/**<Determines from where the timer receives its clock and how it responds to it*/
	timer_trigger_t trigger_source;	/**<Trigger sources for slave behaviour, works with slave_mode */
	timer_master_slave_mode_t msm;	/**<Master/Slave behaviour. Determines whether the master and this timer synchronise themselves */
	const timer_external_trigger_t *external_trigger; 	/**<Struct containing configuration information for trigger sources external to the perpheral*/
}timer_advanced_t;

/**
 * Basic config data. Used to configure the timebase of the timer.
 */
typedef struct
{
timer_alignment_t alignment; 			/**<Determines whether the timer counts unidirectionally or bidirectionally */
timer_direction_t direction;			/**<Determines count direction when unidirectional*/
timer_prescaler_t prescaler;			/**<Division factor of peripheral input clock */
uint32_t auto_reload;					/**<The value by which the counter generates an update event and restarts counting */
timer_arpe_t auto_reload_preload_en;	/**<Setting deciding whether new Auto_Reload values are transferred immediately or after next update*/
const timer_advanced_t *advanced; 		/**<Pointer to a structure containing more advanced options. */
}timer_config_t;

const timer_config_t *timer_config_get(void);

#endif
