/**
  ******************************************************************************
  * @file    Template_2/main.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Use this template for new projects with stm32f0xx family.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0x_gpio.h"
#include "stm32f0x_tim.h"
#include "stm32f0xx_it.h"
#include "hard.h"
#include "watchdog.h"


#include "core_cm0.h"
#include "adc.h"
#include "main_menu.h"
#include "lcd.h"



#include <stdio.h>
#include <string.h>




//--- VARIABLES EXTERNAS ---//
volatile unsigned char timer_1seg = 0;

volatile unsigned short timer_led_comm = 0;
volatile unsigned short timer_for_cat_switch = 0;
volatile unsigned short timer_for_cat_display = 0;
volatile unsigned char buffrx_ready = 0;
volatile unsigned char *pbuffrx;
volatile unsigned short wait_ms_var = 0;

volatile unsigned char *pspi_tx;
volatile unsigned char *pspi_rx;
volatile unsigned char spi_bytes_left = 0;

// ------- Externals de los timers -------
volatile unsigned short show_select_timer = 0;

volatile unsigned short scroll1_timer = 0;
volatile unsigned short scroll2_timer = 0;

volatile unsigned short lcd_backlight_timer = 0;

volatile unsigned char new_t = 0;
volatile unsigned short freq_tim = 0;

// ------- Externals del display LCD -------
const char s_blank_line [] = {"                "};
const char s_blank_line20 [] = {"                    "};

// ------- Externals del ADC -------
volatile unsigned char seq_ready = 0;

volatile unsigned short adc_ch[10];

#define V12V		adc_ch[0]
#define V6V			adc_ch[1]
#define V5V			adc_ch[2]
#define V3V			adc_ch[3]
#define VBATV		adc_ch[4]
#define I12V		adc_ch[5]
#define I6V			adc_ch[6]
#define I5V			adc_ch[7]
#define I3V			adc_ch[8]
#define IBATV		adc_ch[9]


//volatile unsigned char Packet_Detected_Flag;
//volatile unsigned char dmx_receive_flag = 0;
//volatile unsigned char DMX_channel_received = 0;
//volatile unsigned char DMX_channel_selected = 1;
//volatile unsigned char DMX_channel_quantity = 4;
//
//volatile unsigned char data1[512];
////static unsigned char data_back[10];
//volatile unsigned char data[10];
//
//volatile unsigned short prog_timer = 0;

//--- VARIABLES GLOBALES ---//


// ------- de los timers -------
volatile unsigned short timer_standby;
volatile unsigned short display_timer;
volatile unsigned char switches_timer;
volatile unsigned char filter_timer;

// ------- de los switches -------
unsigned short s1;

//volatile unsigned char door_filter;
//volatile unsigned char take_sample;
//volatile unsigned char move_relay;
//
//volatile unsigned char secs = 0;
//volatile unsigned short minutes = 0;
//

// ------- de los filtros y sensado -------

//#define LARGO_F		32
//#define DIVISOR_F	5
//unsigned char vd0 [LARGO_F + 1];
//unsigned char vd1 [LARGO_F + 1];
//unsigned char vd2 [LARGO_F + 1];
//unsigned char vd3 [LARGO_F + 1];
//unsigned char vd4 [LARGO_F + 1];

#define KV 0.0175
#define KI 0.0175
#define RELOAD_CNT	4
float fcalc = 1.0;



//--- FUNCIONES DEL MODULO ---//
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

void ConvertVoltage (unsigned short med, unsigned short *, unsigned short *);
void ConvertCurrent (unsigned short med, unsigned short *, unsigned short *);
void Show34Lines (unsigned char );
#define VOLTAGE 1
#define CURRENT 2

// ------- de los switches -------
void UpdateSwitches (void);
unsigned char CheckS1 (void);




//--- FIN DEFINICIONES DE FILTRO ---//


//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
	unsigned char i;
	unsigned char main_state = 0;
	unsigned short vlt_int, vlt_dec;
	unsigned short cur_int, cur_dec;
	char str [30];
	unsigned char cnt = 0;

	//!< At this stage the microcontroller clock setting is already configured,
    //   this is done through SystemInit() function which is called from startup
    //   file (startup_stm32f0xx.s) before to branch to application main.
    //   To reconfigure the default setting of SystemInit() function, refer to
    //   system_stm32f0xx.c file

	//GPIO Configuration.
	GPIO_Config();

	//ACTIVAR SYSTICK TIMER
	if (SysTick_Config(48000))
	{
		while (1)	/* Capture error */
		{
			if (LOAD_MOSFET)
				LOAD_MOSFET_OFF;
			else
				LOAD_MOSFET_ON;

			for (i = 0; i < 255; i++)
			{
				asm (	"nop \n\t"
						"nop \n\t"
						"nop \n\t" );
			}
		}
	}

	//TIM Configuration.
	//TIM_3_Init();		//input capture cuenta pulsos
