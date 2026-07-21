#include"../mimik.h"

UWORD* ScreenBuffer;

UWORD* allocateScreenBuffer(){
    ScreenBuffer = (UWORD *)malloc(IMAGE_SIZE);
    return ScreenBuffer;
}

void de_allocateScreenBuffer(){
    free(ScreenBuffer);
}

int initScreen(){
    if(DEV_Module_Init()!=0){
        DEBUG("Could not initialize library, pins or spi protocol\n");
        return 0;
    }

    // initializes pwm stuff
    DEV_SET_PWM(100);
    // initializes the lcd
    LCD_1IN3_Init(HORIZONTAL);
    // clear screen
    LCD_1IN3_Clear(WHITE);

    // setting backlight
    LCD_1IN3_SetBacklight(1024);
    if( allocateScreenBuffer() == NULL) {
        DEBUG("Failed to setup memory...\n");
        return 0;
    }

    Paint_NewImage((UBYTE *)ScreenBuffer,SCREEN_WIDTH,SCREEN_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);
    return 1;
}


void drawBootLogo(){
   Paint_Clear(BLACK);
    unsigned int show=1;
    int counter=0;
    while ((counter++)<=4)
    {Paint_Clear(BLACK);
        if(show){
        Paint_DrawLine(100, 90, 130, 110, WHITE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine(130, 110, 100, 130, WHITE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        }
        show=!show;
        SUBMIT(ScreenBuffer);
        board_delay(600);
    }

    //show device name
    Paint_Clear(BLACK);
    SUBMIT(ScreenBuffer);
    board_delay(600);
    int pos_x=70;
    counter=0;
    char* devName="MIMIK";
    while((counter++) <= 5)
    {
        Paint_DrawString_EN(70, 100, devName, &Font24, WHITE, BLACK);
        Paint_DrawRectangle(pos_x, 100, 180, 130, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        pos_x+=17;
        SUBMIT(ScreenBuffer);
        board_delay(200);
    }
    board_delay(800);
}