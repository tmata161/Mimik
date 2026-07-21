/*this file contains all include files necessary for basic needs*/

#ifndef MASTER
#define MASTER

#include "src/LCD_Test.h"
#include "lib/LCD/LCD_1in3.h"
#include"pico/stdio.h"
#include<stdio.h>
#include"pico/stdlib.h"
#include <string.h>

#include"lib/USB/HID-MSC/media_storage.h"


#define SCREEN_HEIGHT (LCD_1IN3_HEIGHT)
#define SCREEN_WIDTH (LCD_1IN3_WIDTH)
#define IMAGE_SIZE SCREEN_HEIGHT*SCREEN_WIDTH

//Button definition
#define OK 15 
#define USB_MODE 17 //usb device mode
#define SET 19 //settings
#define ESC 21 //back

#define JOYSTICK_UP 2
#define JOYSTICK_DOWN 18
#define SUBMIT LCD_1IN3_Display


//..............................
// Custom Macro definition
    #define DEBUG printf
//..............................

//----------Variable Declaration-----------
extern UWORD* ScreenBuffer ;
extern FATFS fat;
//---------------------------------------

//----------Screen function Declaration---------
int initScreen();
UWORD* allocateScreenBuffer();
void de_allocateScreenBuffer();
void drawBootLogo();
//-----------------------------------
#endif