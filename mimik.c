
// entry point

#include "mimik.h"
#include "UI.h"


FATFS fat;
void main(){
   stdio_init_all();
   printf("==========MIMIK BOOT SUCCESSFUL🦆🦆=============\n\n");
   initScreen();

   //init memory card
   memory_card_init();

   // initialize file system objects
    initFilesystem(&fat);
    fileExplorerObject obj;
    obj.fSP=0;
    obj.fP[obj.fSP]=0;
    for(int i=0; i<sizeof(obj.navigationLocation)/sizeof(char); obj.navigationLocation[i++]='\0'); //initialize the navigationLocation with null characters
    obj.navigationLocation[0]='/';
    while(true){
        obj=file_explorer(obj);
    }
}