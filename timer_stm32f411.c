#include "timer_interface.h"
#include "timer_cc_interface.h" /* Only included on MCUs with CC timers */
#include "stm32f411xe.h"

#ifndef NULL
#define NULL (void *)0
#endif

static volatile uint32_t *const TIM_CR1[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE, (uint32_t *)TIM2_BASE, (uint32_t *)TIM3_BASE,
	(uint32_t *)TIM4_BASE, (uint32_t *)TIM5_BASE, (uint32_t *)TIM9_BASE,
	(uint32_t *)TIM10_BASE, (uint32_t *)TIM11_BASE
};

static volatile uint32_t *const TIM_CR2[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x04UL, (uint32_t *)TIM2_BASE + 0x04UL,
	(uint32_t *)TIM3_BASE + 0x04UL, (uint32_t *)TIM4_BASE + 0x04UL,
	(uint32_t *)TIM5_BASE + 0x04UL,
	(uint32_t *) NULL, (uint32_t *) NULL, (uint32_t *) NULL
};

static volatile uint32_t *const TIM_SMCR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x08UL, (uint32_t *)TIM2_BASE + 0x08UL,
	(uint32_t *)TIM3_BASE + 0x08UL, (uint32_t *)TIM4_BASE + 0x08UL,
	(uint32_t *)TIM5_BASE + 0x08UL, (uint32_t *)TIM9_BASE + 0x08UL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

static volatile uint32_t *const TIM_DIER[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x0CUL, (uint32_t *)TIM2_BASE + 0x0CUL,
	(uint32_t *)TIM3_BASE + 0x0CUL, (uint32_t *)TIM4_BASE + 0x0CUL,
	(uint32_t *)TIM5_BASE + 0x0CUL, (uint32_t *)TIM9_BASE + 0x0CUL,
	(uint32_t *)TIM10_BASE + 0x0CUL, (uint32_t *)TIM11_BASE + 0x0CUL
};

static volatile uint32_t *const TIM_SR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x10UL, (uint32_t *)TIM2_BASE + 0x10UL,
	(uint32_t *)TIM3_BASE + 0x10UL, (uint32_t *)TIM4_BASE + 0x10UL,
	(uint32_t *)TIM5_BASE + 0x10UL, (uint32_t *)TIM9_BASE + 0x10UL,
	(uint32_t *)TIM10_BASE + 0x10UL, (uint32_t *)TIM11_BASE + 0x10UL
};

static volatile uint32_t *const TIM_EGR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x14UL, (uint32_t *)TIM2_BASE + 0x14UL,
	(uint32_t *)TIM3_BASE + 0x14UL, (uint32_t *)TIM4_BASE + 0x14UL,
	(uint32_t *)TIM5_BASE + 0x14UL, (uint32_t *)TIM9_BASE + 0x14UL,
	(uint32_t *)TIM10_BASE + 0x14UL, (uint32_t *)TIM11_BASE + 0x14UL
};

static volatile uint32_t *const TIM_CCMR1[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x18UL, (uint32_t *)TIM2_BASE + 0x18UL,
	(uint32_t *)TIM3_BASE + 0x18UL, (uint32_t *)TIM4_BASE + 0x18UL,
	(uint32_t *)TIM5_BASE + 0x18UL, (uint32_t *)TIM9_BASE + 0x18UL,
	(uint32_t *)TIM10_BASE + 0x18UL, (uint32_t *)TIM11_BASE + 0x18UL
};

static volatile uint32_t *const TIM_CCMR2[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x1CUL, (uint32_t *)TIM2_BASE + 0x1CUL,
	(uint32_t *)TIM3_BASE + 0x1CUL, (uint32_t *)TIM4_BASE + 0x1CUL,
	(uint32_t *)TIM5_BASE + 0x1CUL, (uint32_t *)NULL,
	(uint32_t *)NULL, (uint32_t *)NULL
};

static volatile uint32_t *const TIM_CCER[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x20UL, (uint32_t *)TIM2_BASE + 0x20UL,
	(uint32_t *)TIM3_BASE + 0x20UL, (uint32_t *)TIM4_BASE + 0x20UL,
	(uint32_t *)TIM5_BASE + 0x20UL, (uint32_t *)TIM9_BASE + 0x20UL,
	(uint32_t *)TIM10_BASE + 0x20UL, (uint32_t *)TIM11_BASE + 0x20UL
};

