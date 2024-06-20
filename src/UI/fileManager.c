// file manager page

//------------Include Section----------
#include"fileManager.h"
//only for development purpose
#include"pico/bootrom.h"
//------------------------------------


//-----------Function Declaration--------------
void buttonInterruptFileManager();  //handles all button interrupts
void parseFiles(int index); //parse files from file tracker function
void handleScrollUI(); //update ui with respect to button input
void handleOK();
void handleESC();
void drawFileNames(); //draws file names
int fileTracker(int index);
void fillFileArray();
void openDirectory();
void closeDirectory();
void go();
//----------------------------------------------

//-----------Global variables----------------
fileManagerStruct fM;
char demoFiles[25][100]={};
//fileTrack ft;
FF_DIR gDIR; //a global directory structure variable
SDFile sdf; //holds data of 50 entries of a folder in ram
char cDir[100]="/"; //holds location of current directory
//------------------------------------------

//entry section of code for every page
void entryfileManager(){
    openDirectory();
    fillFileArray();
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
            handleScrollUI(fM.userPointer);
            HALT(JOYSTICK_UP);
        }

        //JOYSTICK DOWN
        else if((!DEV_Digital_Read(JOYSTICK_DOWN))){
            fM.userPointer+=1;
            fM.fileTrackingPointer+=1;
            handleScrollUI(fM.userPointer);
            HALT(JOYSTICK_DOWN);
        }

        //OK
        else if((!DEV_Digital_Read(OK))){
            handleOK();
            //pageSwitch=1;
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
            handleESC();
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
        case 1: entryInjection(fM.Info[fM.userPointer].fname); break;
    }
    
}


// function will ask the file tracker function to give it the files
void parseFiles(int index){
    int fileNo = fileTracker(index);
    fM.fileCount=sdf.fCount;
    drawFileNames();
}

// draw file names
void drawFileNames(){
    int height=20;
    Paint_DrawRectangle(45+5, 10+5, SCREEN_HEIGHT-5, SCREEN_WIDTH-5, BACKGROUND , DOT_PIXEL_1X1, DRAW_FILL_FULL);
    for(int i=0; i<FILECAP; i++){
        Paint_DrawString_EN(60, height, fM.Info[i].fname , (fM.userPointer==i)? &Font20 :&Font16, (fM.userPointer==i)?FOREGROUND_SELECTED:FORGROUND, BACKGROUND);
        height+=45;
    }
    SUBMIT(ScreenBuffer);
    
}

void handleScrollUI(){

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
}

//this function handles ok button event
void handleOK(){
    //chk if current entity is file or folder
    if((fM.Info[fM.userPointer].fattrib) == AM_DIR && fM.Info[fM.userPointer].fname!=0){
        //close current directory object
        closeDirectory();
        go();
        printf("target directory %s\n", cDir);
        //delete old file info object
        memset(&sdf, '\0', sizeof(FILINFO));
        //delete old file manager object
        memset(&fM, ' ', sizeof(fileManagerStruct));
        //clear screen blank
        Paint_Clear(BACKGROUND);
        //call entry file manager function again to draw everything back
        entryfileManager();
    }
    else {
        go();
        printf("target file %s\n", cDir);
        entryInjection(cDir);
    }
}

//this function handles esc button event
void handleESC(){
    //chk if folder is not root
    if(strcmp(cDir, "/")!=0){
        //close current directory object
        closeDirectory();
        back();      
        printf("target directory %s\n", cDir);
        //delete old file info object
        memset(&sdf, '\0', sizeof(FILINFO));
        //delete old file manager object
        memset(&fM, '\0', sizeof(fileManagerStruct));
        //clear screen blank
        Paint_Clear(BACKGROUND);
        //call entry file manager function again to draw everything back
        entryfileManager();
    }
    else {
        printf("target directory %s\n", cDir);
        }
}
// read files from demo list and show on screen
// argument-> index of file
// return -> object of file list and number of files present in list
int fileTracker(int index){
    int fileNo=0;
    //if total files in the directory is more than 5 : do maths else : just copy
    if(sdf.fCount>FILECAP){ //is more than 10
            for(int i=index ; i < ((index!=0) ? ((sdf.fCount-index>FILECAP)?FILECAP+index:sdf.fCount) : FILECAP); i++){
                memcpy(&fM.Info[fileNo], &sdf.Info[i], sizeof(FILINFO));
                fileNo+=1;
                
            }
           return fileNo; 
    }
    else{ //is less or equal 5
        for(int i=0; i<sdf.fCount; i++){
            memcpy(&fM.Info[fileNo], &sdf.Info[i], sizeof(FILINFO));
            fileNo+=1;
        }
        return fileNo;
    }
}

void fillFileArray(){
    //count total number of files files
    sdf.fCount=countFiles(&fat, cDir);
    if(sdf.fCount>0){ //if there something present inside the directory
        listFile(&fat, cDir, &gDIR, sdf.Info);
    }
}

void openDirectory(){
    f_opendir(&fat, &gDIR, cDir);
}

void closeDirectory(){
    f_closedir(&gDIR);
}

void go(){
//add a slash to the target directory
        if(strlen(cDir)>1)strcat(cDir, "/");
        //add new location to the directory array
        strcat(cDir, fM.Info[fM.userPointer].fname);
}

void back(){
//goto directory residing above it
        for(int i=sizeof(cDir)-1; i>=0; i--){ 
            //trigger if got a [/]
            if(cDir[i]=='/'){
                //add all null character after
                for(int k=(i==0)?i+1:i; k<sizeof(cDir); k++){
                    cDir[k]='\0';
                }
                break;
            }
        }
}