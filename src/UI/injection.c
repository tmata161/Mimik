// injection page

#include"injection.h"

void buttonInterruptInjection();
void redrawInjection();

#define FILE_SIZE_LIMIT 50*1024


int fileSize;
char *fileContent;
//entry section of code for every page
void entryInjection(char* selectedFile){
    fileSize=file_size(&fat, cDir);
    if(fileSize < FILE_SIZE_LIMIT && fileSize !=0){
        //copy file content to a buffer
        fileContent = readFile(&fat, cDir, fileSize);
    }
    else{
        if(fileSize==0){
            printf("filesize is 0");
            fileContent=(char *) malloc(17);
            memcpy(fileContent, "File is Empty :/", 17);
        }
        else {
            //write file size should be under 50KB
            fileContent=(char *) malloc(31);
            memcpy(fileContent, "File size should be under 50KB", 31);
        }
    }
    redrawInjection();
    buttonInterruptInjection();
}

void redrawInjection(){
    memset(ScreenBuffer, '\0', IMAGE_SIZE);
    Paint_Clear(BACKGROUND);
    drawMenuTray();
    Paint_DrawString_EN(50, 30, fileContent, &Font16, FORGROUND, BACKGROUND);
    SUBMIT(ScreenBuffer);
}

void buttonInterruptInjection(){
    while(1){
        if((!DEV_Digital_Read(ESC))){
            break;
            HALT(ESC);
        }
    }
    back();
    entryfileManager();
}