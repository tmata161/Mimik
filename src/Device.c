// this is the master branch of the device

#include<stdio.h>
#include"../masterHeader.h"
#include"UI/fileManager.h"
#include"../../lib/lfs/init_filesystem.h"


UWORD* ScreenBuffer;
FATFS fat;

void initScreen();
void drawBootLogo();

void master(){
    //init LFS file system
    //setup screen
    initScreen();
    //draw boot logo
    drawBootLogo();
    //INIT sd card 
    free(ScreenBuffer);
     initScreen();
     initFilesystem(&fat);
    //open filemanger screen
    entryfileManager(&fat);
}

void initScreen(){
    if(DEV_Module_Init()!=0){
        ERROR("Could not initialize library, pins or spi protocol\n");
        return;
    }

    // initializes pwm stuff
    DEV_SET_PWM(100);
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
    char cDown[10];
    Paint_DrawString_EN((SCREEN_WIDTH/2)-40, (SCREEN_HEIGHT/2)-24, "Mimik", &Font24, WHITE, BACKGROUND);
   
//loading screen
    for(int i=5; i>=1; i--){
        sprintf(cDown, "%d" ,i);
        Paint_DrawString_EN(SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)+25, cDown, &Font20, CYAN, BACKGROUND);
        SUBMIT(ScreenBuffer);
        DEV_Delay_ms(1000);
    }

    Paint_Clear(BLACK);
}