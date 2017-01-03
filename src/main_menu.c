/*
 * main_menu.c
 *
 *  Created on: 11/02/2016
 *      Author: Mariano
 */

#include "main_menu.h"
#include "hard.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

//#include "funcs_manual.h"

/* Externals variables ---------------------------------------------------------*/
//extern volatile unsigned short mainmenu_timer;
extern volatile unsigned short show_select_timer;

extern volatile unsigned short scroll1_timer;
extern volatile unsigned short scroll2_timer;

extern const char s_blank_line [];

extern volatile unsigned short lcd_backlight_timer;

/* Global variables ------------------------------------------------------------*/
enum var_mainmenu_states mainmenu_state = MAINMENU_INIT;
unsigned char mainmenu_repeat = 0;
unsigned char show_select_state = 0;
unsigned char options_state = 0;
unsigned char options_curr_sel = 0;

unsigned char scroll1_state = 0;
unsigned char scroll1_last_window = 0;
unsigned char scroll1_current_window = 0;

unsigned char scroll2_state = 0;
unsigned char scroll2_last_window = 0;
unsigned char scroll2_current_window = 0;

unsigned char blinking_state = 0;
unsigned char blinking_how_many = 0;

unsigned char change_state = 0;
unsigned short change_current_val = 0;
unsigned char change_last_option = 0;

const unsigned char s_sel_up_down [] = { 0x02, 0x08, 0x0f };
//-------- Functions -------------

