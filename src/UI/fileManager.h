
#include"UI.h"

typedef struct f{
    char fileNameList[FILECAP][255]; //only 10 names at a time will be imported
    int userPointer; //points to currently selected file
    int fileTrackingPointer; //points file index residing in sdcard
    int fileCount; //keeps track of total number of files present
}fileManagerStruct;



