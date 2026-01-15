/*this file contains all include files necessary for basic needs*/

#ifndef MASTER
#define MASTER

#include "src/LCD_Test.h"
#include "lib/LCD/LCD_1in3.h"
#include"pico/stdio.h"
#include<stdio.h>
#include"pico/stdlib.h"
#include <string.h>

#include"lib/USB/Media_Storage/media_storage.h"


#define SCREEN_HEIGHT LCD_1IN3_HEIGHT
#define SCREEN_WIDTH LCD_1IN3_WIDTH
#define IMAGE_SIZE SCREEN_HEIGHT*SCREEN_WIDTH

//Button definition
#define OK 15  //aka A
#define NONE 17 //aka B [will set later]
#define SET 19 //aka X [setting page]
#define ESC 21 //aka Y 

#define JOYSTICK_UP 2
#define JOYSTICK_DOWN 18
#define SUBMIT LCD_1IN3_Display

#define FILECAP 5

//..............................
// Custom Macro definition
    #define ERROR printf
//..............................

//----------Variable Declaration-----------
extern UWORD* ScreenBuffer ;
extern FATFS fat;
//---------------------------------------

//-----------Structure Declaration--------


//----------Screen function Declaration---------
void initScreen();
UWORD* allocateScreenBuffer();
void de_allocateScreenBuffer();
void drawBootLogo();
//-----------------------------------
#endif