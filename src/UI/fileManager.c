// file manager page

//------------Include Section----------
#include"UI.h"
//only for development purpose
#include"pico/bootrom.h"
//------------------------------------


//-----------Function Declaration--------------
void go();
void buttonInterruptFileExplorer();  //handles all button interrupts
void handleOK(fileExplorerObject* exobj);
void handleESC(fileExplorerObject* exobj);
void fetchFiles(fileExplorerObject* exobj, char cDir[50], FF_DIR* gDIR);
void openDirectory(FF_DIR* gDIR, char* cDir);
void closeDirectory(FF_DIR* gDIR, char* cDir);
void initPage(fileExplorerObject* exobj);
void drawFiles(fileExplorerObject* exobj);
void pageMechanism(fileExplorerObject* exobj);
//----------------------------------------------

//------------new code-------------------



//entry section of code for every page
fileExplorerObject entryfileManager(fileExplorerObject obj){
    FF_DIR gDIR; //a global directory structure variable
    fileExplorerObject exobj; //contain entries of all objects present inside a directory
    memset(ScreenBuffer, '\0', IMAGE_SIZE);
    exobj=obj; //make a copy of the object
    initPage(&exobj);
    openDirectory(&gDIR, exobj.navigationLocation);
    fetchFiles(&exobj, exobj.navigationLocation, &gDIR);

    //only for testing
    printf("%s\n",exobj.navigationLocation, exobj.fP[exobj.fSP]);
    for(int i=0; i<exobj.TFC; i++){
        printf("\t%s\n", exobj.files[i].fname);
    }printf("\n");

    if(exobj.TFC==0){
        notification("empty folder");
        while(1){
            if(!DEV_Digital_Read(ESC)){
                handleESC(&exobj);
                break;
            }
        }
        return exobj;
    }
    pageMechanism(&exobj);
    buttonInterruptFileExplorer(&exobj);
    closeDirectory(&gDIR, exobj.navigationLocation);
    return exobj;
}

// *-------------------------------------------------------------------------Button handler---------------------------------
void buttonInterruptFileExplorer(fileExplorerObject* exobj){
    SET_Infrared_PIN(JOYSTICK_UP);
    SET_Infrared_PIN(JOYSTICK_DOWN);
    SET_Infrared_PIN(OK);
    SET_Infrared_PIN(SET);
    SET_Infrared_PIN(NONE);
    SET_Infrared_PIN(ESC);
    int pageSwitch=-1; //0: Setting page; 1: Injection Page
    while(1){
        //JOYSTICK UP
        if((!DEV_Digital_Read(JOYSTICK_UP))){
            exobj->fP[exobj->fSP]-=1;
            exobj->fP[exobj->fSP]=(exobj->fP[exobj->fSP]==0xFFFFFFFF)?0:exobj->fP[exobj->fSP];
            pageMechanism(exobj);
           // printf("file pointer: %u\nfile name: %s\n\n", exobj.fP, exobj.files[exobj.fP].fname);
            HALT(JOYSTICK_UP);
        }

        //JOYSTICK DOWN
        else if((!DEV_Digital_Read(JOYSTICK_DOWN))){
            exobj->fP[exobj->fSP]+=1;
            exobj->fP[exobj->fSP]=(exobj->fP[exobj->fSP] >= exobj->TFC)?exobj->TFC-1:exobj->fP[exobj->fSP];
            pageMechanism(exobj);
            //printf("file pointer: %u\nfile name: %s\n\n", exobj.fP, exobj.files[exobj.fP].fname);
            HALT(JOYSTICK_DOWN);
        }

        //OK
        else if((!DEV_Digital_Read(OK))){
            handleOK(exobj);
            printf("ok: %s\n", exobj->navigationLocation);
            HALT(OK);
            break;
        }

        //SET
        // else if((!DEV_Digital_Read(SET))){
        //     entrySetting();
        //     HALT(SET);
        //     break;
        // }

        //ESC
        else if((!DEV_Digital_Read(ESC))){
            handleESC(exobj);
            printf("esc: %s\n", exobj->navigationLocation);
            HALT(ESC);
            break;
        }

        //NONE
        // else if((!DEV_Digital_Read(NONE))){
        //     //unmount fat and turn of system
        //     printf("trying to unmount\n");
        //     notification("unmounting system (may take some time)");
        //     f_umount(&fat);
        //     reset_usb_boot(0,0);
        //     HALT(NONE);
        // }

    //for memory card detection
        //  if(!detectCard()){
        //  notification("No memory card  detected :(");
        //  exit(1); //halt program indefinitely
        //  }
    
    }
    //page switch
    // switch(pageSwitch){
    //     case 0: entrySetting();break;
    //     case 1: notification("this functionality has not been implemented yet"); break;
    //     default:break;
    //     break;
    // }
    return;
}
//this function handles esc button event
void handleESC(fileExplorerObject* exobj){
    //remove upper most file location
    int len=strlen(exobj->navigationLocation);
    if(exobj->navigationLocation[len-1]!='/'){
         for(int i=len-1; i>=0; i--){ 
            //stop the loop if got a [/]
            if(exobj->navigationLocation[i]!='/'){
                exobj->navigationLocation[i]='\0';
            }
            else{
                if(i!=0)exobj->navigationLocation[i]='\0'; //remove unwanted slash
                break;
            }
        }
        exobj->fP[exobj->fSP]=0;
        exobj->fSP--;
    }
    //else printf("target directory %s\n", cDir);
    return;
}
//TODO- implement a handler for nagivationlocation exceeding its limit
//this function handles ok button event
void handleOK(fileExplorerObject* exobj){
    //chk if current entity is file or folder
    if(exobj->TFC>0){
       if((exobj->files[exobj->fP[exobj->fSP]].fattrib) == AM_DIR && exobj->files[exobj->fP[exobj->fSP]].fname!=0){
            // an operation to operate on cDir
            //add a slash to the target directory
            if(strlen(exobj->navigationLocation)>1)strcat(exobj->navigationLocation, "/");
                //add new location to the directory array
                strcat(exobj->navigationLocation, exobj->files[exobj->fP[exobj->fSP]].fname);
                exobj->fSP++;
                exobj->fP[exobj->fSP]=0; //otherwise it gets initialize to 1 after object copy
        } 
    }
    //else entrySetting();//entryInjection(cDir);
    return;
}
// !-------------------------------------------------------------------------Button handler end---------------------------------