//funcion de seleccion del menu principal
//devuelve nueva selecci�n o estado anterior
unsigned char FuncMainMenu (void)
{
	unsigned char resp = RESP_CONTINUE;
	unsigned char resp_down = 0;

	switch (mainmenu_state)
	{
		case MAINMENU_INIT:
			resp_down = FuncShowBlink ((const char *) "    Entering    ", (const char *) "   Main Menu    ", 3, BLINK_DIRECT);

			if (resp_down == RESP_FINISH)
			{
				FuncShowSelectv2Reset();
				mainmenu_state = MAINMENU_SHOW_MANUAL;
			}


			break;

		case MAINMENU_SHOW_MANUAL:
			resp_down = FuncShowSelectv2((const char *) "  MANUAL DIMMER ");


			//if (resp_down == RESP_CHANGE_DWN)
			//	mainmenu_state = MAINMENU_SHOW_BRD_DIAG;

			if (resp_down == RESP_CHANGE_DWN)
				mainmenu_state = MAINMENU_SHOW_COLORS;

			if (resp_down == RESP_CHANGE_UP)
				mainmenu_state = MAINMENU_SHOW_DMX;

			if (resp_down == RESP_SELECTED)
				mainmenu_state = MAINMENU_SHOW_MANUAL_SELECTED;

			if (resp_down == RESP_CHANGE_ALL_UP)
				mainmenu_state = MAINMENU_INIT;

			break;

		case MAINMENU_SHOW_DMX:
			resp_down = FuncShowSelectv2((const char *) "   DMX DIMMER   ");

			if (resp_down == RESP_CHANGE_DWN)
				mainmenu_state = MAINMENU_SHOW_MANUAL;

			if (resp_down == RESP_CHANGE_UP)
				mainmenu_state = MAINMENU_SHOW_COLORS;

			if (resp_down == RESP_SELECTED)
				mainmenu_state = MAINMENU_SHOW_DMX_SELECTED;

			if (resp_down == RESP_CHANGE_ALL_UP)
				mainmenu_state = MAINMENU_INIT;

			break;

		case MAINMENU_SHOW_COLORS:
			resp_down = FuncShowSelectv2((const char *) "   SET COLORS   ");

			if (resp_down == RESP_CHANGE_DWN)
				mainmenu_state = MAINMENU_SHOW_DMX;

			//if (resp_down == RESP_CHANGE_UP)
			//	mainmenu_state = MAINMENU_SHOW_BRD_DIAG;

			if (resp_down == RESP_CHANGE_UP)
				mainmenu_state = MAINMENU_SHOW_MANUAL;

			if (resp_down == RESP_SELECTED)
				mainmenu_state = MAINMENU_SHOW_COLORS_SELECTED;

			if (resp_down == RESP_CHANGE_ALL_UP)
				mainmenu_state = MAINMENU_INIT;

			break;

		case MAINMENU_SHOW_BRD_DIAG:
			resp_down = FuncShowSelectv2((const char *) "Brd Diagnostics ");

			if (resp_down == RESP_CHANGE_DWN)
				mainmenu_state = MAINMENU_SHOW_COLORS;

			if (resp_down == RESP_CHANGE_UP)
				mainmenu_state = MAINMENU_SHOW_MANUAL;

			if (resp_down == RESP_SELECTED)
				mainmenu_state = MAINMENU_SHOW_BRD_DIAG_SELECTED;

			if (resp_down == RESP_CHANGE_ALL_UP)
				mainmenu_state = MAINMENU_INIT;

			break;

		case MAINMENU_SHOW_MANUAL_SELECTED:
			FuncShowBlink ((const char *) "Manual Mode     ", (const char *) "Selected...     ", 0, BLINK_NO);
			mainmenu_state++;
			break;

		case MAINMENU_SHOW_MANUAL_SELECTED_1:
			if (CheckSSel() == S_NO)
			{
				resp = MAINMENU_SHOW_MANUAL_SELECTED;
				mainmenu_state = MAINMENU_INIT;
			}
			break;

		case MAINMENU_SHOW_DMX_SELECTED:
			FuncShowBlink ((const char *) "DMX Mode        ", (const char *) "Selected...     ", 0, BLINK_NO);
			mainmenu_state++;
			break;

		case MAINMENU_SHOW_DMX_SELECTED_1:
			if (CheckSSel() == S_NO)
			{
				resp = MAINMENU_SHOW_DMX_SELECTED;
				mainmenu_state = MAINMENU_INIT;
			}
			break;

		case MAINMENU_SHOW_COLORS_SELECTED:
			FuncShowBlink ((const char *) "Color Temp. Mode", (const char *) "Selected...     ", 0, BLINK_NO);
			mainmenu_state++;
			break;

		case MAINMENU_SHOW_COLORS_SELECTED_1:
			if (CheckSSel() == S_NO)
			{
				resp = MAINMENU_SHOW_COLORS_SELECTED;
				mainmenu_state = MAINMENU_INIT;
			}
			break;

		case MAINMENU_SHOW_BRD_DIAG_SELECTED:
			FuncShowBlink ((const char *) "Brd Diagnostics ", (const char *) "Selected...     ", 0, BLINK_NO);
			mainmenu_state++;
			break;

		case MAINMENU_SHOW_BRD_DIAG_SELECTED_1:
			if (CheckSSel() == S_NO)
			{
				resp = MAINMENU_SHOW_COLORS_SELECTED;
				mainmenu_state = MAINMENU_INIT;
			}
			break;

		default:
			mainmenu_state = MAINMENU_INIT;
			break;
	}

	//si alguien toco un control prendo el lcd_backlight
	if ((CheckSUp() > S_NO) || (CheckSDown() > S_NO) || (CheckSSel() > S_NO))
		lcd_backlight_timer = TT_LCD_BACKLIGHT;

	return resp;
}

