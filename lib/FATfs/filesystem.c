#include"filesystem.h"
// contains code for main filesystem and api function to communicate with it


const char* FS_BASE=(char*) (PICO_FLASH_SIZE_BYTES - FS_SIZE);
//Filesystem size 512Kb
//block size 512
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

void turnOFFfilesys(FATFS *fs){
    f_umount(fs);
}

//-----------------------------------------FAT IO functions definitions--------------------------------
DRESULT disk_read (void *drv, BYTE* buff, DWORD sector, UINT count)
{
 memcpy(buff, (FS_BASE + XIP_NOCACHE_NOALLOC_BASE +(sector*FAT_BLOCK_SIZE)), FAT_BLOCK_SIZE);
return RES_OK;
}

DRESULT disk_write (void *drv, const BYTE* buff, DWORD sector, UINT count)
{
memcpy((FS_BASE + XIP_NOCACHE_NOALLOC_BASE+(sector*FAT_BLOCK_SIZE)), buff, FAT_BLOCK_SIZE);
return RES_OK;}

DRESULT disk_ioctl (void *drv, BYTE cmd, void* buff)
{
switch(cmd){
  
  case CTRL_SYNC:
    return RES_OK;
    break;
    
  case GET_SECTOR_COUNT:
    *(DWORD*)buff=FS_RAM_SIZE_CPY/FAT_BLOCK_SIZE;
    return RES_OK;
    break;

  case GET_SECTOR_SIZE:
    *(DWORD*)buff=FAT_BLOCK_SIZE;
    return RES_OK;
    break;

  case GET_BLOCK_SIZE:
    *(DWORD*)buff=1;
    return RES_OK;
    break;

  case IOCTL_INIT:
  *(DSTATUS*)buff=RES_OK;
  return RES_OK;
  break;

  case IOCTL_STATUS:
  *(DSTATUS*)buff=RES_OK;
  return RES_OK;
  break;
}
return RES_ERROR;;
}

void FAT_print_files(FATFS *fat){
  FF_DIR *dir;
  FILINFO info;
  int readVal=1;

  dir = malloc(sizeof(FF_DIR));
  if(dir==NULL){printf("Ramdisk error, unable to setup directory\n"); return;}
  if(f_opendir(fat, dir, "") != FR_OK){
      printf("Ramdisk error, Unable to open directory\n");
      return;
  }
f_rewinddir(dir);
printf("Printing files in ramdisk\n");
readVal = f_readdir(dir, &info);
  while(readVal==FR_OK && strlen(info.fname)>0){
    printf("%s\n", info.fname);
  readVal = f_readdir(dir, &info);
  }
}