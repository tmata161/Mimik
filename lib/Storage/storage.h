#ifndef EXT_STORAGE
#define EXT_STORAGE

#include"tusb.h"
#include"bsp/board.h"

#include"../FATfs/ff.h"
#include"../FATfs/diskio.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void storage_init();
void storage_loop();
void tud_unmount_cb(void);


//------------Function Declaration-------
void storage_driver_init();


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
//------------Function Declaration End-------

//-------------Global variable declaration-------
extern FATFS fatsys;
//-------------Global variable declaration End-------

#endif