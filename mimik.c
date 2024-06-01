// first file that will be executed
#include "bootducky.h"
#include "pico/multicore.h"

// entry point

void main(){
   stdio_init_all();
   printf("==========DUCK BOOT SUCCESSFUL🦆🦆=============\n");

   gpio_init(AUTO);
   gpio_pull_up(AUTO);
   gpio_set_dir(AUTO, GPIO_IN);
   int io = gpio_get(AUTO);
   
   // IF button pressed detected -> goto dump mode 
   // ELSE -> goto device mode [Normal running mode]
   //if(!io) USBMODE();
    home_screen();
   
}