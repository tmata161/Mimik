/* this file was modified from tinyUSB example: msc_disk_dual.c*/

/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include "bsp/board.h"
#include "tusb.h"
#include "storage_driver.h"
#include "mimikMSC.h"

#include "hardware/flash.h"
#define PICO_SIZE 2*1024*1024
#define FAT 128*1024
const char* baseSize = (char*) (PICO_SIZE - FAT);
const int sectorSize=512;
const int sectorCount=FAT/sectorSize;
char* fatdata=(char*)(XIP_NOCACHE_NOALLOC_BASE+(PICO_FLASH_SIZE_BYTES-FAT));
sdmmc_data_t *pSDMMC=NULL;

void storage_driver_init() {
  // SDMMC driver initialize
  pSDMMC = (sdmmc_data_t*)malloc(sizeof(sdmmc_data_t)); 
  if(pSDMMC==NULL){printf("unable to allocate memory\n");return;}
  pSDMMC->spiInit=false;
#ifdef __SPI_SDMMC_DMA
  pSDMMC->dmaInit=false;
#endif
  sdmmc_disk_initialize(SDMMC_SPI_PORT, SDMMC_PIN_CS, pSDMMC);

  // LED blinking when reading/writing
  gpio_init(LED_BLINKING_PIN);
  gpio_set_dir(LED_BLINKING_PIN, true);
}



// Invoked to determine max LUN
uint8_t tud_msc_get_maxlun_cb(void)
{
  return 1; // LUN 0: SDMMC
}

// Invoked when received SCSI_CMD_INQUIRY
// Application fill vendor id, product id and revision with string up to 8, 16, 4 characters respectively
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
{
      sprintf(vendor_id  , "Mimik");
      sprintf(product_id , "Mimik Mass Storage");
      sprintf(product_rev, "1.0");
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
  return true; // RAM disk is always ready
}

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size)
{
        *block_count = pSDMMC->sectCount;//sectorCount;
        *block_size  = pSDMMC->sectSize;//sectorSize;
}

// Invoked when received Start Stop Unit command
// - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
// - Start = 1 : active mode, if load_eject = 1 : load disk storage
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
{
  (void) lun;
  (void) power_condition;

  // if ( load_eject )
  // {
  //   if (start)
  //   {
  //     // load disk storage
  //   }else
  //   {
  //     // unload disk storage
  //   }
  // }

  return true;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and return number of copied bytes.
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
  if(disk_read(&lun,buffer, lba, 1)!=RES_OK)return -1;
  return (int32_t) bufsize;
}

bool tud_msc_is_writable_cb (uint8_t lun)
{
  return true;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and return number of written bytes
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize)
{

 if(disk_write(&lun, buffer, lba, 1) != RES_OK) return -1;
 return (int32_t) bufsize;
}

// Callback invoked when received an SCSI command not in built-in list below
// - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, MODE_SENSE6, REQUEST_SENSE
// - READ10 and WRITE10 has their own callbacks
int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize)
{
  // read10 & write10 has their own callback and MUST not be handled here

  void const* response = NULL;
  int32_t resplen = 0;

  // most scsi handled is input
  bool in_xfer = true;

  switch (scsi_cmd[0])
  {
    default:
      // Set Sense = Invalid Command Operation
      tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

      // negative means error -> tinyusb could stall and/or response with failed status
      resplen = -1;
    break;
  }

  // return resplen must not larger than bufsize
  if ( resplen > bufsize ) resplen = bufsize;

  if ( response && (resplen > 0) )
  {
    if(in_xfer)
    {
      memcpy(buffer, response, (size_t) resplen);
    }else
    {
      // SCSI output
    }
  }

  return resplen;
}


void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < 50) return; // not enough time
  start_ms += 50;

  gpio_put(LED_BLINKING_PIN,led_state);
  led_state = 1 - led_state; // toggle
}

void led_blinking_task_off(void) {
  gpio_put(LED_BLINKING_PIN,false);
}



DRESULT disk_read (void *drv, BYTE* buff, DWORD sector, UINT count)
{
  //printf("disk read callback\n");
  if (!sdmmc_read_sector(sector, buff, SDMMC_SECT_SIZE, pSDMMC)) return RES_ERROR;
  //memcpy(buff, fatdata+(sector*sectorSize), sectorSize);
  return RES_OK;
}

DRESULT disk_write (void *drv, const BYTE* buff, DWORD sector, UINT count)
{
  //printf("disk write callback: sector-> %d\n", sector);
  uint32_t ints = save_and_disable_interrupts();
if (!sdmmc_write_sector(sector, (uint8_t *) buff, SDMMC_SECT_SIZE, pSDMMC)) return RES_ERROR;
//flash_range_erase((*fatdata+(sector*sectorSize)), sectorSize);
//flash_range_program((*fatdata+(sector*sectorSize)), buff, sectorSize);
//memcpy(fatdata+(sector*sectorSize), buff, sectorSize);
restore_interrupts(ints);
return RES_OK;
}

DRESULT disk_ioctl (void *drv, BYTE cmd, void* buff)
{
 // printf("disk io control callback\n");
switch(cmd){
  
  case CTRL_SYNC:
    return RES_OK;
    break;
  case GET_SECTOR_COUNT:
    *(DWORD*) buff = pSDMMC->sectCount;//sectorCount;
    return RES_OK;
    break;

  case GET_SECTOR_SIZE:
    *(DWORD*) buff = pSDMMC->sectSize;//sectorSize;
    return RES_OK;
    break;

  case GET_BLOCK_SIZE:
    *(DWORD*) buff = 1;
    return RES_OK;
    break;

  case IOCTL_INIT:
  *(DSTATUS*) buff = RES_OK;
  return RES_OK;
  break;

  case IOCTL_STATUS:
  *(DSTATUS*)buff=RES_OK;
  return RES_OK;
  break;
}
return RES_ERROR;
}