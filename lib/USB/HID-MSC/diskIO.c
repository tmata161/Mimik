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

#include "media_storage.h"
#include "hardware/flash.h"
#include "hardware/watchdog.h"



sdmmc_data_t pSDMMC;
int is_test_ready=0;
bool eject=false;
FATFS* this_fat;

unsigned int memory_card_init() {
  pSDMMC.spiInit=false;
#ifdef __SPI_SDMMC_DMA
  pSDMMC.dmaInit=false;
#endif
int ret = sdmmc_disk_initialize(SDMMC_SPI_PORT, SDMMC_PIN_CS, &pSDMMC);
return (ret==1)?1:0;
}

//detects for sd card
//return: 1 for detected; 0: for not detected
unsigned int detectCard(){
  return ((sdmmc_send_cmd(CMD0, 0, &pSDMMC)==1)? 1:0);
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
    const char deviceID[] = "MimikUSB";
    const char pid[] = "Mass Storage";
    const char rev[] = "1.0";
    
    memcpy(vendor_id, deviceID, strlen(deviceID));
    memcpy(product_id, pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}
bool test=false;
// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
  if(is_test_ready)return true;
  if(eject)test=true;
  return false; 
}
// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size)
{
        *block_count = pSDMMC.sectCount;//sectorCount;
        *block_size  = pSDMMC.sectSize;//sectorSize;
}

// Invoked when received Start Stop Unit command
// - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
// - Start = 1 : active mode, if load_eject = 1 : load disk storage
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
{
  (void) lun;
  (void) power_condition;

  if ( load_eject )
  {
    if (start)
    {
      return true;
    }
    else
    {
      DEBUG("eject command issued\n");
     // tud_deinit(0);
      while(f_umount(this_fat)!=FR_OK);
      //watchdog_reboot(0,0,0);
      is_test_ready=0;
      eject=true;
      return false;
    }
  }

  return false;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and return number of copied bytes.
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
  if(disk_read(&lun,buffer, lba+offset , bufsize)!=RES_OK)return -1;
  return (int32_t) bufsize;
}

bool tud_msc_is_writable_cb (uint8_t lun)
{
  if(pSDMMC.Stat)return true;
  return false;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and return number of written bytes
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize)
{

 if(disk_write(&lun, buffer, lba+offset , bufsize) != RES_OK) return -1;
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
  if ( board_millis() - start_ms < 100) return; // not enough time
  start_ms += 100;

  gpio_put(LED_BLINKING_PIN,led_state);
  led_state = 1-led_state; // toggle
}

void led_blinking_task_off(void) {
  gpio_put(LED_BLINKING_PIN,false);
}



DRESULT disk_read (void *drv, BYTE* buff, DWORD sector, UINT count)
{
  if (!sdmmc_read_sector(sector, buff, count, &pSDMMC)) return RES_ERROR;
  return RES_OK;
}

DRESULT disk_write (void *drv, const BYTE* buff, DWORD sector, UINT count)
{
  uint32_t ints = save_and_disable_interrupts();
  if (!sdmmc_write_sector(sector, (uint8_t *) buff, count, &pSDMMC)) return RES_ERROR;
  restore_interrupts(ints);
  return RES_OK;
}

DRESULT disk_ioctl (void *drv, BYTE cmd, void* buff)
{
switch(cmd){
  case CTRL_SYNC:
    return RES_OK;
    break;
  case GET_SECTOR_COUNT:
    *(DWORD*) buff = pSDMMC.sectCount;//sectorCount;
    return RES_OK;
    break;

  case GET_SECTOR_SIZE:
    *(DWORD*) buff = pSDMMC.sectSize;//sectorSize;
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
  return (!pSDMMC.Stat);
  break;
}
return RES_ERROR;
}


//lets check callback function
void tud_mount_cb(void){}
// Invoked when device is unmounted
void tud_umount_cb(void){
  
}

void mount_as_usb_flash(FATFS *fat){
  this_fat=fat;
  tusb_init(BOARD_TUD_RHPORT);
  is_test_ready=1;
  while(1){
    tud_task();
    if(eject && test) break;
  }
  watchdog_reboot(0,0,0);
}