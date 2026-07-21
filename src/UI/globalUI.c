// contains code for all implemented in more than one page 
#include"UI.h"
#include"images/uiImage.h"

const char menuItems[][5]={"OK", "---", "SET", "ESC"};
// draw menu tray at side
void drawMenuTray(){
    Paint_DrawRectangle(45, 10, SCREEN_HEIGHT, SCREEN_WIDTH, GREEN , DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    
    //Paint_DrawString_EN(10, 30, menuItems[0], &Font12, FORGROUND, BACKGROUND);
    Paint_DrawImage(OK_button, 0, 10,  40, 40);

    Paint_DrawImage(usb_button, 1, 71, 40, 40);
    Paint_DrawRectangle(1, 70, 43, 115, WHITE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    //Paint_DrawString_EN(12, 85, menuItems[1], &Font12, FORGROUND, BACKGROUND);

    
    //Paint_DrawString_EN(10, 150, menuItems[2], &Font12, FORGROUND, BACKGROUND);
    Paint_DrawImage(Setting_button, 0, 130,  40, 40);
    
    //Paint_DrawString_EN(10, 210, menuItems[3], &Font12, FORGROUND, BACKGROUND);
    Paint_DrawImage(ESC_button, 0, 190,  40, 40);

    //SUBMIT(ScreenBuffer);
}

//------------Global datatype definition----------------
int themesLayout[]={CYAN, GREEN};
int themeForegroundSelected[]={RED, MAGENTA};
//------------Global datatype definition end----------------