//	TIM_14_Init();			//lo uso para detectar el break en el DMX
	//TIM_16_Init();		//para OneShoot() cuando funciona en modo master
	//TIM_17_Init();		//lo uso para el ADC de Igrid

	//--- Analog - to - Digital ---
	AdcConfig();

		//--- PRUEBA DISPLAY LCD ---
//		EXTIOff ();
	LCDInit();

		//--- Welcome code ---//
	Lcd_Command(CLEAR);
	Wait_ms(100);
	Lcd_Command(CURSOR_OFF);
	Wait_ms(100);
	Lcd_Command(BLINK_OFF);
	Wait_ms(100);

	while (FuncShowBlink ((const char *) "  Kirno Technology  ", (const char *) "Banco Pruebas Cardio", 2, BLINK_NO) != RESP_FINISH);
	while (FuncShowBlink ((const char *) "  Hardware: V1.0    ", (const char *) "  Software: V1.0    ", 1, BLINK_CROSS) != RESP_FINISH);

//	//--- Comienzo prueba de medicion de tensiones ---//
//	while (1)
//	{
//		Show34Lines(VOLTAGE);
//		Wait_ms(300);
//	}
//	//--- Fin prueba de medicion de tensiones ---//
//
//	//--- Comienzo prueba de medicion de corrientes ---//
//	while (1)
//	{
//		Show34Lines(CURRENT);
//		Wait_ms(300);
//	}
//	//--- Fin prueba de medicion de corrientes ---//

