/*this file contains all include files necessary for basic needs*/

#ifndef BOOT
#define BOOT

#include "src/LCD_Test.h"
#include "LCD_2in.h"
#include "LCD_2in.h"
#include"lib/lfs/lfs.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include"pico/stdio.h"
#include<stdio.h>
#include"pico/stdlib.h"
#include<stdlib.h>
#include <string.h>

#include"src/UI/Screens/homePage.h"


#define SCREEN_HEIGHT 250
#define SCREEN_WIDTH 250
#define IMAGE_SIZE SCREEN_HEIGHT*SCREEN_WIDTH*3
#define ESC 15  //aka A
#define VIEW 17 //aka B
#define MORE 19 //aka X
#define AUTO 21 //aka Y
#define AUTO_RUN 21
#define UI_BUFFER_SIZE 4

//HORIZONTAL MODE
#define JOYSTICK_UP 18
#define JOYSTICK_DOWN 2
#define JOYSTICK_RIGHT 16
#define JOYSTICK_LEFT  20
#define JOYSTICK_ENTER 3


enum option_type{
    UI_FILE=0,
    UI_OPTION=1,
    UI_DIR=2
};

//device has been booted into normal mode
void home_screen();

// a way to simplify code a little bit
enum _screen{
    HOME_SCREEN,
    CONFIG_SCREEN,
};


//device has been booted into usb mode
void USBMODE();

// track current position user's pointing to
typedef struct u{
    char p_filename[LFS_NAME_MAX+1];
    int pointingValue;
    //bool pointing_to_option;
    bool mutex_home; //ensures system/user is on home page
    enum _screen screen_val;
}user;

// stores text position
typedef struct T{
    int width_start;
    int height_start;
}tp;

// 
struct conf{
    int col_code;
    char theme[10];
    char filename[256]; //file name that has to run on startup
};


// keep track of ui interface
typedef struct p{
    char file_name[4][LFS_NAME_MAX+1]; //4 files max and name could be 256 characters
    lfs_off_t file_position; //current position of file pointer
    user user_pointer; // where the user is currently pointing to
    int file_count; //to count number of files in each page
    tp text_pos; // to setup position of text
    int scroll_val; //checks for the page change
    int no_items; //stores total number of files
    struct conf config; //stores the current options and other system states
}interface_handler;
extern interface_handler inf;


//---------FILE SYSTEM-------
void lfs_init(interface_handler *inf);
void get_files(interface_handler *inf);
void lfs_init();
void get_files(interface_handler *inf);
char* lfs_read(char* name, int* s);
void lfs_delete(char *name);
void lfs_print(char* name);
int config_generator(char *newfile, interface_handler *inf);
void read_config(char *filebuffer, interface_handler *inf);
void lfs_create_files(char *filename, char* add_file, int file_size);
//----------NEW DEFINITINS HERE------------------------------
void lfsInit(homePage *homePageStr);

//----------HID-----------------
void parsing_mechanism(char *script, int script_size);
void hid_init();
void hid_run();

//------USB MODE----------------
void USBMODE();


//--------UI API'S--------------
//mimikMode();
#endif