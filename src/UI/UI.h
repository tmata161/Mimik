#ifndef UI
#define UI

#include "../../masterHeader.h"
#include"../../lib/USB/MSC/mimikMSC.h"

#define FORGROUND WHITE
#define BACKGROUND BLACK
#define FOREGROUND_SELECTED RED
#define THEME_SELECTEDBOX GREEN
#define THEME_BACK_LAYOUT CYAN

//------------GLOBAL VARIABLE  DECLARATION-------
extern char cDir[100];
//-----------END GLOBAL VARIABLE DECLARATION--------

//-----------MACRO DEFINITION----------
#define HALT(pragma) while(!DEV_Digital_Read(pragma))
//-----------------------------------

//------------Function Definitions------
void entrySetting();
void entryfileManager();
void entryInjection(char* selectedFile);
void back();
//Global UI functions
void drawMenuTray();
//-------------------------------------

#endif