//		while (1)
//		{
//			if (CheckS1() > S_NO)
//			{
//				LCD_3ER_RENGLON;
////				LCDTransmitStr((const char *) "RELAY_INT -> ON     ");
////				RELAY_INT_ON;
////				LCDTransmitStr((const char *) "RELAY_EXT -> ON     ");
////				RELAY_EXT_ON;
////				LCDTransmitStr((const char *) "LOAD_BAT -> ON     ");
////				LOAD_BAT_ON;
//				LCDTransmitStr((const char *) "LOAD_MOSFET -> ON     ");
//				LOAD_MOSFET_ON;
//
//				new_t = 0;
//			}
//			else if (!new_t)
//			{
//				new_t = 1;
//				LCD_3ER_RENGLON;
////				LCDTransmitStr((const char *) "RELAY_INT -> OFF    ");
////				RELAY_INT_OFF;
////				LCDTransmitStr((const char *) "RELAY_EXT -> OFF    ");
////				RELAY_EXT_OFF;
////				LCDTransmitStr((const char *) "LOAD_BAT -> OFF    ");
////				LOAD_BAT_OFF;
//				LCDTransmitStr((const char *) "LOAD_MOSFET -> OFF    ");
//				LOAD_MOSFET_OFF;
//
//
//			}
//
//			 UpdateSwitches ();
//
//		}


	 //PRUEBA LED Y OE

	//--- COMIENZO PROGRAMA DE PRODUCCION



	 //--- Main loop ---//
	LOAD_MOSFET_OFF;
	LOAD_BAT_OFF;
	RELAY_EXT_OFF;
	RELAY_INT_OFF;

	 while (1)
	 {
		 switch (main_state)
		 {
			 case SYNC_SAMPLES:
				 seq_ready = 0;
				 ADC1->CR |= ADC_CR_ADSTART;


				 LCD_1ER_RENGLON;
				 LCDTransmitStr((const char *) " Listo Para Empezar ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr(s_blank_line20);
				 LCD_3ER_RENGLON;
				 LCDTransmitStr((const char *) "  Kirno Technology  ");
				 LCD_4TO_RENGLON;
				 LCDTransmitStr((const char *) "  Banco de Pruebas  ");

				 main_state++;

				 break;

			 case SYNC_SAMPLES_1:
				 if (seq_ready)		//deberia estar en sync
				 {
					 seq_ready = 0;
					 if ((!display_timer) && (CheckS1() > S_NO))
					 {
						 main_state++;
						 display_timer = 2000;
					 }
				 }
				 break;

			 case BAT_INT_WITHOUT_220:
				 LCD_1ER_RENGLON;
				 LCDTransmitStr((const char *) "BAT INT descon 220V!");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "Tensiones sin Carga ");
				 LCD_3ER_RENGLON;
				 LCDTransmitStr(s_blank_line20);
				 LCD_4TO_RENGLON;
				 LCDTransmitStr(s_blank_line20);

				 LOAD_MOSFET_OFF;
				 LOAD_BAT_OFF;
				 RELAY_EXT_OFF;
				 RELAY_INT_ON;
				 main_state++;
				 break;

			 case BAT_INT_WITHOUT_220_1:
				 if (!timer_standby)
				 {
					 Show34Lines (VOLTAGE);
					 timer_standby = 200;
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state++;
					 display_timer = 2000;
				 }
				 break;

			 case BAT_INT_WITHOUT_220_LOAD:
//				 LCD_1ER_RENGLON;
//				 LCDTransmitStr((const char *) "BAT INT descon 220V ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "  V e I con Carga   ");
				 LOAD_MOSFET_ON;
				 main_state++;
				 cnt = RELOAD_CNT;
				 break;

			 case BAT_INT_WITHOUT_220_1_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (VOLTAGE);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state++;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = BAT_EXT_WITHOUT_220;
					 display_timer = 2000;
				 }
				 break;

			 case BAT_INT_WITHOUT_220_2_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (CURRENT);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state--;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = BAT_EXT_WITHOUT_220;
					 display_timer = 2000;
				 }
				 break;

			 case BAT_EXT_WITHOUT_220:
				 LCD_1ER_RENGLON;
				 LCDTransmitStr((const char *) "BAT EXT descon 220V!");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "Tensiones sin Carga ");
				 LCD_3ER_RENGLON;
				 LCDTransmitStr(s_blank_line20);
				 LCD_4TO_RENGLON;
				 LCDTransmitStr(s_blank_line20);

				 LOAD_MOSFET_OFF;
				 LOAD_BAT_OFF;
				 RELAY_INT_OFF;
				 RELAY_EXT_ON;
				 main_state++;
				 break;

			 case BAT_EXT_WITHOUT_220_1:
				 if (!timer_standby)
				 {
					 Show34Lines (VOLTAGE);
					 timer_standby = 200;
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state++;
					 display_timer = 2000;
				 }
				 break;

			 case BAT_EXT_WITHOUT_220_LOAD:
