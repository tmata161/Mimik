
// entry point

#include "mimik.h"
#include "UI.h"


FATFS fat;
void main(){
   stdio_init_all();
   printf("==========MIMIK BOOT SUCCESSFUL🦆🦆=============\n\n");
   if(!initScreen())return;
   //drawBootLogo(); 
   //init memory card
   if(!memory_card_init()){notification("Failed to setup  Memory Card");return;}
    board_init();
   // initialize file system objects
    initFilesystem(&fat);
    fileExplorerObject obj;
    obj.index_of_gfp=0;
    obj.global_file_pointer[obj.index_of_gfp]=0;
    obj.return_back=1;//memory efficient code
    for(int i=0; i<sizeof(obj.navigationLocation)/sizeof(char); obj.navigationLocation[i++]='\0'); //initialize the navigationLocation buffer with null characters
    obj.navigationLocation[0]='/';
    while(obj.return_back){
        obj=file_explorer(obj);
    }
    mount_as_usb_flash(&fat);
}