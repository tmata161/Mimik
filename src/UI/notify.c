#include"UI.h"

// no memory card detected page
void notification(char* mess){
    Paint_Clear(BACKGROUND);
    Paint_DrawString_EN(5, SCREEN_HEIGHT/3, mess, &Font20, WHITE, BLACK);
    SUBMIT(ScreenBuffer);
}