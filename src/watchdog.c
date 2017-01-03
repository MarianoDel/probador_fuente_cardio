/*
 * watchdog.c
 *
 *  Created on: 13/12/2015
 *      Author: Mariano
 */
#include "stm32f0xx.h"

//Independent watchdog (IWDG)
//is activated by hardware or software

//If the “Hardware watchdog” feature is enabled through the device option bits,
//the watchdog is automatically enabled at power-on

//Independent clock of 40KHz

//---- Globals ----//


//---- Module Functions ----//
void KickWatchdog (void)
{
	IWDG->KR = 0x0000AAAA;
}

void ConfigureWatchdog(void)
{
	IWDG->KR = 0x00005555;
	IWDG->PR = 0x07;		//reset = 0x0000 /1
	IWDG->RLR = 0x0FFF;			//reset = 0x0FFF
	while (IWDG->SR != 0);
	IWDG->KR = 0x0000AAAA;	//reload
	IWDG->KR = 0x0000CCCC;	//enable
}


