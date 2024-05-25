#include "UI.h"

UWORD *ScreenBuffer;

//-----------------------------------------------------------
// this is the first function to be called
void mimikMode(){
    initHomePage();
    sync(ScreenBuffer);
    setupButtons(ScreenBuffer);

    //printf("Every thing has been setup successfully\nPrinting files present in internal storage");

    //for(int i=0; i<homePageStr.payloadList.payloadCount; i++){

   // }
}