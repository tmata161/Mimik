#ifndef UI
#define UI

#include "../../mimik.h"
#include"../../lib/USB/HID-MSC/media_storage.h"

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
#define FILE_COPY_LIMIT 50 //holds max 50 files in memory
#define MAX_FILE_LIMIT 5 //file rendering limit
#define FILE_SIZE_LIMIT 300

// this structure handles file navigation part
typedef struct _fileListDS{
FILINFO files[FILE_COPY_LIMIT]; //keeps list of files and folders in a given directory
UINT lower_bound, upper_bound; //lower and upper bound
UINT slate_no; //store the current number of the slate
UINT global_file_pointer[FILE_COPY_LIMIT]; //file pointer [works as an index that points to file for every nested folders]
UINT total_file_count; //keeps a record of how many files are present in a directory
UINT index_of_gfp;// a sort of index to global file pointer
bool return_back;
char navigationLocation[100];
}fileExplorerObject;


//file pasring
#define INSTRUCTION_LENGTH_LIMIT 50
typedef struct _parser{
    char buffer[INSTRUCTION_LENGTH_LIMIT]; //stores instruction just for each line
    unsigned int line_number;
}parser;
//-------------------------------File explorer End------------------------------------


//----------------------------------Settings-------------------------
// [not implemented yet]
// typedef struct settingPage{
//     int userPointer;
// }settingPage;

// typedef struct save{
//     int theme; //contains index from list of themes
// }savedSetting;
//void entrySetting();
//----------------------------------Settings---------------------------------

//-------------------------------Script Injection--------------------
void start_animation(char* message);
void stop_animation();
//-------------------------------------------------------------------

//------------Function decleration------

fileExplorerObject file_explorer(fileExplorerObject obj);
void entryInjection(char* selectedFile);
void back();
void notification(char* mess);
void drawMenuTray();
//-------------------------------------

#endif