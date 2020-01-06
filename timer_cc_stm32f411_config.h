#ifndef _TIMER_STM32F411_CC_CONFIG
#define _TIMER_STM32F411_CC_CONFIG

#include "stm32f411xe.h"
#include <stdint.h>
#include "timer_stm32f411_config.h"

typedef enum
{
	/*TIM1*/
	TIM1_CCR1,
	TIM1_CCR2,
	TIM1_CCR3,
	TIM1_CCR4,
	/*TIM2*/
	TIM2_CCR1,
	TIM2_CCR2,
	TIM2_CCR3,
	TIM2_CCR4,
	/*TIM3*/
	TIM3_CCR1,
	TIM3_CCR2,
	TIM3_CCR3,
	TIM3_CCR4,
	/*TIM4*/
	TIM4_CCR1,
	TIM4_CCR2,
	TIM4_CCR3,
	TIM4_CCR4,
	/*TIM5*/
	TIM5_CCR1,
	TIM5_CCR2,
	TIM5_CCR3,
	TIM5_CCR4,
	/*TIM9*/
	TIM9_CCR1,
	TIM9_CCR2,
	/*TIM10*/
	TIM10_CCR1,
	/*TIM11*/
	TIM11_CCR1,

	NUM_CCRS
}timer_cc_t;

typedef enum
{
	CC_OUTPUT,
	CC_INPUT_TI_ONE,
	CC_INPUT_TI_TWO,
	CC_INPUT_TRC
}timer_cc_mode_t;

typedef enum
{
	OUTPUT_ACTIVE_HIGH,
	OUTPUT_ACTIVE_LOW
}timer_cc_output_polarity_t;

typedef uint32_t timer_cc_output_fe_t;
typedef uint32_t timer_cc_output_pe_t;

typedef enum
{
	FROZEN,
	ACTIVE_MATCH,
	INACTIVE_MATCH,
	TOGGLE_MATCH,
	FORCE_LOW,
	FORCE_HIGH,
	PWM_MODE_ONE, /* Non-inverted duty cycle */
	PWM_MODE_TWO /* Inverted duty cycle */
}timer_cc_output_mode_t;

typedef uint32_t timer_cc_output_ce_t;

typedef enum
{
	RISING_EDGE,
	FALLING_EDGE,
	BOTH_EDGES
}timer_cc_input_polarity_t;

typedef enum
{
	CAPTURE_DIV_1,
	CAPTURE_DIV_2,
	CAPTURE_DIV_4,
	CAPTURE_DIV_8
}timer_cc_input_prescaler_t;

typedef timer_external_trigger_filter_t timer_cc_input_filter_t;

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


#endif
