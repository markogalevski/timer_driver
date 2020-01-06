#include "timer_cc_stm32f411_config.h"
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

const timer_cc_config_t *timer_cc_config_table_get(void)
{
	return(config_table);
}