// *-------------------------------------------------------------------------UI handler---------------------------------
// draw files on the screen
void drawFiles(fileExplorerObject* exobj){
    int height=20;
    Paint_DrawRectangle(45+5, 10+5, SCREEN_HEIGHT-5, SCREEN_WIDTH-5, BACKGROUND , DOT_PIXEL_1X1, DRAW_FILL_FULL); //draw outline rectangle
    int filesToDrawCount = (exobj->LB+(exobj->TFC-(exobj->slateNo*MFO))-1);//(exobj->UB > exobj->TFC)? exobj->TFC-exobj->LB : exobj->UB;
    for(int i=exobj->LB; i<=filesToDrawCount; i++){
        Paint_DrawString_EN(60, height, exobj->files[i].fname , (i==exobj->fP[exobj->fSP])? &Font20 :&Font16, (i==exobj->fP[exobj->fSP])?FOREGROUND_SELECTED:FORGROUND, BACKGROUND);
        height+=45;
    }
    SUBMIT(ScreenBuffer);
}
void initPage(fileExplorerObject* exobj){
    //initialize file explorer object
    exobj->LB=exobj->UB=exobj->TFC=exobj->slateNo=0;
    //initialize screen buffer
    for(int i=0; i<IMAGE_SIZE; i++){ScreenBuffer[i]=0;}
    Paint_Clear(BACKGROUND);
}
void pageMechanism(fileExplorerObject* exobj){
    //slate calculation
    exobj->slateNo=(int)(exobj->fP[exobj->fSP]/MFO);
    //upper bound and lower bound calculator
    exobj->LB = exobj->slateNo * MFO;
    exobj->UB = (exobj->slateNo * MFO) + 4;
    //Paint_Clear(BACKGROUND);
    SUBMIT(ScreenBuffer);
    drawMenuTray();
    drawFiles(exobj);
}
// !-------------------------------------------------------------------------UI handler end-----------------------------


// *------------------------------------------------------------------------filesystem logic-----------------------------
//fetch files from SD card and store it in memory
void fetchFiles(fileExplorerObject* exobj, char* cDir, FF_DIR* gDIR){
    exobj->TFC=countFiles(&fat, cDir);
    exobj->TFC=(exobj->TFC>FILE_COPY_LIMIT)?FILE_COPY_LIMIT:exobj->TFC;
    if(exobj->TFC>0){
        copyFiles(&fat, cDir, gDIR, exobj->files);
    }
}

void openDirectory(FF_DIR* gDIR, char* cDir){
    f_opendir(&fat, gDIR, cDir);
}

void closeDirectory(FF_DIR* gDIR, char* cDir){
    f_closedir(gDIR);
}
// !------------------------------------------------------------------------filesystem logic end-----------------------------