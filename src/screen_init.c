
//will run once after boot
void setup_screen(){
    DEV_Module_Init();
    DEV_SET_PWM(90);
    LCD_2IN_Init(HORIZONTAL);
    LCD_2IN_Clear(BLACK);
    LCD_2IN_SetBacklight(1023);
    if((ScreenBuffer = (UWORD*)malloc(IMAGE_SIZE))==NULL){
        printf("Unable to reserve memory space for screen.\n");
    }
    Paint_NewImage((UBYTE *)ScreenBuffer,SCREEN_WIDTH,SCREEN_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_270);
}

// keeps track of the file that has to be parsed
//retun index track
int fetch_files(char** fileList, int indx){
return 0;
}