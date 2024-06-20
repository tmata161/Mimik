// first file that will be executed
#include "masterHeader.h"
#include "pico/multicore.h"

#include"lib/USB/MSC/mimikMSC.h"

// entry point

void main(){
   stdio_init_all();
   printf("==========MIMIK BOOT SUCCESSFUL🦆🦆=============\n\n");

//    gpio_init(AUTO);
//    gpio_pull_up(AUTO);
//    gpio_set_dir(AUTO, GPIO_IN);
//    int io = gpio_get(AUTO);
   
   // IF button pressed detected -> goto dump mode 
   // ELSE -> goto device mode [Normal running mode]
   //if(!io) USBMODE();

   master();

   // FATFS fs;
   // initFilesystem(&fs);
   // char* cont;
   // cont = readFile(&fs, "demo1.mik", file_size(&fs, "demo1.mik"));
   // printf("your file content is %s\n", cont);
   // free(cont);
   // printf("code ends here\n");
   
   //initTUDmsc();

   //checkSDCard();

}