static volatile uint32_t *const TIM_CNT[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x24UL, (uint32_t *)TIM2_BASE + 0x24UL,
	(uint32_t *)TIM3_BASE + 0x24UL, (uint32_t *)TIM4_BASE + 0x24UL,
	(uint32_t *)TIM5_BASE + 0x24UL, (uint32_t *)TIM9_BASE + 0x24UL,
	(uint32_t *)TIM10_BASE + 0x24UL, (uint32_t *)TIM11_BASE + 0x24UL
};

static volatile uint16_t *const TIM_PSC[NUM_TIMERS] =
{
	(uint16_t *)TIM1_BASE + 0x28UL, (uint16_t *)TIM2_BASE + 0x28UL,
	(uint16_t *)TIM3_BASE + 0x28UL, (uint16_t *)TIM4_BASE + 0x28UL,
	(uint16_t *)TIM5_BASE + 0x28UL, (uint16_t *)TIM9_BASE + 0x28UL,
	(uint16_t *)TIM10_BASE + 0x28UL, (uint16_t *)TIM11_BASE + 0x28UL
};

static volatile uint32_t *const TIM_ARR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x2CUL, (uint32_t *)TIM2_BASE + 0x2CUL,
	(uint32_t *)TIM3_BASE + 0x2CUL, (uint32_t *)TIM4_BASE + 0x2CUL,
	(uint32_t *)TIM5_BASE + 0x2CUL, (uint32_t *)TIM9_BASE + 0x2CUL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

static volatile uint32_t *const TIM_RCR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x30UL, (uint32_t *)NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

//COMPOSITE REGISTER CONSISTING OF ALL CCR REGISTERS
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

static volatile uint32_t *const TIM_BDTR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x44UL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

static volatile uint32_t *const TIM_DCR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x48UL, (uint32_t *)TIM2_BASE + 0x48UL,
	(uint32_t *)TIM3_BASE + 0x48UL, (uint32_t *)TIM4_BASE + 0x48UL,
	(uint32_t *)TIM5_BASE + 0x48UL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

static volatile uint32_t *const TIM_DMAR[NUM_TIMERS] =
{
	(uint32_t *)TIM1_BASE + 0x4C, (uint32_t *)TIM2_BASE + 0x4C,
	(uint32_t *)TIM3_BASE + 0x4C, (uint32_t *)TIM4_BASE + 0x4C,
	(uint32_t *)TIM5_BASE + 0x4C, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) NULL
};

static volatile uint32_t *const TIM_OR[NUM_TIMERS] =
{
	(uint32_t *) NULL, (uint32_t *)TIM2_BASE + 0x50UL,
	(uint32_t *) NULL, (uint32_t *) NULL,
	(uint32_t *)TIM5_BASE + 0x50UL, (uint32_t *) NULL,
	(uint32_t *) NULL, (uint32_t *) TIM11_BASE + 0x50UL
};

void timer_init(timer_config_t *config_table)
{
	for (int timer; timer < NUM_TIMERS; timer++)
	{
		*TIM_CR1[timer] &= ~(TIM_CR1_CEN_Msk);

		if (sizeof(config_table[timer]) != 0)
		{

			if (config_table[timer].clock_source == INTERNAL_CLOCK)
			{
				*TIM_SMCR[timer] &= ~(TIM_SMCR_SMS_Msk);
			}
			else if (config_table[timer].clock_source == EXTERNAL_MODE_1)
			{
				*TIM_SMCR[timer] |= TIM_SMCR_SMS_Msk;
			}
			else if (config_table[timer].clock_source == EXTERNAL_MODE_2)
			{
				*TIM_SMCR[timer] |= TIM_SMCR_ECE_Msk;
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
			*TIM_CR1[timer] |= (config_table[timer].advanced->one_pulse_mode) << TIM_CR1_OPM_Pos;

			*TIM_CR1[timer] &= ~(TIM_CR1_UDIS_Msk);
			*TIM_CR1[timer] |= (config_table[timer].advanced->update_event_dis) << TIM_CR1_UDIS_Pos;
		}
	}

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
