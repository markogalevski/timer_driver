#ifndef _TIMER_STM32F411_CONFIG
#define _TIMER_STM32F411_CONFIG
#include <stdint.h>


#ifndef DISABLED
#define DISABLED 0
#endif

#ifndef ENABLED
#define ENABLED 1
#endif


typedef enum
{
	TIMER1,
	TIMER2,
	TIMER3,
	TIMER4,
	TIMER5,
	TIMER9,
	TIMER10,
	TIMER11,
	NUM_TIMERS
} timer_t;


typedef enum
{
	update_interrupt,
	CC1_interrupt,
	CC2_interrupt,
	CC3_interrupt,
	CC4_interrupt,
	COM_interrupt,
	trigger_interrupt,
	break_interrupt
}timer_interrupt_t;

typedef enum
{
	INTERNAL_CLOCK,
	EXTERNAL_MODE_1,
	EXTERNAL_MODE_2
}timer_clock_source_t;

typedef enum
{
	SLAVE_MODE_DISABLE,
	ENCODER_MODE_1,
	ENCODER_MODE_2,
	ENCODER_MODE_3,
	RESET_MODE,
	GATED_MODE,
	TRIGGER_MODE
}timer_slave_mode_t;

typedef enum
{
	ITR0,
	ITR1,
	ITR2,
	ITR3,
	TI1_EDGE_DETECTOR,
	TI1_FP1,
	TI2_FP2,
	ETRF
}timer_trigger_t;

typedef enum
{
	DTS,
	CK_INT_N_2,
	CK_INT_N_4,
	CK_INT_N_8,
	DTS_DIV_2_N_6,
	DTS_DIV_2_N_8,
	DTS_DIV_4_N_6,
	DTS_DIV_4_N_8,
	DTS_DIV_8_N_6,
	DTS_DIV_8_N_8,
	DTS_DIV_16_N_5,
	DTS_DIV_16_N_6,
	DTS_DIV_16_N_8,
	DTS_DIV_32_N_5,
	DTS_DIV_32_N_6,
	DTS_DIV_32_N_8
}timer_external_trigger_filter_t;


typedef enum
{
	TRIGGER_DIV_1,
	TRIGGER_DIV_2,
	TRIGGER_DIV_4,
	TRIGGER_DIV_8
}timer_external_trigger_prescaler_t;

typedef enum
{
	EDGE_ALIGNED,
	CENTER_ALIGNED_1,
	CENTER_ALIGNED_2,
	CENTER_ALIGNED_3
}timer_alignment_t;

typedef enum
{
	UPCOUNTER,
	DOWNCOUNTER
}timer_direction_t;

typedef uint16_t timer_prescaler_t;

typedef uint32_t timer_opm_t;

typedef uint32_t timer_arpe_t;

typedef uint32_t timer_udis_t;

typedef uint32_t timer_external_trigger_polarity_t;

typedef struct
{
	timer_external_trigger_filter_t filter;
	timer_external_trigger_prescaler_t prescaler;
	timer_external_trigger_polarity_t polarity;
}timer_external_trigger_t;

//FEEL FREE TO EXPAND ADVANCED SECTION
typedef struct
{
	timer_opm_t one_pulse_mode;
	timer_udis_t update_event_dis;
	timer_trigger_t trigger;
	const timer_external_trigger_t *external_trigger;
}timer_advanced_t;

typedef struct
{
timer_clock_source_t clock_source;
timer_slave_mode_t slave_mode;
timer_alignment_t alignment;
timer_direction_t direction;
timer_prescaler_t prescaler;
uint32_t auto_reload;
timer_arpe_t auto_reload_preload_en;
const timer_advanced_t *advanced;
}timer_config_t;



#endif
