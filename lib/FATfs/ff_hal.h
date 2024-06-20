#ifndef FF_HAL
#define FF_HAL

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ff.h"

// FAT File buffer object
typedef struct list_file_info{
    char **fat_fname;
    int* fat_fsize;
    int fat_nof;
}FAT_LFI;
/*write buffer content into a file*/
FRESULT  fat_write(FATFS *fs, char *filename, char *buffer, UINT bufsize);

/*copy file content into a buffer*/
FRESULT fat_read(FATFS *fs, char *filename, char *buffer, UINT bufsize);

/* return file size 
   if any problem, return -1
*/
int fat_file_size(FATFS *fs, char *filename, char *buffer, UINT bufsize);

/* list and stores info about files (only for rootdir)*/
FRESULT fat_list_files(FATFS *fs, FAT_LFI *l);

#endif FF_HAL