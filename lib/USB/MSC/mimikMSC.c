// contains code that will enable pico to talk to sdmmc though tinyusb interface

#include"mimikMSC.h"

//---------only for testing purpose
#include"pico/bootrom.h"
#define Y 21
const int gpios[] = {4,5,6,7};
//----------------------------APT code resides here-------------------
FRESULT initFilesystem(FATFS* fatsys){
    uint8_t work[FF_MAX_SS];
    FRESULT res;
// //noise ..
for(int k=1; k<=3; k++){
    for(int i=0;i<4;i++){
    gpio_init(gpios[i]);
    gpio_set_dir(gpios[i], GPIO_OUT);
    gpio_put(gpios[i], 0);
}for(int i=0;i<4;i++){
    gpio_put(gpios[i], 1);
}
}
    storage_driver_init();

    //res=f_mkfs(fs, FM_FAT, 0 , work, sizeof(work));
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
        printf("FAT:Mount Successful\n");
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

//return 1 if extension matches .mik
int doesExtensionMatch(char* ex){
//chk if last characters from last 3 index matches
int lenEx=strlen(ex)-1;
if(lenEx>3){ //name of file should be greater than 3
    if(ex[lenEx--]=='k' && ex[lenEx--]=='i' && ex[lenEx--]=='m' && ex[lenEx]=='.') //chk last 3 characters match extension 
        return 1;

    else return 0;
} return 0;
}

//returns number of files with extension .mik and other directories in a directory
int countFiles(FATFS *fs, char *cdir){
    FF_DIR dir;
    FILINFO finfo;
    int retCount=0; //returns no. of obj

    if(f_opendir(fs, &dir, cdir) != FR_OK){
        printf("Could not open directory while counting\n");
        exit(1);
    }

    while(f_readdir(&dir, &finfo) == FR_OK && finfo.fname[0]!=0){
        //do not count . and ..
        if(strcmp(finfo.fname, ".")!=0 || strcmp(finfo.fname, "..")!=0){
            if(doesExtensionMatch(finfo.fname) || finfo.fattrib == AM_DIR) //only if extension matches
                retCount++;
        }
    }
    return retCount;
}

// return 50 entries from a directory with .mik extension
//this function is designed in such a way that it can read files anywhere from between
void listFile(FATFS *fs, char *folder,FF_DIR* directoryPointer, FILINFO* fileInfo){
    // directory is already opened  
    int c=0; //for only 50 files are copied
    FILINFO tempInfo;
    while(f_readdir(directoryPointer, &tempInfo) == FR_OK && tempInfo.fname[0]!=0 && c<50){
        if((tempInfo.fattrib) != AM_DIR){ //current entity is a directory
            if(doesExtensionMatch(tempInfo.fname)) {
                //memcpy(fileInfo[c].fname, tempInfo.fname, strlen(tempInfo.fname));
                fileInfo[c]=tempInfo;
                c++;
            }
        }
        else{ //current entity is a file
            //memcpy(fileInfo[c].fname, tempInfo.fname, strlen(tempInfo.fname));
            fileInfo[c]=tempInfo;
            c++;
        }
    }
}
//-----------------------API code end---------------------------------



//-----------------------TUD code--------------------------
void interrupt_reset_request(){
    if(!gpio_get(Y)){
        reset_usb_boot(0,0);
        
    }
}
//--------------------------------


void initTUDmsc(){

//noise ..
for(int i=0;i<4;i++){
    gpio_init(gpios[i]);
    gpio_set_dir(gpios[i], GPIO_OUT);
    gpio_put(gpios[i], 0);
}for(int i=0;i<4;i++){
    gpio_put(gpios[i], 1);
}
board_delay(5000);
    storage_driver_init();
    board_init();
    tud_init(BOARD_TUD_RHPORT);
    

//only for testing
gpio_init(Y);
gpio_pull_up(Y);
gpio_set_dir(Y, GPIO_IN);
//------------------
    while (1)
    {
        interrupt_reset_request();
        tud_task();
        led_blinking_task();
        //tud_cdc_available();
    }
    
}
//----------------------TUD code end------------------------





void checkSDCard(){
    sdmmc_data_t pSDMMC;
    pSDMMC.spiInit=false;
    sdmmc_disk_initialize(SDMMC_SPI_PORT, SDMMC_PIN_CS, &pSDMMC);

    //only for testing
gpio_init(Y);
gpio_pull_up(Y);
gpio_set_dir(Y, GPIO_IN);
gpio_init(25);
gpio_set_dir(25, true);
    while(1){
        interrupt_reset_request();
        led_blinking_task();
        printf("card presence %d\n", sdmmc_send_cmd(CMD8, 0, &pSDMMC));
        //if(pSDMMC.Stat==0){pSDMMC.spiInit=false;sdmmc_disk_initialize(SDMMC_SPI_PORT, SDMMC_PIN_CS, &pSDMMC);}
    }
}