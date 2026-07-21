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
//return code {0:read uncessful; 1:read successful}
int readFile(FATFS* fs, char* fileName, int fileSize, char* databuffer){
    FIL file;
    FILINFO f_info;
    UINT ptr=0; //keeps track of bytes left to read
    f_open(fs, &file, fileName, FA_READ);
    if(&file==NULL){
        printf("could not open file\n");
        return 0;
    }

    while(1)
    {
        f_read(&file, (char*) databuffer, fileSize, &ptr);
        if(ptr==0) break;
    }
    f_rewind(&file);
    f_close(&file);
    return 1;
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

//open file and checks for the size of file
//return code: -1{error in file opening}
int get_file_size(FATFS *fs, char *filepath){
    FILINFO file_info;
    FIL file;

    if(f_open(fs, &file, filepath, FA_READ)!=FR_OK){
        printf("could not open file (get_file_size)\n");
        return -1;
    }

    if(f_stat(fs, filepath, &file_info) != FR_OK){
        printf("could not get file size (get_file_size)\n");
        file_info.fsize=-1;
    }
    //should close file before function end
    f_rewind(&file);
    f_close(&file);
    return file_info.fsize;
}

//returns number of files present in a directory
int countFiles(FATFS *fs, char *navLocation, FF_DIR *dir){
    //FF_DIR dir;
    FILINFO finfo;
    int retCount=0; //returns no. of obj
    //directory is already opened
    while(f_readdir(dir, &finfo) == FR_OK && finfo.fname[0]!='\0'){
        //do not count . and .. or any file starting with .
        if(finfo.fname[0]!='.'){
            //count only file and directory
            if(finfo.fattrib==AM_ARC || finfo.fattrib==AM_DIR){
                retCount++;
            }
        }
       
    }
    f_rewinddir(dir);
    return retCount;
}

// copies 50 files from a given folder into memory
void copyFiles(FATFS *fs, char *folder,FF_DIR* directoryPointer, FILINFO* fileInfo){
    // directory is already opened  
    f_rewinddir(directoryPointer);
    int fileCounter=0; //copy only 50 file names into memory
    FILINFO temp;
    while(f_readdir(directoryPointer, &temp) == FR_OK && temp.fname[0]!=0 && fileCounter<50){
        if(temp.fname[0]!='.')
            if(temp.fattrib == AM_ARC || temp.fattrib==AM_DIR){
                fileInfo[fileCounter]=temp;
                fileCounter++;
            }
        }
}

//checks the file ends with
int endsWith(char* filename, char* sequence){
    int len=strlen(filename);
    //returns true if certain sequence of characters are present in filename
    return (strstr(filename, sequence)!=NULL)?1:0;
}

//checks if the file is valid text file
int isValidFile(char* filename){
    //open the file and read 50 bytes from it (all the 50 bytes should be simple english character)
}
//-----------------------filesystem helper functions end---------------------------------



//-----------------------TUD code--------------------------
// void interrupt_reset_request(){
//     if(!gpio_get(Y)){
//         reset_usb_boot(0,0);
        
//     }
// }
//--------------------------------


void initTUDmsc(){
    
    board_delay(5000);
    //storage_driver_init();
    tud_init(BOARD_TUD_RHPORT);
    //------------------
    while (1)
    {
            //interrupt_reset_request();
            tud_task();
            led_blinking_task();
        //tud_cdc_available();
    }
    
}
//----------------------TUD code end------------------------
