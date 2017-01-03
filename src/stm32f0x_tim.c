/**
  ******************************************************************************
  * @file    Template_2/stm32f0x_tim.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   TIM functions.
  ******************************************************************************
  * @attention
  *
  * Use this functions to configure timers.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0x_tim.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"

#include "hard.h"

//--- VARIABLES EXTERNAS ---//
extern volatile unsigned char timer_1seg;
extern volatile unsigned short timer_led_comm;
extern volatile unsigned short wait_ms_var;

extern volatile unsigned char new_t;
extern volatile unsigned short freq_tim;

//--- VARIABLES GLOBALES ---//

volatile unsigned short timer_1000 = 0;



//--- FUNCIONES DEL MODULO ---//
void Update_TIM3_CH1 (unsigned short a)
{
	TIM3->CCR1 = a;
}

void Update_TIM3_CH2 (unsigned short a)
{
	TIM3->CCR2 = a;
}

void Update_TIM3_CH3 (unsigned short a)
{
	TIM3->CCR3 = a;
}

void Update_TIM3_CH4 (unsigned short a)
{
	TIM3->CCR4 = a;
}

void Wait_ms (unsigned short wait)
{
	wait_ms_var = wait;

#ifdef WITH_HARDWARE_WATCHDOG
	while (wait_ms_var)
	{
		KickWatchdog();
	}
#else
	while (wait_ms_var);
#endif

}

//-------------------------------------------//
// @brief  TIM configure.
// @param  None
// @retval None
//------------------------------------------//
void TIM3_IRQHandler (void)
{
	if (TIM3->SR & 0x02)	//bajo el flag CC1IF
	{
		new_t = 1;
		freq_tim = TIM3->CCR1;
	}
	TIM3->SR = 0x00;

}


void TIM_3_Init (void)
{
	if (!RCC_TIM3_CLK)
		RCC_TIM3_CLK_ON;

	//Configuracion del timer.
	TIM3->CR1 = 0x00;		//clk int / 1; upcounting
	TIM3->CR2 = 0x00;		//igual al reset
	TIM3->CCMR1 = 0x0011;			//CH1 filtro N=2, input capt TI1
	//TIM3->CCMR1 = 0x0001;			//CH1 sin filtro, input capt TI1
	TIM3->CCMR2 = 0x0000;			//CH4 y CH3 no uso

	TIM3->CCER = 0x01;	//CH1 enabled trigger rising edge

	TIM3->ARR = 0xFFFF;
	TIM3->CNT = 0;
	TIM3->PSC = 0;

	//TIM3->EGR = TIM_EGR_CC1G;	//evento input capt CH1

	// Enable timer ver UDIS
	TIM3->DIER |= TIM_DIER_CC1IE;
	TIM3->CR1 |= TIM_CR1_CEN;

	//Timer sin Int
	//NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	//Configuracion Pines
	//Alternate Fuction
	GPIOA->AFR[0] = 0x01000000;	// PA6 -> AF1
	//GPIOB->AFR[0] = 0x00000011;	//PB1 -> AF1; PB0 -> AF1

	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 5);

}

void TIM_6_Init (void)
{
	if (!RCC_TIM6_CLK)
		RCC_TIM6_CLK_ON;

	//Configuracion del timer.
	TIM6->CR1 = 0x00;		//clk int / 1; upcounting
	TIM6->PSC = 47;			//tick cada 1us
	TIM6->ARR = 0xFFFF;			//para que arranque
	//TIM6->CR1 |= TIM_CR1_CEN;
}

void TIM14_IRQHandler (void)	//100uS
{

	if (TIM14->SR & 0x01)
		//bajar flag
		TIM14->SR = 0x00;
}


void TIM_14_Init (void)
{

	//NVIC_InitTypeDef NVIC_InitStructure;

	if (!RCC_TIM14_CLK)
		RCC_TIM14_CLK_ON;

	/*
	//Configuracion del timer.
	TIM14->ARR = 2000; //10m
	TIM14->CNT = 0;
	TIM14->PSC = 479;
	TIM14->EGR = TIM_EGR_UG;

	// Enable timer ver UDIS
	TIM14->DIER |= TIM_DIER_UIE;
	TIM14->CR1 |= TIM_CR1_CEN;

	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/

	//Configuracion del timer.
	TIM14->CR1 = 0x00;		//clk int / 1; upcounting; uev
	TIM14->PSC = 47;			//tick cada 1us
	TIM14->ARR = 0xFFFF;			//para que arranque
	TIM14->EGR |= 0x0001;
}

inline void TIM14_IC_CH1_OFF (void)
{
	//desa int
	//TIM14->DIER |= TIM_DIER_UIE;
	TIM14->DIER &= 0xFFFD;
}

inline void TIM14_IC_CH1_ON (void)
{
	//ena int
	TIM14->DIER |= TIM_DIER_CC1IE;
}



void TIM16_IRQHandler (void)	//100uS
{

	if (TIM16->SR & 0x01)
		//bajar flag
		TIM16->SR = 0x00;
}


void TIM_16_Init (void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	if (!RCC_TIM16_CLK)
		RCC_TIM16_CLK_ON;

	//Configuracion del timer.
	TIM16->ARR = 2000; //10m
	TIM16->CNT = 0;
	TIM16->PSC = 479;
	TIM16->EGR = TIM_EGR_UG;

	// Enable timer ver UDIS
	TIM16->DIER |= TIM_DIER_UIE;
	TIM16->CR1 |= TIM_CR1_CEN;

	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM17_IRQHandler (void)	//100uS
{

	//if (GPIOA_PIN0_OUT)
	//	GPIOA_PIN0_OFF;
	//else
	//	GPIOA_PIN0_ON;

	if (TIM17->SR & 0x01)
		//bajar flag
		TIM17->SR = 0x00;
}


void TIM_17_Init (void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	if (!RCC_TIM17_CLK)
		RCC_TIM17_CLK_ON;

	//Configuracion del timer.
	TIM17->ARR = 2000; //10m
	TIM17->CNT = 0;
	TIM17->PSC = 479;
	TIM17->EGR = TIM_EGR_UG;

	// Enable timer ver UDIS
	TIM17->DIER |= TIM_DIER_UIE;
	TIM17->CR1 |= TIM_CR1_CEN;

	NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//--- end of file ---//


