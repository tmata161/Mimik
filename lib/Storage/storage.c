// this file contains code that will initialize the usb storage mode
#include"storage.h"
#include"tusb.h"


FATFS fatsys;

// initialized the storage api
void storage_init(){
    tud_init(BOARD_TUD_RHPORT);
}

//when called start all functions of fatsfs
void storage_loop(){
    tud_task();
    tud_cdc_available();
}

//*************************FAT INTERNAL API RESIDES HERE***********************

FRESULT initFilesystem(FATFS *fs){
    
    uint8_t work[FF_MAX_SS];
    FRESULT res;

    //res=f_mkfs(fs, FM_FAT, 0 , work, sizeof(work));
      if (f_mount(fs)!=FR_OK){
        printf("FAT:Unable to mount , attempting to format\n");

        if(f_mkfs(fs, FM_FAT, 0 , work, sizeof(work))!=FR_OK) {
            printf("FAT:Format Unsuccessful turning down system\n");
            return FR_DISK_ERR;
            }
        else {
            printf("FAT:Format Successful , trying to mount system again");
            if(f_mount(fs)!=FR_OK){
                printf("FAT:Unable to mount again, turning down system\n"); return FR_DISK_ERR;
                }
            else{
                printf("FAT:Mount Successful\n");
            }    
            }
      }
      else {
        printf("FAT:Mount Successful\n");
        }

     f_setlabel(fs, "QUILLO-USB");
     return FR_OK;
}

void tud_mount_cb(void){
  printf("mount callback\n");
     //init filesystem
    if(initFilesystem(&fatsys) == FR_OK)
        printf("File system initialization completed\n");

    else{
        printf("Filesystem initialization error, Turning down system\n");
        return;
    }
}

void tud_unmount_cb(void){
    //turnOFFfilesys(&fatsys);
}
