// all storage setting and related stuff will be kept here
// have to store config files into internal memory banks
#include"UI.h"

typedef struct settingPage{
    int userPointer;
}settingPage;

typedef struct save{
    int theme; //contains index from list of themes
    int dryRun; //contain boolean value for dry run
}savedSetting;

