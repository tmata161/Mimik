// this file contains code that will initialize the usb storage mode
#include"storage.h"

void tud_msc_write10_complete_cb(uint8_t lun);
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);

void tud_mount_cb(void);
void tud_unmount_cb(void);
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]);
bool tud_msc_test_unit_ready_cb(uint8_t lun);
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size);
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject);
bool tud_msc_is_writable_cb (uint8_t lun);
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);
int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize);


FATFS fatsys;

// initialized the storage api
void storage_init(){
    tud_init(BOARD_TUD_RHPORT);
}

//when called start all functions of fatsfs
void storage_loop(){
    tud_task();
    tud_cdc_available();
}

//*************************FAT INTERNAL API RESIDES HERE***********************
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize){
  printf("read callback\n");
disk_read(&lun, buffer, lba+offset, 1);
return bufsize;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize){
   printf("write callback\n");
disk_write(&lun, buffer, lba+offset, 1);
    return bufsize;
}


void tud_mount_cb(void){
  printf("mount callback\n");
     //init filesystem
    if(initFilesystem(&fatsys) == FR_OK)
        printf("File system initialization completed\n");

    else{
        printf("Filesystem initialization error, Turning down system\n");
        return;
    }
}

void tud_unmount_cb(void){
    //turnOFFfilesys(&fatsys);
}

void tud_msc_write10_complete_cb(uint8_t lun){} //No need of this function now
void tud_msc_read10_complete_cb(uint8_t lun){}

int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize){
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

  return (int32_t) resplen;
    
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size){
    
    *block_size = FAT_BLOCK_SIZE;
    *block_count= (((FS_SIZE/1024) * 1000) / FAT_BLOCK_SIZE);
}

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]){

    const char vid[] = "Quill Script";
    const char pid[] = "Mass Storage";
    const char rev[] = "1.0";

    memcpy(vendor_id  , vid, strlen(vid));
    memcpy(product_id , pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}

bool tud_msc_test_unit_ready_cb(uint8_t lun){
return true;    
}

bool tud_msc_is_writable_cb (uint8_t lun){
return true;
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject){
   return true;
}