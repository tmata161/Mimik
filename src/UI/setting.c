// for setting page
#include"UI.h"
#include"../../lib/lfs/init_filesystem.h"

void buttonInterruptSetting();
void redrawSetting();
void drawItemSetting();
void drawUIChange();

char* filecontent;
char items[][10]={"Theme:", "Dry Run:"};
int itemCount=sizeof(items)/sizeof(items[0]);
settingPage st;
//entry section of code for every page
void entrySetting(){
    st.userPointer=0;
    redrawSetting();
    buttonInterruptSetting();
    return;
}

void redrawSetting(){
    // Paint_Clear(BACKGROUND);
    // drawMenuTray();
    // drawItemSetting();

    notification("This functionality has not been implemented yet :(");
}

void buttonInterruptSetting(){
    while(1){
        //ESC
        if((!DEV_Digital_Read(ESC))){
            break;
            HALT(ESC);
        }
        //Joystick UP
        else if((!DEV_Digital_Read(JOYSTICK_UP))){
            st.userPointer-=1;
            drawUIChange();
            HALT(JOYSTICK_UP);
        }
        //Joystick Down
        else if((!DEV_Digital_Read(JOYSTICK_DOWN))){
            st.userPointer+=1;
            drawUIChange();
            HALT(JOYSTICK_DOWN);
        }
    }
    
    // //free memory
    // free(ScreenBuffer);ScreenBuffer=NULL;
    // //save contents
    // char _saving[]={"Theme: 0\nDryRun: 0"};
    // lfs_file_t f;lfs_file_open(&f, "setting", LFS_O_RDWR);lfsFileWrite(&f, _saving);lfs_file_close(&f);
    // //set memory and go back
    // ScreenBuffer=(UWORD*)malloc(IMAGE_SIZE);
    // if(ScreenBuffer==NULL){ERROR("Unable to setup memory for screen buffer\n");exit(1);}
    // initScreen();
    return;
}

void drawItemSetting(){ 
    int height=20;
    Paint_DrawRectangle(45+5, 10+5, SCREEN_HEIGHT-5, SCREEN_WIDTH-5, BACKGROUND , DOT_PIXEL_1X1, DRAW_FILL_FULL);
    for(int i=0; i<itemCount; i++){
    Paint_DrawString_EN(60, height, items[i], (st.userPointer==i)?&Font20:&Font16, (st.userPointer==i)?FOREGROUND_SELECTED:FORGROUND, BACKGROUND);
    height+=45;
    }
    SUBMIT(ScreenBuffer);
}

void drawUIChange(){
    //code for checking user pointer is in bounds
    if(st.userPointer >= (int)sizeof(itemCount)-1) st.userPointer=itemCount-1;
    else if(st.userPointer<0) st.userPointer=0;
    drawItemSetting();
}