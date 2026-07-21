#include"../mimik.h"
#include"UI/images/bootlogo.h"

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
        ERROR("Could not initialize library, pins or spi protocol\n");
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
        ERROR("Failed to setup memory...\n");
        return 0;
    }

    Paint_NewImage((UBYTE *)ScreenBuffer,SCREEN_WIDTH,SCREEN_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);
    return 1;
}


void drawBootLogo(){
   // const unsigned char *(logoArray[])={frame_0_delay_0_1s, frame_1_delay_0_1s, frame_2_delay_0_1s, frame_3_delay_0_1s, frame_4_delay_0_1s, frame_5_delay_0_1s};
   // Paint_Clear(WHITE);
   Paint_Clear(BLACK);
    // //loading screen
    // for(int time=1; time<=2; time++){
    //     for(int i=0; i<6; i++){
    //     unsigned char temp[240*240];
    //     memset(temp, 0xFF, 240*240);
    //     for(int y=0; y<60; y++){
    //         for(int x=0; x<240; x++){
    //             //a sort of interpolation algorithm
    //             temp[((y*4)*120)+x*2] = logoArray[i][(y*120)+x];
    //             temp[((y*4)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];
          

    //             temp[(((y*4)+1)*120)+x*2] = logoArray[i][(y*120)+x];
    //             temp[(((y*4)+1)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];

    //             temp[(((y*4)+2)*120)+x*2] = logoArray[i][(y*120)+x];
    //             temp[(((y*4)+2)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];
                
    //             temp[(((y*4)+3)*120)+x*2] = logoArray[i][(y*120)+x];
    //             temp[(((y*4)+3)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];
    //         }
    //     }
        
    //     Paint_DrawImage(temp, 55, 55, 120, 480);
    //     SUBMIT(ScreenBuffer);
    //     }
    //     Paint_Clear(WHITE);
    // }


    unsigned int show=1;
    int counter=0;
    while ((counter++)<=4)
    {Paint_Clear(BLACK);
        if(show){
        // Paint_DrawLine(30, 90, 60, 110, WHITE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        // Paint_DrawLine(60, 110, 30, 130, WHITE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

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