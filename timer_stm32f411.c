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

static void timer_init_external_mode_1(timer_t timer, timer_config_t *config)
{

	*TIM_SMCR[timer] |= TIM_SMCR_SMS_Msk;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_TS_Msk);
	*TIM_SMCR[timer] |= config->advanced->trigger_source << TIM_SMCR_TS_Pos;

	*TIM_SMCR[timer] &= ~(TIM_SMCR_MSM_Msk);
	*TIM_SMCR[timer] |= config->advanced->msm << TIM_SMCR_MSM_Pos;
}


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

uint32_t timer_read(timer_t timer)
{
	return (*TIM_CNT[timer]);
}


void timer_prescaler_set(timer_t timer, timer_prescaler_t prescaler)
{
	*TIM_PSC[timer] = prescaler;
}

timer_prescaler_t timer_prescaler_get(timer_t timer)
{
	return (*TIM_PSC[timer]);
}


void timer_interrupt_control(timer_t timer, timer_interrupt_t interrupt, timer_interrupt_control_t signal)
{
	if (signal == INTERRUPT_ENABLE)
	{
		*TIM_DIER[timer] |= 0x01UL << interrupt;
	}
	else if (signal == INTERRUPT_DISABLE)
	{
		*TIM_DIER[timer] &= ~(0x01UL << interrupt);
	}
}

void timer_register_write(timer_t timer, uint32_t timer_register, uint32_t value)
{
	assert(    (timer_register >= TIM2_BASE && timer_register < RTC_BASE)
			|| (timer_register >= TIM1_BASE && timer_register < USART1_BASE)
			|| (timer_register >= TIM9_BASE && timer_register < SPI5_BASE));

	*((uint32_t *)timer_register) = value;
}

uint32_t timer_register_read(timer_t timer, uint32_t timer_register)
{
	assert(    (timer_register >= TIM2_BASE && timer_register < RTC_BASE)
				|| (timer_register >= TIM1_BASE && timer_register < USART1_BASE)
				|| (timer_register >= TIM9_BASE && timer_register < SPI5_BASE));

	return(*((uint32_t *) timer_register));
}


#ifdef _TIMER_CC_H

static void timer_cc_init_output(timer_cc_t timer_cc, timer_cc_config_t *config);
static void timer_cc_init_input(timer_cc_t timer_cc, timer_cc_config_t *config);

static inline void timer_cc_parse_ccr(timer_cc_t timer_cc, uint32_t *timer_cc_port, uint32_t *timer_cc_channel)
{
		if (timer_cc < 20)
		{
			*timer_cc_channel = timer_cc % 4;
			*timer_cc_port = timer_cc / 4;
		}
		else if (timer_cc < 22)
		{
			*timer_cc_channel = timer_cc % 2;
			*timer_cc_port = TIMER9;
		}
		else if (timer_cc == 22)
		{
			*timer_cc_channel = 0;
			*timer_cc_port = TIMER10;
		}
		else
		{
			*timer_cc_channel = 0;
			*timer_cc_port = TIMER11;
		}
}

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

uint32_t timer_cc_read(timer_cc_t timer_cc)
{
	return(*TIM_CCR[timer_cc]);
}

void timer_cc_write(timer_cc_t timer_cc, uint32_t value)
{
	*TIM_CCR[timer_cc] = value;
}

void timer_cc_pwm_duty_cycle_set(timer_cc_t timer_cc, uint32_t duty_cycle_pcnt)
{
	/*
	 * This PWM calculation mode works with as small as 2% error for PWM frequencies
	 * of up to 100kHz when run on the APB2 bus timers at 96MHz, i.e. TIM1,9,10,11.
	 * This suits almost all DC motor applications, heating/slow response systems, and low
	 * frequency power supplies. If you want to use it for high quality audio (200kHz, an
	 * implementation using the FPU will keep the error low. The resolution at such
	 * high frequencies drops to about 0.2% of your output voltage.
	 */
	assert(duty_cycle_pcnt <= 100);
	uint32_t timer_cc_port;
	uint32_t timer_cc_channel;
	timer_cc_parse_ccr(timer_cc, &timer_cc_port, &timer_cc_channel);


	/* NOTE: The error on this duty cycle calculation rises the fewer ticks you use
	 * for PWM generation. A 22kHz PWM on a 48MHz 16bit timer leads to a maximum
	 * error of <0.5% when generating a 50% signal. If "absolute precision" is required,
	 * I will look into using the FPU, but I rather wouldn't.
	 */

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
	else if (config->input_polarity == BOTH_EDGES)
	{
		*TIM_CCER[timer_cc_port] |= (0x05UL << (4 * timer_cc_channel));
	}
}

#endif
