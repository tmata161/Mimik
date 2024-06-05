// injection page

#include"injection.h"

void buttonInterruptInjection();
void redrawInjection();

char file[255];
//entry section of code for every page
void entryInjection(char* selectedFile){
    memcpy(file, selectedFile, sizeof(file));
    redrawInjection();
    buttonInterruptInjection();
}

void redrawInjection(){
    memset(ScreenBuffer, '0', IMAGE_SIZE);
    Paint_Clear(BACKGROUND);
    drawMenuTray();
    Paint_DrawString_EN(50, 30, file, &Font16, FORGROUND, BACKGROUND);
    Paint_DrawString_EN(20, SCREEN_HEIGHT/2, "Injection Page", &Font24, FORGROUND, BACKGROUND);
    SUBMIT(ScreenBuffer);
}

void buttonInterruptInjection(){
    while(1){
        if((!DEV_Digital_Read(ESC))){
            break;
            HALT(ESC);
        }
    }
    entryfileManager();
}