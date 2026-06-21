#ifndef UI
#define UI

#include "../../mimik.h"
#include"../../lib/USB/Media_Storage/media_storage.h"

#define FORGROUND WHITE
#define BACKGROUND BLACK
#define FOREGROUND_SELECTED RED
#define THEME_SELECTEDBOX GREEN
#define THEME_BACK_LAYOUT CYAN

//------------GLOBAL VARIABLE  DECLARATION-------
//extern char cDir[50];
//-----------END GLOBAL VARIABLE DECLARATION--------
//-----------MACRO DEFINITION----------
#define HALT(pragma) while(!DEV_Digital_Read(pragma));
//-----------------------------------

//----------------------------------File explorer-------------------------
#define FILE_COPY_LIMIT 50 
#define MFO 5 //maximum file occupancy
//this object is for handling file explorer
// this structure is made for internal system
typedef struct _fileListDS{
FILINFO files[FILE_COPY_LIMIT]; //keeps list of fileinfo object
UINT LB, UB; //lower and upper bound
UINT slateNo; //store the current number of the slate
UINT fP[FILE_COPY_LIMIT]; //file pointer [works as an index to point to file object for every nested folders]
UINT TFC; //total number of files inside a directory
UINT fSP;// file stack pointer [works as an index to point to file object]
char navigationLocation[FILE_COPY_LIMIT*4];
}fileExplorerObject;
//-------------------------------------------------------------------

//----------------------------------Settings-------------------------
typedef struct settingPage{
    int userPointer;
}settingPage;

typedef struct save{
    int theme; //contains index from list of themes
    int dryRun; //contain boolean value for dry run
}savedSetting;
//-------------------------------------------------------------------

//-------------------------------Script Injection--------------------
typedef struct injection{
    int userPointer;
}injectionPage;
//-------------------------------------------------------------------

//------------Function decleration------
void entrySetting();
fileExplorerObject file_explorer(fileExplorerObject obj);
void entryInjection(char* selectedFile);
void back();
void notification(char* mess);
void drawMenuTray();
//-------------------------------------

#endif