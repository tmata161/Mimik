// contains code for all implemented in more than one page 
#include"UI.h"

const char menuItems[][5]={"OK", "---", "SET", "ESC"};
// draw menu tray at side
void drawMenuTray(){
    Paint_DrawRectangle(45, 10, SCREEN_HEIGHT, SCREEN_WIDTH, THEME_BACK_LAYOUT , DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // now draw the icons
    //Paint_DrawImage1(usb, 10, 20, 20, 40);

    //Paint_DrawLine(10, 35, 38, 35, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); //X_DIFFERENCE = 20, Y_DIFFERENCE = 25
    Paint_DrawString_EN(10, 30, menuItems[0], &Font12, FORGROUND, BACKGROUND);

    Paint_DrawRectangle(10, 80, 30, 105, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); 
    Paint_DrawString_EN(10, 90, menuItems[1], &Font12, FORGROUND, BACKGROUND);

    Paint_DrawRectangle(10, 140, 30, 165, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(10, 150, menuItems[2], &Font12, FORGROUND, BACKGROUND);

    Paint_DrawRectangle(10, 200, 30, 225, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(10, 210, menuItems[3], &Font12, FORGROUND, BACKGROUND);

    SUBMIT(ScreenBuffer);
}