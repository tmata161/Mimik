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

void initScreen(){
    if(DEV_Module_Init()!=0){
        ERROR("Could not initialize library, pins or spi protocol\n");
        return;
    }

    // initializes pwm stuff
    DEV_SET_PWM(100);
    // initializes the lcd
    LCD_1IN3_Init(HORIZONTAL);
    // clear screen
    LCD_1IN3_Clear(WHITE);

    // setting backlight
    LCD_1IN3_SetBacklight(1023);
    if( allocateScreenBuffer() == NULL) {
        ERROR("Failed to setup memory...\n");
        exit(0);
    }

    Paint_NewImage((UBYTE *)ScreenBuffer,SCREEN_WIDTH,SCREEN_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);
}


void drawBootLogo(){
    const unsigned char *(logoArray[])={frame_0_delay_0_1s, frame_1_delay_0_1s, frame_2_delay_0_1s, frame_3_delay_0_1s, frame_4_delay_0_1s, frame_5_delay_0_1s};
    Paint_Clear(WHITE);
   
    //loading screen
    for(int time=1; time<=5; time++){
        for(int i=0; i<6; i++){
        unsigned char temp[240*240];
        memset(temp, 0xFF, 240*240);
        for(int y=0; y<60; y++){
            for(int x=0; x<240; x++){
                //a sort of interpolation algorithm
                temp[((y*4)*120)+x*2] = logoArray[i][(y*120)+x];
                temp[((y*4)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];
          

                temp[(((y*4)+1)*120)+x*2] = logoArray[i][(y*120)+x];
                temp[(((y*4)+1)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];

                temp[(((y*4)+2)*120)+x*2] = logoArray[i][(y*120)+x];
                temp[(((y*4)+2)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];
                
                temp[(((y*4)+3)*120)+x*2] = logoArray[i][(y*120)+x];
                temp[(((y*4)+3)*120)+((x*2)+1)] = logoArray[i][(y*120)+x];
            }
        }
        
        Paint_DrawImage(temp, 55, 55, 120, 480);
        SUBMIT(ScreenBuffer);
        sleep_ms(0.016);
        }
        Paint_Clear(WHITE);
    }
}