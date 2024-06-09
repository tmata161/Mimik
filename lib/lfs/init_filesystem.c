// file to init the filesystem and show files
//#include"init_filesystem.h"
#include"init_filesystem.h"



/// @brief File System internal code-------------------
static int pico_hal_read(lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size);
static int pico_hal_prog(lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size);
static int pico_hal_erase(lfs_block_t block);

// configuration of the filesystem is provided by this struct
// for Pico: prog size = 256, block size = 4096, so cache is 8K
// minimum cache = block size, must be multiple
struct lfs_config pico_cfg = {
    // block device operations
    .read = pico_hal_read,
    .prog = pico_hal_prog,
    .erase = pico_hal_erase,

    // block device configuration
    .read_size = 1,
    .prog_size = FLASH_PAGE_SIZE,
    .block_size = FLASH_SECTOR_SIZE,
    .block_count = FS_SIZE / FLASH_SECTOR_SIZE,
    .cache_size = FLASH_SECTOR_SIZE / 4,
    .lookahead_size = 32,
    .block_cycles = 500
    };

// Pico specific hardware abstraction functions

// file system offset in flash
const char* FS_BASE = (char*)(PICO_FLASH_SIZE_BYTES - FS_SIZE);

static int pico_hal_read(lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size) {
    assert(block < pico_cfg.block_count);
    assert(off + size <= pico_cfg.block_size);
    memcpy(buffer, FS_BASE + XIP_NOCACHE_NOALLOC_BASE + (block * pico_cfg.block_size) + off, size);
    return LFS_ERR_OK;
}

static int pico_hal_prog(lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size) {
    assert(block < pico_cfg.block_count);
    // program with SDK
    uint32_t p = (uint32_t)FS_BASE + (block * pico_cfg.block_size) + off;
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(p, buffer, size);
    restore_interrupts(ints);
    return LFS_ERR_OK;
}

static int pico_hal_erase(lfs_block_t block) {
    assert(block < pico_cfg.block_count);
    // erase with SDK
    uint32_t p = (uint32_t)FS_BASE + block * pico_cfg.block_size;
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(p, pico_cfg.block_size);
    restore_interrupts(ints);
    return LFS_ERR_OK;
}
/// @brief File System internal code end-------------------


//---------------------User API Functions Definitions-----------------------

//returns the content of the file
char* lfsFileRead(lfs_file_t* file, int fileSize){
    char* fileContent = (char*) malloc(fileSize);
    lfs_file_read(file, fileContent, fileSize);
    lfs_file_rewind(file);
    return fileContent;
}

//returns the file size
int lfsFileSize(lfs_file_t* file){
    int size = lfs_file_size(file);
    lfs_file_rewind(file);
    return size;
}

// writes content to the file
void lfsFileWrite(lfs_file_t* file, char* fileContent){
    lfs_file_write(file, fileContent, strlen(fileContent));
    lfs_file_rewind(file);
}

void lfsUnMount(){
    lfs_unmount();
}

void lfsMount(){
    int err;
    err = lfs_mount(&pico_cfg);
    if(err == LFS_ERR_OK){}
    else {
        printf("Unable to mount attempting to format\n");
        err = lfs_format(&pico_cfg);
        if(err != LFS_ERR_OK){printf("Format Failed turning down system, code: %d\n", err);exit(1);}
        else{
            printf("Format Successful attempting to mount again\n");
            err = lfs_mount(&pico_cfg);
            if(err != LFS_ERR_OK){printf("Mount Failed turning down system, code: %d\n", err);exit(1);}
        }
    }
   printf("Mount Successful\n");
}
//---------------------User API Functions Definitions END-------------------------------
