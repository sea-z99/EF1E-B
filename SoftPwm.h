/*
 * PWM.H
 *
 *  Created on: 2021-3-11
 *      Author: Rick
 */

#ifndef SOFTPWM_H_
#define SOFTPWM_H_
typedef struct softpwm
{
	uint8_t status;
	uint16_t counter;
	uint16_t freq_counter;
	uint16_t duty_counter;
	void (*Set)(void);
	void (*ReSet)(void);
	struct softpwm* next;
}softpwm;
void pwm_test(void);
void pwm_loop(void);
#endif /* PWM_H_ */
