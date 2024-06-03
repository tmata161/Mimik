// file manager page
#include"fileManager.h"

//-----------Function Declaration--------------
void buttonInterrupt();
void drawMenuTray();
void showFiles();
//----------------------------------------------

//-----------Global variables----------------
const char menuItems[][5]={"OK", "---", "SET", "ESC"};
fileManagerStruct fM;

//------------------------------------------

//entry section of code for every page
void entryfileManager(){
    printf("Entry function of file manager screen\n");
    drawMenuTray();
    buttonInterrupt();
}


// button press will be handled here
void buttonInterrupt(){
    SET_Infrared_PIN(JOYSTICK_UP);
    SET_Infrared_PIN(JOYSTICK_DOWN);
    SET_Infrared_PIN(OK);
    SET_Infrared_PIN(SET);
    SET_Infrared_PIN(NONE);
    SET_Infrared_PIN(ESC);
    while(1){
        
    }
}

// draw menu tray at side
void drawMenuTray(){
    Paint_DrawRectangle(45, 10, SCREEN_HEIGHT, SCREEN_WIDTH, CYAN , DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // now draw the icons
    //Paint_DrawImage1(usb, 10, 20, 20, 40);

    //Paint_DrawLine(10, 35, 38, 35, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); //X_DIFFERENCE = 20, Y_DIFFERENCE = 25
    Paint_DrawString_EN(10, 30, menuItems[0], &Font12, WHITE, BLACK);

    Paint_DrawRectangle(10, 80, 30, 105, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); 
    Paint_DrawString_EN(10, 90, menuItems[1], &Font12, WHITE, BLACK);

    Paint_DrawRectangle(10, 140, 30, 165, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(10, 150, menuItems[2], &Font12, WHITE, BLACK);

    Paint_DrawRectangle(10, 200, 30, 225, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(10, 210, menuItems[3], &Font12, WHITE, BLACK);

    SUBMIT(ScreenBuffer);
}

// function will ask the file tracker function to give it the files
void showFiles(){
    fM.userPointer=0;
}