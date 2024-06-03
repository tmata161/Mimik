/*this file contains all include files necessary for basic needs*/

#ifndef MASTER
#define MASTER

#include "src/LCD_Test.h"
#include "lib/LCD/LCD_1in3.h"
//#include"lib/lfs/lfs.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include"pico/stdio.h"
#include<stdio.h>
#include"pico/stdlib.h"
#include <string.h>


#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 240
#define IMAGE_SIZE SCREEN_HEIGHT*SCREEN_WIDTH

//Button definition
#define OK 15  //aka A
#define NONE 17 //aka B [will set later]
#define SET 19 //aka X [setting page]
#define ESC 21 //aka Y 

#define JOYSTICK_UP 18
#define JOYSTICK_DOWN 2
#define SUBMIT LCD_1IN3_Display

//..............................
// Custom Macro definition
    #define ERROR printf
//..............................

//----------Variable Declaration-----------
extern UWORD* ScreenBuffer;
//---------------------------------------


//----------Function Definition---------
void master();
//-----------------------------------

//-----------Structure Declaration--------
typedef struct tracker{
    char files[10][255];
    int fileNo;
}fileTrack;

#endif