//funcion que muestra los string de renglon 1 y renglon 2
//recibe puntero a primer renglon
//puntero a segundo renglon
//how many cantidad de pantallas en blinking
//modo BLINK_DIRECT o BLINK_CROSS
//si modo es BLINK_NO en how_many me pasan los segundos antes de terminar
unsigned char FuncShowBlink (const char * p_text1, const char * p_text2, unsigned char how_many, unsigned char mode)
{
    unsigned char resp = RESP_CONTINUE;

    switch (blinking_state)
    {
        case BLINKING_INIT:
            if (mode == BLINK_DIRECT)
                blinking_state = BLINKING_MARK_D;
            else if (mode == BLINK_CROSS)
                blinking_state = BLINKING_MARK_C;
            else
                blinking_state = BLINKING_MARK_N;

//            if (!how_many)
//                how_many = 1;

            blinking_how_many = how_many;
            show_select_timer = 0;
            break;

        case BLINKING_MARK_D:
            if (!show_select_timer)
            {
                if (blinking_how_many)
                {
                	blinking_how_many--;
                    LCD_1ER_RENGLON;
                    LCDTransmitStr(p_text1);
                    LCD_2DO_RENGLON;
                    LCDTransmitStr(p_text2);
                    show_select_timer = 1000;
                    blinking_state = BLINKING_SPACE_D;
                }
                else
                {
                	blinking_state = BLINKING_INIT;
                	resp = RESP_FINISH;
                }
            }
            break;

        case BLINKING_SPACE_D:
            if (!show_select_timer)
            {
                LCD_1ER_RENGLON;
                LCDTransmitStr((const char *)s_blank_line);
                LCD_2DO_RENGLON;
                LCDTransmitStr((const char *)s_blank_line);
                show_select_timer = 600;
                blinking_state = BLINKING_MARK_D;
            }
            break;

        case BLINKING_MARK_C:
            if (!show_select_timer)
            {
                if (blinking_how_many)
                {
                	blinking_how_many--;
                    LCD_1ER_RENGLON;
                    LCDTransmitStr(p_text1);
                    LCD_2DO_RENGLON;
                    LCDTransmitStr((const char *)s_blank_line);
                    show_select_timer = 1000;
                    blinking_state = BLINKING_SPACE_C;
                }
                else
                {
                    LCD_1ER_RENGLON;
                    LCDTransmitStr((const char *) s_blank_line);
                    blinking_state = BLINKING_INIT;
                    resp = RESP_FINISH;
                }
            }
            break;

        case BLINKING_SPACE_C:
            if (!show_select_timer)
            {
                LCD_1ER_RENGLON;
                LCDTransmitStr((const char *) s_blank_line);
                LCD_2DO_RENGLON;
                LCDTransmitStr(p_text2);
                show_select_timer = 1000;
                blinking_state = BLINKING_MARK_C;
            }
            break;

        case BLINKING_MARK_N:
            LCD_1ER_RENGLON;
            LCDTransmitStr(p_text1);
            LCD_2DO_RENGLON;
            LCDTransmitStr(p_text2);
            show_select_timer = 1000 * how_many;
            blinking_state = BLINKING_SPACE_N;
            break;

        case BLINKING_SPACE_N:
            if (!show_select_timer)
            {
            	blinking_state = BLINKING_INIT;
                resp = RESP_FINISH;
            }
            break;

        default:
        	blinking_state = BLINKING_INIT;
            resp = RESP_FINISH;
            break;
    }
    return resp;
}

