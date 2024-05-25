#include"ff.h"
#include"diskio.h"
#include"stdio.h"
#include"stdlib.h"
#include"pico.h"
#include"pico/bootrom.h"
#include "hardware/regs/addressmap.h"

#define FAT_BLOCK_SIZE 512
#define FS_SIZE (FAT_BLOCK_SIZE * 1024)

FRESULT initFilesystem(FATFS *fs);
void turnOFFfilesys(FATFS *fs);