/*
 * adc.h
 *
 *  Created on: 10/09/2014
 *      Author: Mariano
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#define RCC_ADC_CLK 		(RCC->APB2ENR & 0x00000200)
#define RCC_ADC_CLK_ON 		RCC->APB2ENR |= 0x00000200
#define RCC_ADC_CLK_OFF 	RCC->APB2ENR &= ~0x00000200

#define ADC_CH0		0x00000001
#define ADC_CH1		0x00000002
#define ADC_CH2		0x00000004

/* Calibration time out */
#define CALIBRATION_TIMEOUT       ((uint32_t)0x0000F000)


void ADC1_COMP_IRQHandler (void);
void AdcConfig (void);
uint32_t ADC_GetCalFactor(void);

#endif /* ADC_H_ */
