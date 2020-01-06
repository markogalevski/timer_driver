#ifndef _TIMER_CC_H
#define _TIMER_CC_H

#include "timer_cc_stm32f411_config.h"
#include "stm32f411xe.h"
#include <stdint.h>

void timer_cc_init(timer_cc_config_t *cc_config_table);
void timer_cc_control(timer_cc_t timer_cc, timer_control_t signal);
uint32_t timer_cc_read(timer_cc_t timer_cc);
#endif
