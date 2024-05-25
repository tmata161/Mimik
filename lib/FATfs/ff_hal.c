#include"ff_hal.h"

#define true 1
// a hal for fat file system

/*write buffer content into a file*/
FRESULT fat_write(FATFS *fs, char *filename, char *buffer, UINT bufsize){
    UINT bytes_written=0;
    FIL fp;
    if(f_open(fs, &fp, filename, FA_WRITE | FA_CREATE_NEW) != FR_OK) {printf("could not open file or it does not exist\n");return 1;}
    if(f_write(&fp, buffer, bufsize, &bytes_written) != FR_OK) {printf("could not write to a file\n"); return 1;}
    if(f_sync(&fp) != FR_OK) {printf("could not flush buffer\n"); return 1;}
    f_close(&fp);
    return FR_OK;
}

/*copy file content into a buffer*/
FRESULT fat_read(FATFS *fs, char *filename, char *buffer, UINT bufsize){
    UINT bytes_written=0;
    FIL fp;
    if(f_open(fs, &fp, filename, FA_READ) != FR_OK) {printf("could not open file or it does not exist\n");return 1;}
    if(f_read(&fp, buffer, bufsize, &bytes_written) != FR_OK){printf("could not read file\n");return 1;}
    f_close(&fp);
    return FR_OK;
}

/* return file size 
   if any problem, return -1
*/
int fat_file_size(FATFS *fs, char *filename, char *buffer, UINT bufsize){
    FILINFO fil_info;
    FRESULT result;
    result = f_stat(fs, filename, &fil_info);
    if(result == FR_OK) return fil_info.fsize;
    else {printf("file does not exist\n"); return -1;}
}

/* list and stores info about files (only for rootdir)*/
FRESULT fat_list_files(FATFS *fs, FAT_LFI *l){
    int index=0;
    int filecount=0;
    FRESULT res;
    FF_DIR dir;
    FILINFO fno;

    res = f_opendir(fs, &dir, "");    /* Open the directory */
        if (res != FR_OK) {printf("cannot open directory\n"); return FR_NO_FILESYSTEM;}

    if(l->fat_fname != NULL){
        for(int i=0; i<l->fat_nof; i++){
            free(l->fat_fname[i]);
            l->fat_fname[i] = NULL;
        }

        free(l->fat_fname);
        l->fat_fname = NULL;

        free(l->fat_fsize);
        l->fat_fsize = NULL;
    }

    while(true){
        res = f_readdir(&dir, &fno);
        if(res != FR_OK || fno.fname[0] == 0) break;
        if(fno.fattrib & AM_ARC){
            filecount+=1;
        }
    }

    if(filecount == 0) {f_closedir(&dir); return FR_OK;}
    //now allocate memory
    l->fat_nof = filecount;
    l->fat_fname = malloc(filecount*sizeof(char*));
    l->fat_fsize = (int*) malloc(l->fat_nof * sizeof(int));
    index=0;
    f_rewinddir(&dir);

    while(true){
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;
        if(fno.fattrib & AM_ARC){
        int namesize = strlen(fno.fname);
        l->fat_fname[index] = (char*)malloc(namesize);
        
        memcpy(l->fat_fname[index], fno.fname, namesize);
        l->fat_fsize[index] = fno.fsize;
        index+=1;
        }
    }

    f_closedir(&dir);
    return FR_OK;
}