#include"bsp/board.h"
#include"tusb.h"
#include"../FATfs/filesystem.h"


void storage_init();
void storage_loop();
void tud_unmount_cb(void);
