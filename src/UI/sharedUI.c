#include"UI.h"
#include"images/icons.h"

const char menuItems[][5]={"OK", "---", "SET", "ESC"};
// draw menu tray at side
void drawMenuTray(){
    Paint_DrawRectangle(45, 10, SCREEN_HEIGHT, SCREEN_WIDTH, GREEN , DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawImage(OK_button, 0, 10,  40, 40);
    Paint_DrawImage(usb_button, 1, 71, 40, 40);
    Paint_DrawRectangle(1, 70, 43, 115, WHITE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    Paint_DrawImage(Setting_button, 0, 130,  40, 40);
    Paint_DrawImage(ESC_button, 0, 190,  40, 40);

}

void notification(char* mess){
    Paint_Clear(BLACK);
    Paint_DrawString_EN(20, 95, mess, &Font16, RED, BLACK);
    SUBMIT(ScreenBuffer);
}

//------------Global datatype definition----------------
// int themesLayout[]={CYAN, GREEN};
// int themeForegroundSelected[]={RED, MAGENTA};
//------------Global datatype definition end----------------