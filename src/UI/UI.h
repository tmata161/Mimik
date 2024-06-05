#ifndef UI
#define UI

#include "../../masterHeader.h"

#define FORGROUND WHITE
#define BACKGROUND BLACK
#define FOREGROUND_SELECTED RED
#define THEME_SELECTEDBOX GREEN
#define THEME_BACK_LAYOUT CYAN

//-----------MACRO DEFINITION----------
#define HALT(pragma) while(!DEV_Digital_Read(pragma))
//-----------------------------------

//------------Function Definitions------
void entrySetting();
void entryfileManager();
void entryInjection(char* selectedFile);
//Global UI functions
void drawMenuTray();
//-------------------------------------

#endif