//				 LCD_1ER_RENGLON;
//				 LCDTransmitStr((const char *) "BAT INT descon 220V ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "  V e I con Carga   ");
				 LOAD_MOSFET_ON;
				 cnt = RELOAD_CNT;
				 main_state++;
				 break;

			 case BAT_EXT_WITHOUT_220_1_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (VOLTAGE);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state++;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = MAINS;
					 display_timer = 2000;
				 }
				 break;

			 case BAT_EXT_WITHOUT_220_2_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (CURRENT);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state--;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = MAINS;
					 display_timer = 2000;
				 }
				 break;

			 case MAINS:
				 LCD_1ER_RENGLON;
				 LCDTransmitStr((const char *) "   Conectar 220V    ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "Tensiones sin Carga ");
				 LCD_3ER_RENGLON;
				 LCDTransmitStr(s_blank_line20);
				 LCD_4TO_RENGLON;
				 LCDTransmitStr(s_blank_line20);

				 LOAD_MOSFET_OFF;
				 LOAD_BAT_OFF;
				 RELAY_INT_OFF;
				 RELAY_EXT_OFF;
				 main_state++;
				 break;

			 case MAINS_1:
				 if (!timer_standby)
				 {
					 Show34Lines (VOLTAGE);
					 timer_standby = 200;
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state++;
					 display_timer = 2000;
				 }
				 break;

			 case MAINS_LOAD:
//				 LCD_1ER_RENGLON;
//				 LCDTransmitStr((const char *) "BAT INT descon 220V ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "  V e I con Carga   ");
				 LOAD_MOSFET_ON;
				 cnt = RELOAD_CNT;
				 main_state++;
				 break;

			 case MAINS_1_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (VOLTAGE);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state++;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = CHARGER;
					 display_timer = 2000;
				 }
				 break;

			 case MAINS_2_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (CURRENT);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state--;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = CHARGER;
					 display_timer = 2000;
				 }
				 break;

			 case CHARGER:
				 LCD_1ER_RENGLON;
				 LCDTransmitStr((const char *) " Carga BAT con 220V ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "  V e I sin Carga   ");
				 LCD_3ER_RENGLON;
				 LCDTransmitStr(s_blank_line20);
				 LCD_4TO_RENGLON;
				 LCDTransmitStr(s_blank_line20);

				 LOAD_MOSFET_OFF;
				 LOAD_BAT_OFF;
				 RELAY_INT_OFF;
				 RELAY_EXT_OFF;
				 main_state++;
				 break;

			 case CHARGER_1:
				 if (!timer_standby)
				 {
					 LCD_3ER_RENGLON;
					 ConvertVoltage (VBATV, &vlt_int, &vlt_dec);
					 ConvertCurrent (IBATV, &cur_int, &cur_dec);
					 sprintf(str, "VBAT=%2d.%02d I=%1d.%03d", vlt_int, vlt_dec, cur_int, cur_dec);
					 LCDTransmitStr(str);

					 timer_standby = 200;
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state++;
					 display_timer = 2000;
				 }
				 break;

			 case CHARGER_2:
//				 LCD_1ER_RENGLON;
//				 LCDTransmitStr((const char *) " Carga BAT con 220V ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) "  V e I con Carga   ");
				 LOAD_MOSFET_OFF;
				 LOAD_BAT_ON;
				 RELAY_INT_OFF;
				 RELAY_EXT_OFF;
				 main_state++;
				 break;

			 case CHARGER_3:
				 if (!timer_standby)
				 {
					 LCD_3ER_RENGLON;
					 ConvertVoltage (VBATV, &vlt_int, &vlt_dec);
					 ConvertCurrent (IBATV, &cur_int, &cur_dec);
					 sprintf(str, "VBAT=%2d.%02d I=%1d.%03d", vlt_int, vlt_dec, cur_int, cur_dec);
					 LCDTransmitStr(str);

					 timer_standby = 200;
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = START_LOAD;
					 display_timer = 2000;
				 }
				 break;

			 case START_LOAD:
				 LCD_1ER_RENGLON;
				 LCDTransmitStr((const char *) " Verificar Arranque ");
				 LCD_2DO_RENGLON;
				 LCDTransmitStr((const char *) " con 220V Carga BIAS");
				 LCD_3ER_RENGLON;
				 LCDTransmitStr(s_blank_line20);
				 LCD_4TO_RENGLON;
				 LCDTransmitStr(s_blank_line20);

				 LOAD_MOSFET_ON;
				 LOAD_BAT_OFF;
				 RELAY_INT_OFF;
				 RELAY_EXT_OFF;
				 cnt = RELOAD_CNT;
				 main_state++;
				 break;

			 case START_1_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (VOLTAGE);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state++;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = SYNC_SAMPLES;
					 display_timer = 2000;
				 }
				 break;

			 case START_2_LOAD:
				 if (!timer_standby)
				 {
					 Show34Lines (CURRENT);
					 timer_standby = 200;
					 if (cnt)
						 cnt--;
					 else
					 {
						 main_state--;
						 cnt = RELOAD_CNT;
					 }
				 }

				 if ((!display_timer) && (CheckS1() > S_NO))
				 {
					 main_state = SYNC_SAMPLES;
					 display_timer = 2000;
				 }
				 break;

			 default:
				 main_state = SYNC_SAMPLES;
				 break;
		 }

		 //inicio cuestiones particulares
		 //iniciar variables de usao del programa segun funcion de memoria
		 UpdateSwitches ();

#ifdef WITH_HARDWARE_WATCHDOG
		KickWatchdog();
#endif

	 }	//termina while(1)

	return 0;
}


