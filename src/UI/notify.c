#include"UI.h"

// no memory card detected page
void notification(char* mess){
    Paint_Clear(BLACK);
    Paint_DrawString_EN(20, 95, mess, &Font16, RED, BLACK);
    SUBMIT(ScreenBuffer);
}