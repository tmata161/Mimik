
#include"UI.h"

//this structure object is made for UI controls
typedef struct f{
    FILINFO Info[5]; //used for UI only holds only 5 files objects at a time
    int userPointer; //points to currently selected file
    int fileTrackingPointer; //points file index residing in sdcard
    int fileCount; //keeps track of total number of files present
}fileManagerStruct;

// this structure is made for internal API controls
typedef struct _SDFile{
FILINFO Info[50]; //list of fileinfo structure
int fCount; //keeps track of total number of files in a directory
}SDFile;