//--- End of Main ---//
void Update_PWM (unsigned short pwm)
{
	Update_TIM3_CH1 (pwm);
	Update_TIM3_CH2 (4095 - pwm);
}

//muestra en el 3ER y 4TO renglon tension o corriente
//recibe variable VOLTAGE o CURRENT
void Show34Lines (unsigned char wich)
{
	char str [30];
	unsigned short vlt_int, vlt_dec;
	unsigned short vlt2_int, vlt2_dec;
	unsigned short cur_int, cur_dec;
	unsigned short cur2_int, cur2_dec;


	if (wich == VOLTAGE)
	{
		 LCD_3ER_RENGLON;
		 ConvertVoltage (V12V, &vlt_int, &vlt_dec);
		 ConvertVoltage (V5V, &vlt2_int, &vlt2_dec);
		 sprintf(str, "V12=%2d.%02d V5V=%2d.%02d", vlt_int, vlt_dec, vlt2_int, vlt2_dec);
		 LCDTransmitStr(str);

		 LCD_4TO_RENGLON;
		 ConvertVoltage (V6V, &vlt_int, &vlt_dec);
		 ConvertVoltage (V3V, &vlt2_int, &vlt2_dec);
		 sprintf(str, "V6V=%2d.%02d V3V=%2d.%02d", vlt_int, vlt_dec, vlt2_int, vlt2_dec);
		 LCDTransmitStr(str);
	}

	if (wich == CURRENT)
	{
		 LCD_3ER_RENGLON;
		 ConvertCurrent (I12V, &cur_int, &cur_dec);
		 ConvertCurrent (I5V, &cur2_int, &cur2_dec);
		 sprintf(str, "I12=%1d.%03d I5V=%1d.%03d", cur_int, cur_dec, cur2_int, cur2_dec);
		 LCDTransmitStr(str);

		 LCD_4TO_RENGLON;
		 ConvertCurrent (I6V, &cur_int, &cur_dec);
		 ConvertCurrent (I3V, &cur2_int, &cur2_dec);
		 sprintf(str, "I6V=%1d.%03d I3V=%1d.%03d", cur_int, cur_dec, cur2_int, cur2_dec);
		 LCDTransmitStr(str);
	}
}

//Convierte los valores de tension
//recibe el valor de tension a convertir (short) y dos punteros al valor entero y decimal (short)
void ConvertVoltage (unsigned short med, unsigned short * ent, unsigned short * dec)
{
	 fcalc = med * KV;
	 *ent = (short) fcalc;
	 fcalc = fcalc - *ent;
	 fcalc = fcalc * 100;
	 *dec = (short) fcalc;
}

//Convierte los valores de corriente
//recibe el valor de corriente a convertir (short) y dos punteros al valor entero y decimal (short)
void ConvertCurrent (unsigned short med, unsigned short * ent, unsigned short * dec)
{
	 fcalc = med * KI;
	 *ent = (short) fcalc;
	 fcalc = fcalc - *ent;
	 fcalc = fcalc * 100;
	 *dec = (short) fcalc;
}

void UpdateSwitches (void)
{
	//revisa los switches cada 10ms
	if (!switches_timer)
	{
		if (S1_PIN)
			s1++;
		else if (s1 > 50)
			s1 -= 50;
		else if (s1 > 10)
			s1 -= 5;
		else if (s1)
			s1--;

		switches_timer = SWITCHES_TIMER_RELOAD;
	}
}

unsigned char CheckS1 (void)	//cada check tiene 10ms
{
	if (s1 > SWITCHES_THRESHOLD_FULL)
		return S_FULL;

	if (s1 > SWITCHES_THRESHOLD_HALF)
		return S_HALF;

	if (s1 > SWITCHES_THRESHOLD_MIN)
		return S_MIN;

	return S_NO;
}




void TimingDelay_Decrement(void)
{
	if (wait_ms_var)
		wait_ms_var--;

	if (display_timer)
		display_timer--;

	if (timer_standby)
		timer_standby--;

	if (switches_timer)
		switches_timer--;

	//-------- Timers para funciones de seleccion ---------//
	if (show_select_timer)
		show_select_timer--;

	if (scroll1_timer)
		scroll1_timer--;

	if (scroll2_timer)
		scroll2_timer--;


}