//funcion que muestra el string enviado en formato de menu
unsigned char FuncShowSelect (const char * p_text)
{
	unsigned char resp = RESP_CONTINUE;

	switch (show_select_state)
	{
		case SHOW_SELECT_INIT:
			show_select_state++;
			break;

		case SHOW_SELECT_1:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			show_select_timer = 1000;
			show_select_state++;
			break;

		case SHOW_SELECT_2:
			if (!show_select_timer)
			{
				LCD_1ER_RENGLON;
				LCDTransmitStr((const char *)s_blank_line);
				show_select_timer = 500;
				show_select_state++;
			}

			//check s_down, s_up y s_sel
			if (CheckSDown() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_DWN;

			if (CheckSUp() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_UP;

			if (CheckSSel() > S_NO)
				show_select_state = SHOW_SELECT_SELECTED;

			break;

		case SHOW_SELECT_3:
			if (!show_select_timer)
			{
				show_select_state = SHOW_SELECT_1;
			}

			//check s_down, s_up y s_sel
			if (CheckSDown() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_DWN;

			if (CheckSUp() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_UP;

			if (CheckSSel() > S_NO)
				show_select_state = SHOW_SELECT_SELECTED;

			break;

		case SHOW_SELECT_SELECTED:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "Selected...     ");
			show_select_state++;
			break;

		case SHOW_SELECT_SELECTED_1:
			if (CheckSSel() == S_NO)
			{
				resp = RESP_SELECTED;
				show_select_state = SHOW_SELECT_INIT;
			}
			break;

		case SHOW_SELECT_CHANGE_DWN:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "Changing Down...");
			show_select_state++;
			break;

		case SHOW_SELECT_CHANGE_DWN_1:
			if (CheckSDown() == S_NO)
			{
				resp = RESP_CHANGE_DWN;
				show_select_state = SHOW_SELECT_INIT;
			}

			if (CheckSDown() > S_HALF)
			{
				resp = RESP_CHANGE_ALL_UP;
				show_select_state = SHOW_SELECT_INIT;
			}
			break;

		case SHOW_SELECT_CHANGE_UP:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "Changing Up...  ");
			show_select_state++;
			break;

		case SHOW_SELECT_CHANGE_UP_1:
			if (CheckSUp() == S_NO)
			{
				resp = RESP_CHANGE_UP;
				show_select_state = SHOW_SELECT_INIT;
			}

			if (CheckSUp() > S_HALF)
			{
				resp = RESP_CHANGE_ALL_UP;
				show_select_state = SHOW_SELECT_INIT;
			}
			break;

		default:
			show_select_state = SHOW_SELECT_INIT;
			break;
	}

	return resp;
}

void FuncShowSelectv2Reset (void)
{
	show_select_state = SHOW_SELECT_INIT;
}

//funcion que muestra el string enviado en formato de menu
//ademas v2 agrega B1:- B2:+ B3:SEL en el segundo renglon
unsigned char FuncShowSelectv2 (const char * p_text)
{
	unsigned char resp = RESP_CONTINUE;

	switch (show_select_state)
	{
		case SHOW_SELECT_INIT:
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "B1:- B2:+ B3:SEL");
			show_select_state++;
			break;

		case SHOW_SELECT_1:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			show_select_timer = 1000;
			show_select_state++;
			break;

		case SHOW_SELECT_2:
			if (!show_select_timer)
			{
				LCD_1ER_RENGLON;
				LCDTransmitStr((const char *) s_blank_line);
				show_select_timer = 500;
				show_select_state++;
			}

			//check s_down, s_up y s_sel
			if (CheckSDown() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_DWN;

			if (CheckSUp() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_UP;

			if (CheckSSel() > S_NO)
				show_select_state = SHOW_SELECT_SELECTED;

			break;

		case SHOW_SELECT_3:
			if (!show_select_timer)
			{
				show_select_state = SHOW_SELECT_1;
			}

			//check s_down, s_up y s_sel
			if (CheckSDown() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_DWN;

			if (CheckSUp() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_UP;

			if (CheckSSel() > S_NO)
				show_select_state = SHOW_SELECT_SELECTED;

			break;

		case SHOW_SELECT_SELECTED:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "Selected...     ");
			show_select_state++;
			break;

		case SHOW_SELECT_SELECTED_1:
			if (CheckSSel() == S_NO)
			{
				resp = RESP_SELECTED;
				show_select_state = SHOW_SELECT_INIT;
			}
			break;

		case SHOW_SELECT_CHANGE_DWN:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "Changing Down...");
			show_select_state++;
			break;

		case SHOW_SELECT_CHANGE_DWN_1:
			if (CheckSDown() == S_NO)
			{
				resp = RESP_CHANGE_DWN;
				show_select_state = SHOW_SELECT_INIT;
			}

			if (CheckSDown() > S_HALF)
			{
				resp = RESP_CHANGE_ALL_UP;
				show_select_state = SHOW_SELECT_INIT;
			}
			break;

		case SHOW_SELECT_CHANGE_UP:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "Changing Up...  ");
			show_select_state++;
			break;

		case SHOW_SELECT_CHANGE_UP_1:
			if (CheckSUp() == S_NO)
			{
				resp = RESP_CHANGE_UP;
				show_select_state = SHOW_SELECT_INIT;
			}

			if (CheckSUp() > S_HALF)
			{
				resp = RESP_CHANGE_ALL_UP;
				show_select_state = SHOW_SELECT_INIT;
			}
			break;

		default:
			show_select_state = SHOW_SELECT_INIT;
			break;
	}

	return resp;
}

//funcion que muestra el string enviado en formato de menu
//ademas v2 agrega B1:- B2:+ B3:SEL en el segundo renglon
//ademas v3 no traba la seleccion sino que responde al toque con lo elegido || NECESITO RESET EXTERNO
unsigned char FuncShowSelectv3 (const char * p_text)
{
	unsigned char resp = RESP_CONTINUE;

	switch (show_select_state)
	{
		case SHOW_SELECT_INIT:
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "B1:- B2:+ B3:SEL");
			show_select_state++;
			break;

		case SHOW_SELECT_1:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text);
			show_select_timer = 1000;
			show_select_state++;
			break;

		case SHOW_SELECT_2:
			if (!show_select_timer)
			{
				LCD_1ER_RENGLON;
				LCDTransmitStr((const char *) s_blank_line);
				show_select_timer = 500;
				show_select_state++;
			}

			//check s_down, s_up y s_sel
			if (CheckSDown() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_DWN;

			if (CheckSUp() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_UP;

			if (CheckSSel() > S_NO)
				show_select_state = SHOW_SELECT_SELECTED;

			break;

		case SHOW_SELECT_3:
			if (!show_select_timer)
			{
				show_select_state = SHOW_SELECT_1;
			}

			//check s_down, s_up y s_sel
			if (CheckSDown() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_DWN;

			if (CheckSUp() > S_NO)
				show_select_state = SHOW_SELECT_CHANGE_UP;

			if (CheckSSel() > S_NO)
				show_select_state = SHOW_SELECT_SELECTED;

			break;

		case SHOW_SELECT_SELECTED:
			resp = RESP_SELECTED;
			show_select_state = SHOW_SELECT_INIT;
			break;

		case SHOW_SELECT_CHANGE_DWN:
			show_select_state = SHOW_SELECT_INIT;
			resp = RESP_CHANGE_DWN;
			break;


		case SHOW_SELECT_CHANGE_UP:
			resp = RESP_CHANGE_UP;
			show_select_state = SHOW_SELECT_INIT;
			break;

		default:
			show_select_state = SHOW_SELECT_INIT;
			break;
	}

	return resp;
}

//recibe el primer renglon y el segundo
//recibe un puntero a las posiciones de memoria de los asteriscos
//recibe una variable de catidad de opciones y otra variable con la primera opcion a la que apunta
//devuelve RESP_CONTINUE o (RESP_SELECTED | opcion_elegida << 4)
unsigned char FuncOptions (const char * p_text1, const char * p_text2, unsigned char * p_sel, unsigned char opts, unsigned char first_option)
{
	unsigned char resp = RESP_CONTINUE;

	if (first_option & 0x80)	//me piden que elija una opcion especial
	{
		if ((first_option & 0x7F) < opts)	//no esta fuera de escala
		{
			if (options_state == OPTIONS_INIT)
				options_curr_sel = (first_option & 0x7F);
			else
			{
				if (options_curr_sel != (first_option & 0x7F)) //fuerzo el cambio, ya estaba mostrando la pantalla
					options_state = OPTIONS_CHANGE_SELECT_UP;	//TODO: no entiendo ue caraj hago aca!!!!
			}
		}
		else
			options_curr_sel = 0;
	}

	switch (options_state)
	{
		case OPTIONS_INIT:
			LCD_1ER_RENGLON;
			LCDTransmitStr(p_text1);
			LCD_2DO_RENGLON;
			LCDTransmitStr(p_text2);
			show_select_timer = 0;
			options_state++;
			break;

		case OPTIONS_WAIT_SELECT:
			if (!show_select_timer)
			{
				Lcd_SetDDRAM(*(p_sel + options_curr_sel));
				LCDTransmitStr("*");
				show_select_timer = 1000;
				options_state++;
			}
			break;

		case OPTIONS_WAIT_SELECT_1:
			if (CheckSDown() > S_NO)
				options_state = OPTIONS_CHANGE_SELECT_DWN;

			if (CheckSUp() > S_NO)
				options_state = OPTIONS_CHANGE_SELECT_UP;

			if (CheckSSel() > S_NO)
			{
				resp = (RESP_SELECTED | (options_curr_sel << 4));
				options_state = OPTIONS_INIT;
			}

			if (!show_select_timer)
			{
				Lcd_SetDDRAM(*(p_sel + options_curr_sel));
				LCDTransmitStr(" ");
				show_select_timer = 500;
				options_state = OPTIONS_WAIT_SELECT_3;
			}
			break;

		case OPTIONS_WAIT_SELECT_2:
			if ((CheckSDown() == S_NO) && (CheckSUp() == S_NO))
			{
				options_state = OPTIONS_WAIT_SELECT_1;
			}
			break;

		case OPTIONS_WAIT_SELECT_3:
			if (CheckSDown() > S_NO)
				options_state = OPTIONS_CHANGE_SELECT_DWN;

			if (CheckSUp() > S_NO)
				options_state = OPTIONS_CHANGE_SELECT_UP;

			if (CheckSSel() > S_NO)
			{
				resp = (RESP_SELECTED | (options_curr_sel << 4));
				options_state = OPTIONS_INIT;
			}

			if (!show_select_timer)
			{
				Lcd_SetDDRAM(*(p_sel + options_curr_sel));
				LCDTransmitStr("*");
				show_select_timer = 1000;
				options_state = OPTIONS_WAIT_SELECT_1;
			}
			break;

		case OPTIONS_CHANGE_SELECT_UP:
			Lcd_SetDDRAM(*(p_sel + options_curr_sel));
			LCDTransmitStr(" ");

			if (first_option & 0x80)	//me piden que elija una opcion especial
				options_curr_sel = (first_option & 0x7F);
			else
			{
				if (options_curr_sel < (opts - 1))
					options_curr_sel++;
				else
					options_curr_sel = 0;
			}

			Lcd_SetDDRAM(*(p_sel + options_curr_sel));
			LCDTransmitStr("*");

			options_state = OPTIONS_WAIT_SELECT_2;
			break;

		case OPTIONS_CHANGE_SELECT_DWN:
			Lcd_SetDDRAM(*(p_sel + options_curr_sel));
			LCDTransmitStr(" ");

			if (first_option & 0x80)	//me piden que elija una opcion especial
				options_curr_sel = (first_option & 0x7F);
			else
			{
				if (options_curr_sel > 0)	//recorre entre 0 y (opts - 1)
					options_curr_sel--;
				else
					options_curr_sel = (opts - 1);
			}

			Lcd_SetDDRAM(*(p_sel + options_curr_sel));
			LCDTransmitStr("*");

			options_state = OPTIONS_WAIT_SELECT_2;
			break;

		default:
			options_state = OPTIONS_INIT;
			break;
	}

	return resp;
}

void FuncOptionsReset (void)
{
	options_state = OPTIONS_INIT;
}

//recibe el valor original para arrancar seleccion
//recibe el modo CHANGE_PERCENT, CHANGE_SECS o CHANGE_CHANNELS Ademas puede tener |CHANGE_RESET
//recibe min val permitido, MAX val permitido
//devuelve RESP_CONTINUE o RESP_FINISH si termino la seleccion
unsigned char FuncChange (unsigned short * p_orig_value, unsigned char mode, unsigned short min_val, unsigned short max_val)
{
	unsigned char resp = RESP_CONTINUE;
	unsigned char resp_down = RESP_CONTINUE;
	char s_current [20];

	switch (change_state)
	{
		case CHANGE_INIT:
			change_current_val = *p_orig_value;
			change_last_option = 0;
			FuncOptionsReset();
			change_state++;
			break;

		case CHANGE_WAIT_SELECT:
			if (!scroll2_timer)
			{
				scroll2_timer = TT_UPDATE_BUTTON;
				memset(s_current, ' ', sizeof(s_current));
				if (mode == CHANGE_PERCENT)
				{
					sprintf(s_current, "chg  %3d", change_current_val);
					strcat(s_current, (const char*)"%   sel");
				}
				else if (mode == CHANGE_SECS)
				{
					sprintf(s_current, "chg %2d", change_current_val);
					strcat(s_current, (const char*)" secs sel");
				}
				else	//debe ser CHANNELS
				{
					sprintf(s_current, "chg   %3d", change_current_val);
					strcat(s_current, (const char*)"ch  sel");
				}

				resp_down = FuncOptions ((const char *) "up  down   done ", s_current,(unsigned char *) s_sel_up_down, 3, change_last_option);
				change_last_option = 0;

				if ((resp_down & 0x0f) == RESP_SELECTED)
				{
					resp_down = resp_down & 0xf0;
					resp_down >>= 4;
					if (resp_down == 0)
					{
						if (change_current_val < max_val)
							change_current_val++;

						resp = RESP_WORKING;
					}

					if (resp_down == 1)
					{
						if (change_current_val > min_val)
							change_current_val--;

						change_last_option = (1 | 0x80);	//fuerzo update de la opcion
						resp = RESP_WORKING;
					}

					if (resp_down == 2)
					{
						change_state = CHANGE_INIT;
						resp = RESP_FINISH;
						*p_orig_value = change_current_val;
					}
				}

			}
			break;

		default:
			change_state = CHANGE_INIT;
			break;
	}

	return resp;
}

//recibe el valor original para arrancar seleccion en p_orig_value
//recibe el modo CHANGE_CHANNELS o CHANGE_COLORS sino vuelve
//recibe min val permitido, MAX val permitido
//devuelve RESP_CONTINUE o RESP_FINISH si termino la seleccion
unsigned char FuncChangeThreeButtonsCHAR (unsigned char * p_orig_value, unsigned char mode, unsigned char min_val, unsigned char max_val)
{
	unsigned char resp = RESP_CONTINUE;
	unsigned char resp_down = RESP_CONTINUE;
	char s_current [20];

	switch (change_state)
	{
		case CHANGE_INIT:

			change_current_val = *p_orig_value;

			//chequeo errores
			if (change_current_val > max_val)
				change_current_val = max_val;
			else if (change_current_val < min_val)
				change_current_val = min_val;

			FuncOptionsReset();
			change_state++;
			break;

		case CHANGE_WAIT_SELECT:
			if (!scroll2_timer)		//timer para que la seleccion sea un poco mas lenta
			{						//pero si tuvo mucho tiempo apretado lo acelero al triple
				if ((CheckSUp() > S_HALF) || (CheckSDown() > S_HALF))
					scroll2_timer = TT_UPDATE_BUTTON_SPEED;
				else
					scroll2_timer = TT_UPDATE_BUTTON;

				memset(s_current, ' ', sizeof(s_current));
				if (mode == CHANGE_CHANNELS)
				{
					if (change_current_val <= 1)
					{
						strcpy(s_current, (const char *) "Channels: 1     ");
					}
					else
						strcpy(s_current, (const char *) "Channels: 2     ");
				}
				else if (mode == CHANGE_COLORS)
				{
					switch (change_current_val)
					{
						case 0:
							strcpy(s_current, (const char *) "Set Clr to 3200K");
							break;

						case 1:
							strcpy(s_current, (const char *) "Set Clr to 4500K");
							break;

						case 2:
							strcpy(s_current, (const char *) "Set Clr to 5600K");
							break;

						default:
							*p_orig_value = 2;
							strcpy(s_current, (const char *) "Set Clr to 5600K");
							break;
					}
				}
				else
				{
					change_state = CHANGE_INIT;
					return RESP_FINISH;
				}

				resp_down = FuncShowSelectv3 (s_current);

				if (resp_down == RESP_CHANGE_DWN)
				{
					if (change_current_val > min_val)
						change_current_val--;

					resp = RESP_WORKING;
				}

				if (resp_down == RESP_CHANGE_UP)
				{
					if (change_current_val < max_val)
						change_current_val++;

					resp = RESP_WORKING;
				}

				if (resp_down == RESP_SELECTED)
				{
					change_state = CHANGE_INIT;
					*p_orig_value = change_current_val;

					resp = RESP_FINISH;
				}
			}
			break;

		default:
			change_state = CHANGE_INIT;
			break;
	}

	return resp;
}

//recibe el valor original para arrancar seleccion en p_orig_value
//recibe el modo CHANGE_PERCENT, CHANGE_SECS o CHANGE_CHANNELS Ademas puede tener |CHANGE_RESET
//recibe min val permitido, MAX val permitido
//devuelve RESP_CONTINUE o RESP_FINISH si termino la seleccion
unsigned char FuncChangeThreeButtons (unsigned short * p_orig_value, unsigned char mode, unsigned short min_val, unsigned short max_val)
{
	unsigned char resp = RESP_CONTINUE;
	unsigned char resp_down = RESP_CONTINUE;
	char s_current [20];

	switch (change_state)
	{
		case CHANGE_INIT:

			change_current_val = *p_orig_value;

			//chequeo errores
			if (change_current_val > max_val)
				change_current_val = max_val;
			else if (change_current_val < min_val)
				change_current_val = min_val;

			FuncOptionsReset();
			change_state++;
			break;

		case CHANGE_WAIT_SELECT:
			if (!scroll2_timer)		//timer para que la seleccion sea un poco mas lenta
			{						//pero si tuvo mucho tiempo apretado lo acelero al triple
				if ((CheckSUp() > S_HALF) || (CheckSDown() > S_HALF))
					scroll2_timer = TT_UPDATE_BUTTON_SPEED;
				else
					scroll2_timer = TT_UPDATE_BUTTON;

				memset(s_current, ' ', sizeof(s_current));
				if (mode == CHANGE_CHANNELS)
				{
					if (change_current_val <= 1)
					{
						strcpy(s_current, (const char *) "Channels: 1     ");
						change_current_val = 1;
					}
					else
						strcpy(s_current, (const char *) "Channels: 2     ");
				}
				else	//debe ser ADDRESS
				{
					sprintf(s_current, "Address: %3d    ", change_current_val);
				}

				resp_down = FuncShowSelectv3 (s_current);

				if (resp_down == RESP_CHANGE_DWN)
				{
					if (change_current_val > min_val)
						change_current_val--;

					resp = RESP_WORKING;
				}

				if (resp_down == RESP_CHANGE_UP)
				{
					if (change_current_val < max_val)
						change_current_val++;

					resp = RESP_WORKING;
				}

				if (resp_down == RESP_SELECTED)
				{
					change_state = CHANGE_INIT;
					*p_orig_value = change_current_val;

					resp = RESP_FINISH;
				}
			}
			break;

		default:
			change_state = CHANGE_INIT;
			break;
	}

	return resp;
}

void FuncChangeReset (void)
{
	change_state = CHANGE_INIT;
}


//hace un scroll en el primer renglon del lcd
//recibe un puntero al string
//devuelve RESP_CONTINUE o RESP_FINISH
unsigned char FuncScroll1 (const char * p_text)
{
	unsigned char resp = RESP_CONTINUE;
	unsigned char last_window;
	unsigned char i;

	switch (scroll1_state)
	{
		case SCROLL_INIT:
			scroll1_last_window = strlen(p_text) + 32;
			scroll1_current_window = 1;
			scroll1_state++;
			break;

		case SCROLL_SENDING:
			if (!scroll1_timer)
			{
				last_window = scroll1_current_window + 16;
				LCD_1ER_RENGLON;

				for (i = scroll1_current_window; i < last_window; i++)
				{
					if (i < 16)
						LCDStartTransmit(' ');
					else if (i < (scroll1_last_window - 16))
						LCDStartTransmit(*(p_text + (i - 16)));
					else if (i < scroll1_last_window)
						LCDStartTransmit(' ');
					else
					{
						//termine de enviar
						i = last_window;
						scroll1_state++;
					}
				}
				scroll1_current_window++;
				scroll1_timer = 550;
			}
			break;

		case SCROLL_FINISH:
			resp = RESP_FINISH;
			scroll1_state = SCROLL_INIT;
			break;

		default:
			resp = RESP_FINISH;
			scroll1_state = SCROLL_INIT;
			break;
	}

	return resp;
}

//hace un scroll en el segundo renglon del lcd
//recibe un puntero al string
//devuelve RESP_CONTINUE o RESP_FINISH
unsigned char FuncScroll2 (const char * p_text)
{
	unsigned char resp = RESP_CONTINUE;
	unsigned char last_window;
	unsigned char i;

	switch (scroll2_state)
	{
		case SCROLL_INIT:
			scroll2_last_window = strlen(p_text) + 32;
			scroll2_current_window = 1;
			scroll2_state++;
			break;

		case SCROLL_SENDING:
			if (!scroll2_timer)
			{
				last_window = scroll2_current_window + 16;
				LCD_2DO_RENGLON;

				for (i = scroll2_current_window; i < last_window; i++)
				{
					if (i < 16)
						LCDStartTransmit(' ');
					else if (i < (scroll2_last_window - 16))
						LCDStartTransmit(*(p_text + (i - 16)));
					else if (i < scroll2_last_window)
						LCDStartTransmit(' ');
					else
					{
						//termine de enviar
						i = last_window;
						scroll2_state++;
					}
				}
				scroll2_current_window++;
				scroll2_timer = 550;
			}
			break;

		case SCROLL_FINISH:
			resp = RESP_FINISH;
			scroll2_state = SCROLL_INIT;
			break;

		default:
			resp = RESP_FINISH;
			scroll2_state = SCROLL_INIT;
			break;
	}

	return resp;
}

