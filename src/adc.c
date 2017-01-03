/*
 * adc.c
 *
 *  Created on: 04/05/2015
 *      Author: Mariano
 */
#include "adc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx.h"
//#include "core_cm0.h"

#include <stdint.h>


//--- VARIABLES EXTERNAS ---//
extern volatile unsigned short adc_ch [];

extern volatile unsigned char seq_ready;

//--- VARIABLES GLOBALES ---//
volatile unsigned short * p_channel;

void AdcConfig (void)
{
	unsigned short cal = 0;

	if (!RCC_ADC_CLK)
		RCC_ADC_CLK_ON;

	// preseteo los registros a default, la mayoria necesita tener ADC apagado
	ADC1->CR = 0x00000000;
	ADC1->IER = 0x00000000;
	ADC1->CFGR1 = 0x00000000;
	ADC1->CFGR2 = 0x00000000;
	ADC1->SMPR = 0x00000000;
	ADC1->TR = 0x0FFF0000;
	ADC1->CHSELR = 0x00000000;

	//set clock
	ADC1->CFGR2 = ADC_ClockMode_SynClkDiv4;

	//set resolution & trigger
	ADC1->CFGR1 |= ADC_Resolution_10b | ADC_CFGR1_CONT;	//software trigger

	//set sampling time
	ADC1->SMPR |= ADC_SampleTime_239_5Cycles;
	//ADC1->SMPR |= ADC_SampleTime_28_5Cycles;		//
	//ADC1->SMPR |= ADC_SampleTime_7_5Cycles;		//
	//ADC1->SMPR |= ADC_SampleTime_1_5Cycles;		//

	//set channel selection siempre convierte creciente, termina con el canal mas alto de la secuencia
	//ADC1->CHSELR |= ADC_Channel_0 | ADC_Channel_1 | ADC_Channel_2;
	//ADC1->CHSELR |= ADC_Channel_1 | ADC_Channel_2;

	ADC1->CHSELR |= ADC_Channel_0 | ADC_Channel_1 | ADC_Channel_2;		//modificado 03-01-16
	ADC1->CHSELR |= ADC_Channel_3 | ADC_Channel_4 | ADC_Channel_5;		//V12V			CH0
	ADC1->CHSELR |= ADC_Channel_6 | ADC_Channel_7 | ADC_Channel_8;		//V6V		 	CH1
	ADC1->CHSELR |= ADC_Channel_9;										//V5V			CH2
																		//V3V			CH3
																		//One_Ten_Sense	CH4
																		//One_Ten_Pote 	CH5
																		//Vout_Sense 	CH6


	//calibrar ADC
	cal = ADC_GetCalFactor();

	//set interrupts
	ADC1->IER |= ADC_IT_EOC;

	/* Configure NVIC for ADC */
	/* (7) Enable Interrupt on ADC */
	/* (8) Set priority for ADC */
	NVIC_EnableIRQ(ADC1_IRQn); /* (7) */
	NVIC_SetPriority(ADC1_IRQn,4); /* (8) */

	p_channel = &adc_ch[0];
	// Enable ADC1
	ADC1->CR |= ADC_CR_ADEN;
}

void ADC1_COMP_IRQHandler (void)
{
	if (ADC1->ISR & ADC_IT_EOC)
	{

		if (ADC1->ISR & ADC_IT_EOSEQ)	//seguro que es channel9
		{
			p_channel = &adc_ch[9];
			*p_channel = ADC1->DR;
			p_channel = &adc_ch[0];
			seq_ready = 1;
		}
		else
		{
			*p_channel = ADC1->DR;		//
			if (p_channel < &adc_ch[9])
				p_channel++;
		}
	}
	//clear pending
	ADC1->ISR |= ADC_IT_EOC | ADC_IT_EOSEQ;

}


/**
  * @brief  Active the Calibration operation for the selected ADC.
  * @note   The Calibration can be initiated only when ADC is still in the
  *         reset configuration (ADEN must be equal to 0).
  * @param  ADCx: where x can be 1 to select the ADC1 peripheral.
  * @retval ADC Calibration factor
  */
uint32_t ADC_GetCalFactor(void)
{
  uint32_t tmpreg = 0, calibrationcounter = 0, calibrationstatus = 0;

  /* Set the ADC calibartion */
  ADC1->CR |= (uint32_t)ADC_CR_ADCAL;

  /* Wait until no ADC calibration is completed */
  do
  {
    calibrationstatus = ADC1->CR & ADC_CR_ADCAL;
    calibrationcounter++;
  } while((calibrationcounter != CALIBRATION_TIMEOUT) && (calibrationstatus != 0x00));

  if((uint32_t)(ADC1->CR & ADC_CR_ADCAL) == RESET)
  {
    /*Get the calibration factor from the ADC data register */
    tmpreg = ADC1->DR;
  }
  else
  {
    /* Error factor */
    tmpreg = 0x00000000;
  }
  return tmpreg;
}



