// file manager page

//------------Include Section----------
#include"fileManager.h"

//only for development purpose
#include"pico/bootrom.h"
//------------------------------------


//-----------Function Declaration--------------
void buttonInterruptFileManager();  //handles all button interrupts
void parseFiles(int index); //parse files from file tracker function
void drawUiChange(); //update ui with respect to button input
void drawFileNames(); //draws file names
//----------------------------------------------

//-----------Global variables----------------

fileManagerStruct fM;
//------------------------------------------

//entry section of code for every page
void entryfileManager(){
    fM.userPointer=0;
    fM.fileTrackingPointer=0;
    memset(ScreenBuffer, '0', IMAGE_SIZE);
    Paint_Clear(BACKGROUND);

    drawMenuTray();
    parseFiles(fM.userPointer);
    buttonInterruptFileManager();
}


// button press will be handled here
void buttonInterruptFileManager(){
    SET_Infrared_PIN(JOYSTICK_UP);
    SET_Infrared_PIN(JOYSTICK_DOWN);
    SET_Infrared_PIN(OK);
    SET_Infrared_PIN(SET);
    SET_Infrared_PIN(NONE);
    SET_Infrared_PIN(ESC);
    int pageSwitch; //0: Setting page; 1: Injection Page
    while(1){
        //JOYSTICK UP
        if((!DEV_Digital_Read(JOYSTICK_UP))){
            fM.userPointer-=1;
            fM.fileTrackingPointer-=1;
            drawUiChange(fM.userPointer);
            HALT(JOYSTICK_UP);
        }

        //JOYSTICK DOWN
        else if((!DEV_Digital_Read(JOYSTICK_DOWN))){
            fM.userPointer+=1;
            fM.fileTrackingPointer+=1;
            drawUiChange(fM.userPointer);
            HALT(JOYSTICK_DOWN);
        }

        //OK
        else if((!DEV_Digital_Read(OK))){
            pageSwitch=1;
            break;
            HALT(OK);
        }

        //SET
        else if((!DEV_Digital_Read(SET))){
            pageSwitch=0;
            break;
            HALT(SET);
        }

        //ESC
        else if((!DEV_Digital_Read(ESC))){
            printf("Joystick ESC pressed\n");
            HALT(ESC);
        }

        //NONE
        else if((!DEV_Digital_Read(NONE))){
            reset_usb_boot(0,0);
            HALT(ESC);
        }
    }

    //page switch
    switch(pageSwitch){
        case 0: entrySetting(); break;
        case 1: entryInjection(fM.fileNameList[fM.userPointer]); break;
    }
    
}


// function will ask the file tracker function to give it the files
void parseFiles(int index){
    fileTrack* ft = fileTracker(index);
    fM.fileCount=ft->totalFileCount;
    // resetting memory of array
    for(int i=0; i<FILECAP; memset(fM.fileNameList[i++], ' ', 255));
        
    for(int i=0; i<ft->fileNo; i++){
        memcpy(fM.fileNameList[i], ft->files[i], 255);
    }
    drawFileNames();
}

// draw file names
void drawFileNames(){
    int height=20;
    for(int i=0; i<FILECAP; i++){
        //Paint_DrawRectangle(50, height, SCREEN_WIDTH, height+30, (fM.userPointer==i)?THEME_SELECTEDBOX:BACKGROUND, DOT_PIXEL_1X1, (fM.userPointer==i)?DRAW_FILL_FULL:DRAW_FILL_EMPTY);
        Paint_DrawString_EN(55, height, fM.fileNameList[i], &Font20, (fM.userPointer==i)?FOREGROUND_SELECTED:FORGROUND, BACKGROUND);
        height+=45;
    }
    SUBMIT(ScreenBuffer);
}

void drawUiChange(){
    
    if(fM.userPointer>=FILECAP || fM.fileTrackingPointer>=fM.fileCount) {
        int nextIndex=(fM.fileTrackingPointer>=fM.fileCount)?(0, fM.fileTrackingPointer=0):fM.fileTrackingPointer;
        Paint_Clear(BACKGROUND);
        fM.userPointer=0;
        parseFiles(nextIndex);
        drawMenuTray();
    }

    else if(fM.userPointer<0 || fM.fileTrackingPointer<0){ 
        int nextIndex;
        if(fM.fileTrackingPointer<0){
            if(fM.fileCount%FILECAP==0){
                fM.userPointer=FILECAP-1; //-1 due to indexing
                fM.fileTrackingPointer=fM.fileCount-1; //-1 due to indexing
                nextIndex=fM.fileCount-FILECAP;
            }
            else{
                fM.userPointer=(fM.fileCount%FILECAP)-1; //-1 due to indexing
                nextIndex=fM.fileCount-(fM.fileCount%FILECAP);
                fM.fileTrackingPointer=fM.fileCount-1; //due to indexing
            }
        }
        else{
            nextIndex=fM.fileTrackingPointer-(FILECAP-1); //-1 due to indexing
            fM.userPointer=FILECAP-1; //-1 due to indexing
        }
        Paint_Clear(BACKGROUND);
        //fM.userPointer=FILECAP-1;
        parseFiles(nextIndex);
        drawMenuTray();
    }
    else drawFileNames();
    SUBMIT(ScreenBuffer);
}