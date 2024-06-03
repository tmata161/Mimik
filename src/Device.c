// this is the master branch of the device

#include<stdio.h>
#include"screen_init.c"
#include"../masterHeader.h"
#include"UI/fileManager.h"

const char demoFiles[][15] = {"demo1.mik", "demo2.mik", "demo3.mik", "demo4.mik", "demo5.mik", "demo6.mik", "demo7.mik", "demo8.mik", "demo9.mik", "demo10.mik", "demo11.mik", "demo12.mik", "demo13.mik", "demo14.mik", "demo15.mik", "demo16mik", "demo17.mik", "demo18.mik", "demo19.mik", "demo20.mik",};
int fileNo=20;
UWORD* ScreenBuffer;
fileTrack ft;

void initScreen();
void drawBootLogo();

void master(){
    //setup screen
    initScreen();
    //draw boot logo
    drawBootLogo();
    //open filemanger screen
    entryfileManager();
}

void initScreen(){
    if(DEV_Module_Init()!=0){
        ERROR("Could not initialize library, pins or spi protocol\n");
        return;
    }

    // initializes pwm stuff
    DEV_SET_PWM(50);
    // initializes the lcd
    LCD_1IN3_Init(HORIZONTAL);
    // clear screen
    LCD_1IN3_Clear(WHITE);

    // setting backlight
    LCD_1IN3_SetBacklight(1023);

    if((ScreenBuffer = (UWORD *)malloc(IMAGE_SIZE)) == NULL) {
        ERROR("Failed to setup memory...\n");
        exit(0);
    }

    Paint_NewImage((UBYTE *)ScreenBuffer,SCREEN_WIDTH,SCREEN_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);

    //submit content of screenbuffer to show to the screen
   // LCD_2IN_Display((UBYTE*)ScreenBuffer);
}

void drawBootLogo(){
    Paint_Clear(BLACK);
    Paint_DrawString_EN(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "MIMIK", &Font20, WHITE, BLACK);
    SUBMIT(ScreenBuffer);
    DEV_Delay_ms(200);
    Paint_Clear(BLACK);
}

// read files from demo list and show on screen
// argument-> index of file
// return -> object of file list and number of files present in list
fileTrack* fileTracker(int index){
    for(int i=0; i<10 ; i++){
                memset(ft.files[i], '0', 255);
    }
    ft.fileNo=0;
    //get number of files [if more than 10 return pack of 10 or else whole]
    if(fileNo>10){ //is more than 10
            for(int i=index ; i < ((index!=0) ? ((fileNo-index>10)?10:(fileNo-index)) : 10); i++){
                memcpy(ft.files[i], demoFiles[i], 255);
                ft.fileNo+=1;
            }
            &ft; 
    }
    else{ //is less or equal 10
        return &ft;
    }
}