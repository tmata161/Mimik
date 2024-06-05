// for setting page
#include"setting.h"

void buttonInterruptSetting();
void redrawSetting();

//entry section of code for every page
void entrySetting(){
redrawSetting();
buttonInterruptSetting();
}

void redrawSetting(){
    memset(ScreenBuffer, '0', IMAGE_SIZE);
    Paint_Clear(BACKGROUND);
    drawMenuTray();
    Paint_DrawString_EN(20, SCREEN_HEIGHT/2, "Setting Page", &Font24, FORGROUND, BACKGROUND);
    SUBMIT(ScreenBuffer);
}

void buttonInterruptSetting(){
    while(1){
        if((!DEV_Digital_Read(ESC))){
            break;
            HALT(ESC);
        }
    }
    entryfileManager();
}