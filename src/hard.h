/*
 * hard.h
 *
 *  Created on: 28/11/2013
 *      Author: Mariano
 */

#ifndef HARD_H_
#define HARD_H_


//-- Defines For Configuration -------------------
//---- Configuration for Hardware or Software Watchdog -------
//#define WITH_HARDWARE_WATCHDOG

//---- Configuration for Hardware Versions -------
#define VER_1_0

//---- Configuration for Software Versions -------
//#define FOUR_FILTER
//#define SIXTEEN_FILTER

//-- End Of Defines For Configuration ---------------

//GPIOA pin0
//GPIOA pin1
//GPIOA pin2
//GPIOA pin3
//GPIOA pin4
//GPIOA pin5
//GPIOA pin6
//GPIOA pin7	analog input

//GPIOA pin8
#define LOAD_MOSFET		((GPIOA->ODR & 0x0100) != 0)
#define LOAD_MOSFET_ON	GPIOA->BSRR = 0x00000100
#define LOAD_MOSFET_OFF	GPIOA->BSRR = 0x01000000

//GPIOA pin9
#define S1_PIN	((GPIOA->IDR & 0x0200) == 0)

//GPIOA pin10
#define RELAY_EXT		((GPIOA->ODR & 0x0400) != 0)
#define RELAY_EXT_ON	GPIOA->BSRR = 0x00000400
#define RELAY_EXT_OFF	GPIOA->BSRR = 0x04000000

//GPIOA pin11
#define RELAY_INT		((GPIOA->ODR & 0x0800) != 0)
#define RELAY_INT_ON	GPIOA->BSRR = 0x00000800
#define RELAY_INT_OFF	GPIOA->BSRR = 0x08000000

//GPIOA pin12
#define LOAD_BAT		((GPIOA->ODR & 0x1000) != 0)
#define LOAD_BAT_ON		GPIOA->BSRR = 0x00001000
#define LOAD_BAT_OFF	GPIOA->BSRR = 0x10000000

//GPIOA pin13
//GPIOA pin14

//GPIOA pin15
#define LCD_RS ((GPIOA->ODR & 0x8000) != 0)
#define LCD_RS_ON	GPIOA->BSRR = 0x00008000
#define LCD_RS_OFF GPIOA->BSRR = 0x80000000

//GPIOB pin0
//GPIOB pin1	analog input

//GPIOB pin3
#define LCD_E		((GPIOB->ODR & 0x0008) != 0)
#define LCD_E_ON	GPIOB->BSRR = 0x00000008
#define LCD_E_OFF	GPIOB->BSRR = 0x00080000

//GPIOB pin4
//GPIOB pin5
//GPIOB pin6
//GPIOB pin7	interface lcd




//ESTADOS DEL PROGRAMA PRINCIPAL
enum MainState
{
	SYNC_SAMPLES = 0,
	SYNC_SAMPLES_1,
	BAT_INT_WITHOUT_220,
	BAT_INT_WITHOUT_220_1,
	BAT_INT_WITHOUT_220_LOAD,
	BAT_INT_WITHOUT_220_1_LOAD,
	BAT_INT_WITHOUT_220_2_LOAD,
	BAT_EXT_WITHOUT_220,
	BAT_EXT_WITHOUT_220_1,
	BAT_EXT_WITHOUT_220_LOAD,
	BAT_EXT_WITHOUT_220_1_LOAD,
	BAT_EXT_WITHOUT_220_2_LOAD,
	MAINS,
	MAINS_1,
	MAINS_LOAD,
	MAINS_1_LOAD,
	MAINS_2_LOAD,
	CHARGER,
	CHARGER_1,
	CHARGER_2,
	CHARGER_3,
	START_LOAD,
	START_1_LOAD,
	START_2_LOAD,
};


//----- Definiciones para timers ------
#define SWITCHES_TIMER_RELOAD	10

#define SWITCHES_THRESHOLD_FULL	300		//3 segundos
#define SWITCHES_THRESHOLD_HALF	100		//1 segundo
#define SWITCHES_THRESHOLD_MIN	5		//50 ms


//----- Definiciones para Main_Menu.h ------
#define TT_LCD_BACKLIGHT					30000	//30 segundos
#define TT_UPDATE_BUTTON 80
#define TT_UPDATE_BUTTON_SPEED 10

//----- Definiciones para switches ------
#define S_FULL		10
#define S_HALF		3
#define S_MIN		1
#define S_NO		0

#endif /* HARD_H_ */
