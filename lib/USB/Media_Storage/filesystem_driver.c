// contains code that will enable pico to talk to sdmmc though tinyusb interface

#include"media_storage.h"

//---------only for testing purpose
#include"pico/bootrom.h"
#define Y 21

const int gpios[] = {4,5,6,7};
//----------------------------filesystem helper functions resides here-------------------
FRESULT initFilesystem(FATFS* fatsys){
    uint8_t work[FF_MAX_SS];
    FRESULT res;
    
      if (f_mount(fatsys)!=FR_OK){
        printf("FAT:Unable to mount , attempting to format\n");

        if(f_mkfs(fatsys, FM_FAT, 0 , work, sizeof(work))!=FR_OK) {
            printf("FAT:Format Unsuccessful turning down system\n");
            exit(1);
            }
        else {
            printf("FAT:Format Successful , trying to mount system again");
            if(f_mount(fatsys)!=FR_OK){
                printf("FAT:Unable to mount again, turning down system\n"); return FR_DISK_ERR;
                }
            else{
                printf("FAT:Mount Successful\n");
            }    
            }
      }
      else {
        printf("FAT mount Successful\n");
        }

     return FR_OK;
}

//read contents of the file
char* readFile(FATFS* fatsys, char* fileName, int fileSize){
    FIL fp;
    FILINFO finfo;
    char* filCont=(char *)malloc(fileSize);
    memset(filCont, '\0', fileSize);
    f_open(fatsys, &fp, fileName, FA_READ);
    f_read(&fp, filCont, fileSize, 0);
    f_close(&fp);
    return filCont;
}

//write content to a file
FRESULT writeFile(FATFS *fs, char *filename, char *buffer, UINT bufsize){
    UINT bytes_written=0;
    FIL fp;
    if(f_open(fs, &fp, filename, FA_WRITE | FA_CREATE_NEW) != FR_OK) {printf("could not open file or it does not exist\n");return 1;}
    if(f_write(&fp, buffer, bufsize, &bytes_written) != FR_OK) {printf("could not write to a file\n"); return 1;}
    if(f_sync(&fp) != FR_OK) {printf("could not flush buffer\n"); return 1;}
    f_close(&fp);
    return FR_OK;
}

/* return file size 
   if any problem, return -1
*/
int file_size(FATFS *fs, char *filename){
    FILINFO fil_info;
    FRESULT result;
    result = f_stat(fs, filename, &fil_info);
    if(result == FR_OK) return fil_info.fsize;
    else {printf("could not get file size\n"); return -1;}
}

//returns number of files present in a directory (ending with extension .mik)
int countFiles(FATFS *fs, char *cdir){
    FF_DIR dir;
    FILINFO finfo;
    int retCount=0; //returns no. of obj
    if(f_opendir(fs, &dir, cdir) != FR_OK){
        printf("Could not open directory for counting\n");
        exit(1);
    }
    f_rewinddir(&dir);
    while(f_readdir(&dir, &finfo) == FR_OK && finfo.fname[0]!=0){
        //do not count . and ..
        if((strcmp(finfo.fname, ".")!=0 || strcmp(finfo.fname, "..")!=0) && (finfo.fname[0]!='.')){
            if(finfo.fattrib==AM_ARC || finfo.fattrib==AM_DIR) //only if is a file and extension matches OR is a directory
                retCount++;
        }
       
    }
    return retCount;
}

// copies 50 file entries from external storage into memory
void copyFiles(FATFS *fs, char *folder,FF_DIR* directoryPointer, FILINFO* fileInfo){
    // directory is already opened  
    int fileCounter=0; //copy only 50 file names into memory
    FILINFO temp;
    while(f_readdir(directoryPointer, &temp) == FR_OK && temp.fname[0]!=0 && fileCounter<50){
        if(temp.fattrib == AM_ARC || (temp.fattrib==AM_DIR && (temp.fname[0]!='.'))){
                fileInfo[fileCounter]=temp;
                fileCounter++;
            }
        }
}
//-----------------------filesystem helper functions end---------------------------------



//-----------------------TUD code--------------------------
// void interrupt_reset_request(){
//     if(!gpio_get(Y)){
//         reset_usb_boot(0,0);
        
//     }
// }
//--------------------------------


// void initTUDmsc(){
//     //noise ..
//     for(int i=0;i<4;i++){
//         gpio_init(gpios[i]);
//         gpio_set_dir(gpios[i], GPIO_OUT);
//         gpio_put(gpios[i], 0);
//     }
//     for(int i=0;i<4;i++){
//         gpio_put(gpios[i], 1);
//     }
//     board_delay(5000);
//     storage_driver_init();
//     board_init();
//     tud_init(BOARD_TUD_RHPORT);
    
//     //only for testing
//     gpio_init(Y);
//     gpio_pull_up(Y);
//     gpio_set_dir(Y, GPIO_IN);
//     //------------------
//     while (1)
//     {
//             interrupt_reset_request();
//             tud_task();
//             led_blinking_task();
//         //tud_cdc_available();
//     }
    
// }
//----------------------TUD code end------------------------
