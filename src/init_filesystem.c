// file to init the filesystem and show files
//#include"init_filesystem.h"
#include"../bootducky.h"
#include"UI/Screens/homePage.h"

#define FS_SIZE (256 * 1024)


static int pico_hal_read(lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size);
static int pico_hal_prog(lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size);
static int pico_hal_erase(lfs_block_t block);
static int pico_lock(void);
static int pico_unlock(void);

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
    .block_cycles = 500};

// Pico specific hardware abstraction functions

// file system offset in flash
const char* FS_BASE = (char*)(PICO_FLASH_SIZE_BYTES - FS_SIZE);

static int pico_hal_read(lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size) {
    assert(block < pico_cfg.block_count);
    assert(off + size <= pico_cfg.block_size);
    // read flash via XIP mapped space
    int* p = (int *)(FS_BASE + XIP_NOCACHE_NOALLOC_BASE + (block * pico_cfg.block_size) + off);
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
//function to mount filesystem
//in later builds this will be handled by tinyusb library
void create_files();
void get_files();
void lfs_init(interface_handler *inf){
    int err;
    err = lfs_mount(&pico_cfg);
    if(err != LFS_ERR_OK){
        printf("Unable to mount attempting to format\n");
        err = lfs_format(&pico_cfg);
        if(err != LFS_ERR_OK){
            printf("Format Failed turning down system, code: %d\n", err);
            return;
        }
        err = lfs_mount(&pico_cfg);
        if(err != LFS_ERR_OK){
            printf("Mount Failed turning down system, code: %d\n", err);
            return;
        }
    }
    printf("Mount Successful\n");
    
// now just count and store number of files
lfs_dir_t *d = malloc(sizeof(lfs_dir_t));
struct lfs_info dif;
inf->no_items=0;
if(d==NULL) printf("Unable to allocate mem\n");
err = lfs_dir_open(d, "/");
if(err!=LFS_ERR_OK) printf("unable to open directroy\n");
lfs_dir_rewind(d);
int rd=1;
while(true){
    rd=lfs_dir_read(d, &dif);
    if(rd==0) break; //EOF
    if(dif.type==LFS_TYPE_REG){
        inf->no_items+=1;
    }
}
lfs_dir_close(d);
free(d);
d=NULL;
}

bool ends_with(char *word, char *e){  //safety mechanism
    int t=0;
    while(word[t++] != '\0');
    t-=1;
    if(word[t-1]=='d' && word[t-2]=='d' && word[t-3]=='.')
        return true;
    else return false;
}

//only 4 files at a time
static int scroll_value_change;
void get_files(interface_handler *inf){
    int counter=0;
    struct lfs_info info;
    lfs_dir_t *dir = malloc(sizeof(lfs_dir_t));
    if(dir==NULL)
        printf("no mem\n");
    int err = lfs_dir_open(dir, "/");
    if(err!=LFS_ERR_OK) printf("unable to open dirertory\n");

    if(inf->scroll_val==1){
        lfs_dir_rewind(dir);
        inf->file_position = dir->pos;
        }
    
    //if scroll value gets decreased
    else if(inf->scroll_val<scroll_value_change){ //user has scrolled back
        if(inf->file_count!=UI_BUFFER_SIZE)
            inf->file_position -= inf->file_count+UI_BUFFER_SIZE;
        else inf->file_position -= (UI_BUFFER_SIZE)*2;
        lfs_dir_seek(dir, inf->file_position);
    }

    //if scroll value gets increased
    else lfs_dir_seek(dir, inf->file_position);  //user has scrolled forward

    int read_val=1;
    for(int i=0; i<4;i++){
        memset(inf->file_name[i], ' ', LFS_NAME_MAX+1);
    }
    while(counter<UI_BUFFER_SIZE){
       read_val = lfs_dir_read(dir, &info);
       if(read_val==0)break; // we have reached end of file
       if(info.type==LFS_TYPE_REG){
        memcpy(inf->file_name[counter], info.name, sizeof(info.name));
        counter+=1;
        inf->file_count=counter; //used to iterate for number of files
       }
    }
    inf->file_position = dir->pos; //set the position of file position for forward jump
    lfs_dir_close(dir);
    free(dir);
    dir=NULL;
    scroll_value_change=inf->scroll_val;
    return;
}


char* lfs_read(char* name, int* s){
    lfs_file_t *file = malloc(sizeof(lfs_file_t));
    if(file==NULL){
        printf("no mem\n");
        return NULL;
    }
    int err;
    err = lfs_file_open(file, name, LFS_O_RDWR);
    if(err!=LFS_ERR_OK){printf("Unable to open or file (%s) not found\n", name);return NULL;}
    err = lfs_file_size(file);
    char *buffer=(char*)malloc(err);
    memset(buffer, '\0', err);
    *s=err;
    //printf("buffer location inside function-> %p and payload size-> %d\n", buffer, err);
    lfs_file_rewind(file);
    lfs_file_read(file, buffer, err);
    lfs_file_close(file);
    free(file);
    file=NULL;
    return buffer;
}

void create_files(){
   
    lfs_file_t *file = malloc(sizeof(lfs_file_t));
    if(file==NULL){
        printf("no mem\n");
        return;
    }
    lfs_file_open(file, "payload10.txt", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(file);
    lfs_file_write(file, "Hello 10", 9);
    int err = lfs_file_sync(file);
    if(err == LFS_ERR_OK)
        printf("File write sucessful\n");
    lfs_file_close(file);
    free(file);
    file=NULL;

    
    lfs_unmount();
}


void lfs_print(char* name){ //just to read
    lfs_file_t *file = malloc(sizeof(lfs_file_t));
    if(file==NULL){
        printf("no mem\n");
        return;
    }
    int err;
    err = lfs_file_open(file, name, LFS_O_RDWR);
    if(err != LFS_ERR_OK){printf("Unable to open or the file (%s) does not exist\n", name);return;}
    err = lfs_file_size(file);
    lfs_file_rewind(file);
    char tempbuffer[err];
    memset(tempbuffer, '\0', sizeof(tempbuffer)+1);
    lfs_file_rewind(file);
    int b_r = lfs_file_read(file, tempbuffer, err);
    printf("bytes read %d\n", b_r);
    printf("%s\n", tempbuffer);
    lfs_file_close(file);
    free(file);
    file=NULL;
    return;
}

void lfs_delete(char *name){
    int err;
    struct lfs_info info;
    lfs_dir_t *dir=malloc(sizeof(lfs_dir_t));
    //lfs_file_t *f = malloc(sizeof(lfs_file_t));
    if(name==NULL){
        printf("deleting everything\n");
        //delete everything on root
        if(dir==NULL){printf("Unable to allocate memory for this operation\n"); return;}
        err = lfs_dir_open(dir, "/");
        if(err!=LFS_ERR_OK){printf("Unable to open directory or it doesn't exist\n"); return;}
        lfs_dir_rewind(dir);
        while(true){
            err=lfs_dir_read(dir, &info);
            if(err==0) break;
            if(info.type==LFS_TYPE_REG){
                // err = lfs_file_open(f, info.name, LFS_O_CREAT | LFS_O_RDWR);
                // if(err!=NULL){printf("Unable to open file\n"); return;}
                err = lfs_remove(info.name);
                if(err==LFS_ERR_OK) printf("Sucessfully deleted %s\n", info.name);
                else{printf("Unable to delete %s\n", info.name); return;}
            }
        }
    }

    else{
        lfs_remove(name);
        if(err==LFS_ERR_OK) printf("Sucessfully deleted %s\n", info.name);
        else{printf("Unable to delete %s\n", info.name); return;}
    }

    free(dir);
    dir=NULL;
}


// config file generator (read from config structure and create a file)
int config_generator(char *newfile, interface_handler *inf){  
    int i;
    for(i=0; inf->config.theme[i] != '\0' ; i++) {
        newfile[i]=inf->config.theme[i];
    }
    newfile[i++]='\n';
    for(int j=0; inf->config.filename[j] != '\0'; j++){
        newfile[i++]=inf->config.filename[j];
    }
    return i; //size of new config file
}

//read config file and save into data structure (read from file and create config structure)
void read_config(char *filebuffer, interface_handler *inf){
    int i=0;
    for(int k=0; filebuffer[i]!='\n'; k++){
        inf->config.theme[k]=filebuffer[i++];
    }
    i+=1;
    for (int k=0; filebuffer[i]!='\0'; k++)
    {
        inf->config.filename[k]=filebuffer[i++];
    }
    
}

void lfs_create_files(char *filename, char* add_file, int file_size){
    int err=0;
    lfs_file_t *file = malloc(sizeof(lfs_file_t));
    if(file==NULL){
        printf("no mem\n");
        return;
    }
 
    err=lfs_file_open(file, filename,  LFS_O_CREAT | LFS_O_RDWR);
    if(err!=LFS_ERR_OK){printf("Unable to open file\n"); return;}
    lfs_file_rewind(file);
    lfs_file_write(file, add_file, file_size);
    err = lfs_file_sync(file);
    if(err == LFS_ERR_OK)
        printf("File write sucessful\n");
    lfs_file_close(file);
    free(file);
    file=NULL;    
}

//----------SOME NEW CODE DOWN HERE-----------------
void getFiles(homePage *homePageStr); //only for tst purpose
//Initializes internal storage
//will store config file and UI images
//valid only for testing since at production payloads will be stored externally
void lfsInit(homePage *homePageStr){
    int err;
    err = lfs_mount(&pico_cfg);
    if(err != LFS_ERR_OK){
        printf("Unable to mount attempting to format\n");
        err = lfs_format(&pico_cfg);
        if(err != LFS_ERR_OK){
            printf("Format Failed turning down system, code: %d\n", err);
            return;
        }
        err = lfs_mount(&pico_cfg);
        if(err != LFS_ERR_OK){
            printf("Mount Failed turning down system, code: %d\n", err);
            return;
        }
    }
    printf("Mount Successful\n");
    
// now just count number of files
lfs_dir_t *d = malloc(sizeof(lfs_dir_t));
struct lfs_info dif;
if(d==NULL) printf("Unable to allocate mem\n");
err = lfs_dir_open(d, "/");
if(err!=LFS_ERR_OK) printf("unable to open directroy\n");
lfs_dir_rewind(d);

homePageStr->payloadList.payloadCount=0;
while(lfs_dir_read(d, &dif) != 0){
    if(dif.type==LFS_TYPE_REG){
        homePageStr->payloadList.payloadCount+=1;
    }
}
lfs_dir_close(d);
free(d);
d=NULL;
getFiles(homePageStr);
}

void getFiles(homePage *homePageStr){
    int counter=0;

    struct lfs_info info;
    lfs_dir_t *dir = malloc(sizeof(lfs_dir_t));
    if(dir==NULL)
        printf("no mem\n");
    int err = lfs_dir_open(dir, "/");
    if(err!=LFS_ERR_OK) printf("unable to open dirertory\n");

    //setup memory to store all payload names
    homePageStr->payloadList.payloadName = malloc(homePageStr->payloadList.payloadCount);
    while(lfs_dir_read(dir, &info) != 0){
       if(info.type==LFS_TYPE_REG){
        homePageStr->payloadList.payloadName[counter] = malloc(LFS_NAME_MAX+1);
        memcpy(homePageStr->payloadList.payloadName[counter], info.name, sizeof(info.name));
        counter+=1;
       }
    }

    // if(inf->scroll_val==1){
    //     lfs_dir_rewind(dir);
    //     inf->file_position = dir->pos;
    //     }
    
    // //if scroll value gets decreased
    // else if(inf->scroll_val<scroll_value_change){ //user has scrolled back
    //     if(inf->file_count!=UI_BUFFER_SIZE)
    //         inf->file_position -= inf->file_count+UI_BUFFER_SIZE;
    //     else inf->file_position -= (UI_BUFFER_SIZE)*2;
    //     lfs_dir_seek(dir, inf->file_position);
    // }

    // //if scroll value gets increased
    // else lfs_dir_seek(dir, inf->file_position);  //user has scrolled forward

    // int read_val=1;
    // for(int i=0; i<4;i++){
    //     memset(inf->file_name[i], ' ', LFS_NAME_MAX+1);
    // }
    // while(counter<UI_BUFFER_SIZE){
    //    read_val = lfs_dir_read(dir, &info);
    //    if(read_val==0)break; // we have reached end of file
    //    if(info.type==LFS_TYPE_REG){
    //     memcpy(inf->file_name[counter], info.name, sizeof(info.name));
    //     counter+=1;
    //     inf->file_count=counter; //used to iterate for number of files
    //    }
    // }
    // inf->file_position = dir->pos; //set the position of file position for forward jump
    // lfs_dir_close(dir);
    // free(dir);
    // dir=NULL;
    // scroll_value_change=inf->scroll_val;
}