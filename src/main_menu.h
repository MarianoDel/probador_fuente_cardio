/*
 * main_menu.h
 *
 *  Created on: 11/02/2016
 *      Author: Mariano
 */

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

//estados del MAIN MENU
enum var_mainmenu_states
{
	MAINMENU_INIT = 0,
	MAINMENU_SHOW_MANUAL,
	MAINMENU_SHOW_DMX,
	MAINMENU_SHOW_COLORS,
	MAINMENU_SHOW_BRD_DIAG,

	MAINMENU_SHOW_MANUAL_SELECTED,
	MAINMENU_SHOW_MANUAL_SELECTED_1,
	MAINMENU_SHOW_DMX_SELECTED,
	MAINMENU_SHOW_DMX_SELECTED_1,
	MAINMENU_SHOW_COLORS_SELECTED,
	MAINMENU_SHOW_COLORS_SELECTED_1,
	MAINMENU_SHOW_BRD_DIAG_SELECTED,
	MAINMENU_SHOW_BRD_DIAG_SELECTED_1

};

//respuestas de las funciones
#define RESP_CONTINUE		0
#define RESP_SELECTED		1
#define RESP_CHANGE_DWN		2
#define RESP_CHANGE_UP		3
#define RESP_CHANGE_ALL_UP	4
#define RESP_WORKING		5

#define RESP_FINISH			10
#define RESP_YES			11
#define RESP_NO				12
#define RESP_NO_CHANGE		13
#define RESP_UPDATED		14


//estados de la funcion SHOW SELECT
#define SHOW_SELECT_INIT				0
#define SHOW_SELECT_1					1
#define SHOW_SELECT_2					2
#define SHOW_SELECT_3					3
#define SHOW_SELECT_SELECTED			4
#define SHOW_SELECT_SELECTED_1			5
#define SHOW_SELECT_CHANGE_DWN			6
#define SHOW_SELECT_CHANGE_DWN_1		7
#define SHOW_SELECT_CHANGE_UP			8
#define SHOW_SELECT_CHANGE_UP_1			9

//estados de la funcion OPTIONS
#define OPTIONS_INIT					0
#define OPTIONS_WAIT_SELECT			1
#define OPTIONS_WAIT_SELECT_1			2
#define OPTIONS_WAIT_SELECT_2			3
#define OPTIONS_WAIT_SELECT_3			4

#define OPTIONS_CHANGE_SELECT_UP	10
#define OPTIONS_CHANGE_SELECT_DWN	11

//estados de la funcion SCROLL
#define SCROLL_INIT				0
#define SCROLL_SENDING			1
#define SCROLL_FINISH			2

//estados de la funcion CHANGE
#define CHANGE_INIT					0
#define CHANGE_WAIT_SELECT			1

//estados de la funcion BLINKING
#define BLINKING_INIT       0
#define BLINKING_MARK_D     1
#define BLINKING_SPACE_D    2
#define BLINKING_MARK_C     3
#define BLINKING_SPACE_C    4
#define BLINKING_MARK_N     5
#define BLINKING_SPACE_N    6

//modos de blinking
#define BLINK_DIRECT    0
#define BLINK_CROSS     1
#define BLINK_NO        2

//modos de change
#define CHANGE_PERCENT    0
#define CHANGE_SECS		  1
#define CHANGE_CHANNELS	  2
#define CHANGE_ADDRESS	  3
#define CHANGE_COLORS	  4
#define CHANGE_RESET	  0x80

//wrapers de la funcion FuncChange
#define FuncChangePercent(X)		FuncChange(X, CHANGE_PERCENT, 0, 100)
#define FuncChangeSecs(X)			FuncChange(X, CHANGE_SECS, 0, 10)
#define FuncChangeChannels(X)		FuncChangeThreeButtonsCHAR(X, CHANGE_CHANNELS, 1, 2)
#define FuncChangeColors(X)			FuncChangeThreeButtonsCHAR(X, CHANGE_COLORS, 0, 2)
#define FuncChangeAddress(X)		FuncChangeThreeButtons(X, CHANGE_ADDRESS, 1, 512)
#define FuncChangePercentReset()	FuncChangeReset()
#define FuncChangeSecsReset()		FuncChangeReset()
#define FuncChangeChannelsReset()	FuncChangeReset()
#define FuncChangeAddressReset()	FuncChangeReset()
#define FuncChangeColorsReset()		FuncChangeReset()

//-------- Functions -------------
unsigned char FuncMainMenu (void);
unsigned char FuncShowSelect (const char *);
unsigned char FuncShowSelectv2 (const char *);
unsigned char FuncShowSelectv3 (const char *);
unsigned char FuncOptions (const char *, const char *, unsigned char *, unsigned char, unsigned char);
unsigned char FuncScroll1 (const char *);
unsigned char FuncScroll2 (const char *);
unsigned char FuncShowBlink (const char * , const char * , unsigned char, unsigned char);
//unsigned char FuncChange (unsigned char *);
unsigned char FuncChange (unsigned short *, unsigned char , unsigned short , unsigned short );
unsigned char FuncChangeThreeButtons (unsigned short * , unsigned char , unsigned short , unsigned short );
unsigned char FuncChangeThreeButtonsCHAR (unsigned char * , unsigned char , unsigned char , unsigned char );

void FuncOptionsReset (void);
void FuncShowSelectv2Reset (void);
void FuncChangeReset (void);

#endif /* MAIN_MENU_H_ */
