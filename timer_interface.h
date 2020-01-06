#ifndef _TIMER_H
#define _TIMER_H

#include "timer_stm32f411_config.h"
#include <stdint.h>
#include <assert.h>

#ifndef DISABLED
#define DISABLED 0
#endif

#ifndef ENABLED
#define ENABLED 1
#endif

typedef enum
{
	TIMER_STOP,
	TIMER_START
} timer_control_t;


void timer_init(timer_config_t *config_table);
void timer_control(timer_t timer, timer_control_t signal);
uint32_t timer_read(timer_t timer);
void timer_prescaler_set(timer_t timer, timer_prescaler_t prescaler);
timer_prescaler_t timer_prescaler_get(timer_t timer);
void timer_interrupt_control(timer_t timer, timer_interrupt_t interrupt);

void timer_register_write(timer_t timer, uint32_t timer_register, uint32_t value);
uint32_t timer_register_read(timer_t timer, uint32_t timer_register);












#endif
