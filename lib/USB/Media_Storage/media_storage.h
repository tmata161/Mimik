

#ifndef EXT_STORAGE
#define EXT_STORAGE


#include"../FATfs/ff.h"
#include"../FATfs/diskio.h"

#include "../sdmmc/spi_sdmmc.h"
#include "hardware/gpio.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"pico/stdio.h"
#include"pico/stdlib.h"
#include"bsp/board.h"
#include"tusb.h"
#include<stdio.h>
#include"pico/stdio.h"

#include"storage_driver.h"

//------------Function Declaration-------
uint8_t tud_msc_get_maxlun_cb(void);
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]);
bool tud_msc_test_unit_ready_cb(uint8_t lun);
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size);
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject);
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
bool tud_msc_is_writable_cb (uint8_t lun);
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);
int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize);
void led_blinking_task(void);
void led_blinking_task_off(void);

DRESULT disk_read (void *drv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (void *drv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (void *drv, BYTE cmd, void* buff);


FRESULT initFilesystem(FATFS* fatsys);
char* readFile(FATFS* fatsys, char* fileName, int fileSize);
int file_size(FATFS *fs, char *filename);
FRESULT writeFile(FATFS *fs, char *filename, char *buffer, UINT bufsize);
void initTUDmsc();
void copyFiles(FATFS *fs, char *folder,FF_DIR* directoryPointer, FILINFO* fileInfo);
int countFiles(FATFS *fs, char *cdir); //returns no. of entries present inside a folder

uint8_t sdmmc_disk_initialize(spi_inst_t *spi, uint cs_pin, sdmmc_data_t *sdmmc);
unsigned int memory_card_init(void);
unsigned int detectCard();

//new function
void mount_as_usb_flash();
//------------Function Declaration End-------
#endif

