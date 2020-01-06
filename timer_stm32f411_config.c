#include "timer_stm32f411_config.h"

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

static const timer_advanced_t config_advanced[NUM_TIMERS] =
{
				//OPM		//UDIS			//EXTERNAL_TRIGGER
/*TIM1_ADV*/	{},
/*TIM2_ADV*/	{},
/*TIM3_ADV*/	{},
/*TIM4_ADV*/	{},
/*TIM5_ADV*/	{},
/*TIM9_ADV*/	{},
/*TIM10_ADV*/	{},
/*TIM11_ADV*/	{}
};

static const timer_config_t config_table[NUM_TIMERS] =
{ 			//CLOCK			//SLAVE MODE	    //ALIGNMENT		 	//DIR		//PRE		//AUTO	//ARPE	  //ADVANCED
			//SOURCE															//SCALER	//RELOAD
/*TIM1*/	{INTERNAL_CLOCK, SLAVE_MODE_DISABLE, EDGE_ALIGNED,	UPCOUNTER,   	400,	  	16,			1,    0},
/*TIM2*/	{},
/*TIM3*/	{},
/*TIM4*/	{},
/*TIM5*/	{},
/*TIM9*/	{},
/*TIM10*/	{},
/*TIM11*/	{}
};

const timer_config_t *timer_config_table_get(void)
{
	return(config_table);
}
