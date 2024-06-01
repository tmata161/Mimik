//for file list from storage
//keep it in a structure
//show it on homePage
#ifndef HOMEPAGE
#define HOMEPAGE

//structure will contain the payload list that will get
//fetched from secondary storage
//(only 7 payloads at a time)
struct payloadFileList{
    char** payloadName; //will set to dynamic at default (file list size is not known)
    int payloadCount; //keeps track of no. of payload files
};

typedef struct home
{
    int userPointer; //points to current location of the user
    struct payloadFileList payloadList;
}homePage;

extern homePage homePageStr;
void initHomePage();
void sync(UWORD *ScreenBuffer);
void setupButtons();
#endif