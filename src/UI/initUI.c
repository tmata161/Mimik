// contains functions that initializes ui 

#include "UI.h"


//----------------------------------C CODE FOR HOMEPAGE-------------------------------------------
homePage homePageStr;
#define SCREEN_SIZE 250*250

void setupScreen();
void drawMenuTray();
void setupScreen();
void drawMenuTray();
void initStructures(); //initializes all variables inside the structures
// communicates with secondary storage and keep data in homepage structure
    void initHomePage(){
        //lfs_init(&homePageStr);
        //hid_init();
        setupScreen();
        drawMenuTray();
        initStructures();
    }

    void setupScreen(UWORD *ScreenBuffer){
    DEV_Module_Init();
    DEV_SET_PWM(90);
    LCD_2IN_Init(HORIZONTAL);
    LCD_2IN_Clear(BLACK);
    LCD_2IN_SetBacklight(1023);
    if((ScreenBuffer = (UWORD*)malloc(SCREEN_SIZE))==NULL){
        printf("Unable to reserve memory space for screen.\n");
    }
    Paint_NewImage((UBYTE *)ScreenBuffer,SCREEN_WIDTH,SCREEN_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_270);
    }

    void drawMenuTray(){
        Paint_DrawRectangle(45, 10, SCREEN_HEIGHT, SCREEN_WIDTH, CYAN , DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // now draw the icons
    //Paint_DrawImage1(usb, 10, 20, 20, 40);

    //Paint_DrawLine(10, 35, 38, 35, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); //X_DIFFERENCE = 20, Y_DIFFERENCE = 25
    Paint_DrawString_EN(10, 30, "OK", &Font12, WHITE, BLACK);

    Paint_DrawRectangle(10, 80, 30, 105, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); 
    Paint_DrawString_EN(10, 90, "SET", &Font12, WHITE, BLACK);

    Paint_DrawRectangle(10, 140, 30, 165, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(10, 150, "RUN", &Font12, WHITE, BLACK);

    Paint_DrawRectangle(10, 200, 30, 225, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(10, 210, "BACK", &Font12, WHITE, BLACK);
    }

    void initStructures(){
        homePageStr.userPointer=1; //i.e pointing to the first payload by default
    }

    //Syncs RAM with Display
    void sync(UWORD *ScreenBuffer){
        LCD_2IN_Display((uint8_t*)ScreenBuffer);
    }

    // void setupButtons(){
    //     SET_Infrared_PIN(JOYSTICK_UP);
    //     SET_Infrared_PIN(JOYSTICK_DOWN);
    //     SET_Infrared_PIN(JOYSTICK_RIGHT);
    //     SET_Infrared_PIN(JOYSTICK_LEFT);

    //     SET_Infrared_PIN(OK);
    //     SET_Infrared_PIN(SETTING);
    //     SET_Infrared_PIN(RUN);
    //     SET_Infrared_PIN(BACK);
    // }
//-----------------------------------------------------------------------------


// read config file and add items in settings structure