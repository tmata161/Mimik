// #include"../bootducky.h"
// #include "pico/bootrom.h"
// #include "Themes.h"

// #define isPressed(no) (((no)==0)? 1 : 0)
// #define define_color(val) ((strcmp(inf.user_pointer.p_filename, inf.file_name[(val)]) == 0)?inf.config.col_code:WHITE)
// // boots into normal mode (This is the default page of the app)
// #define option_list_length 2

// char option_list[option_list_length][12]={{"About"},{"Theme"}};
// UWORD *ScreenBuffer;
// interface_handler inf;
// int temp;


// void setup_inf();
// void joystick_handler(int c);
// void settings_screen();
// void bootlogo_screen();
// void redraw(bool from_start);
// void view_script_screen();
// void draw_menu_tray();
// void setup_screen();
// void draw_files_directory(const char *names, int x_start, int y_start, int Color, sFONT *font_size);

// void init_config_file(){
    
//     // use to setup config file
//     int file_size=0;
//     char newfile[sizeof(struct conf)];
//     char *filebuffer = lfs_read(".config", &file_size);
//     if(filebuffer==NULL) {
//         //cannot fetch file so create a new one
//         memset(inf.config.theme, '\0', sizeof(inf.config.theme));
//         memset(inf.config.filename, '\0', sizeof(inf.config.filename));
//         memcpy(inf.config.theme, "GREEN", 5);
//         memcpy(inf.config.filename, "NONE404", 7);

//         int size = config_generator(newfile, &inf);
//         lfs_delete(".config");
//         lfs_create_files(".config", newfile, size);
//         memset(inf.config.theme, '\0', sizeof(inf.config.theme));
//         memset(inf.config.filename, '\0', sizeof(inf.config.filename));
//     }
//     read_config(filebuffer, &inf);
// }

// void setup_inf(){
//     // for only one time after boot
//     inf.text_pos.width_start=50;
//     inf.text_pos.height_start=20;
//     inf.scroll_val=1;
//     inf.user_pointer.screen_val=HOME_SCREEN;
//     init_config_file();
//     inf.config.col_code=get_col_code(inf.config.theme);
//     inf.user_pointer.pointingValue=0; //have to use with array
//     memcpy(inf.user_pointer.p_filename, inf.file_name[inf.user_pointer.pointingValue], sizeof(inf.user_pointer.p_filename));
//     temp=1;
//     inf.user_pointer.mutex_home=true; //put it on home_screen on startup
//  }

// // for drawing texts
// void draw_files_directory(const char *names, int x_start, int y_start, int Col, sFONT *font_size){
//     Paint_DrawString_EN(x_start, y_start, names, font_size, Col, BLACK);
// }



// void home_screen(){ 
//     //first mount filesystem
//     lfs_init(&inf);
//     hid_init();
//     //then get the files
//     get_files(&inf);
//     //extra stuff
//     setup_screen();
//     bootlogo_screen();
//     draw_menu_tray();
//     //setup data structure for first boot
//     setup_inf();
//     for(int i=0;i<inf.file_count; i++){
//         draw_files_directory(inf.file_name[i], inf.text_pos.width_start, inf.text_pos.height_start, define_color(i), &Font20);
//         inf.text_pos.height_start+=60;
//     }
//     LCD_2IN_Display((uint8_t*)ScreenBuffer);
//     SET_Infrared_PIN(JOYSTICK_ENTER);
//     SET_Infrared_PIN(JOYSTICK_UP);
//     SET_Infrared_PIN(JOYSTICK_DOWN);
//     SET_Infrared_PIN(JOYSTICK_RIGHT);
//     SET_Infrared_PIN(JOYSTICK_LEFT);
//     SET_Infrared_PIN(AUTO_RUN);
//     SET_Infrared_PIN(MORE);
//     SET_Infrared_PIN(VIEW);
//     SET_Infrared_PIN(ESC);
    
//     bool once_pressed=true;
//     bool toggle=false;
//     while(true){
//         hid_run();
//         // button interrupts has to be handled in this section of code
//         if(isPressed(DEV_Digital_Read(JOYSTICK_UP))){
//             if(once_pressed && inf.user_pointer.mutex_home){
//                 once_pressed = 1-once_pressed;
//                 joystick_handler(1);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//             }

//             else if(once_pressed && !(inf.user_pointer.mutex_home) && inf.user_pointer.pointingValue>0){
//                 once_pressed = 1-once_pressed;
//                 inf.user_pointer.pointingValue-=1;
//                 memset(inf.user_pointer.p_filename, '\0', sizeof(inf.user_pointer.p_filename));
//                 memcpy(inf.user_pointer.p_filename, option_list[inf.user_pointer.pointingValue], sizeof(option_list[inf.user_pointer.pointingValue]));
//                 inf.text_pos.width_start=50;
//                 inf.text_pos.height_start=30;
//                 for(int i=0; i<option_list_length; i++){
//                 Paint_DrawString_EN(inf.text_pos.width_start, inf.text_pos.height_start, option_list[i], &Font20, ((strcmp(inf.user_pointer.p_filename, option_list[(i)]) == 0)?inf.config.col_code:WHITE), BLACK);
//                 inf.text_pos.height_start+=50;
//                 }
//                 LCD_2IN_Display((uint8_t*)ScreenBuffer);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
    
//             }
//         }
        
//         else if(isPressed(DEV_Digital_Read(JOYSTICK_DOWN))){
//             if(once_pressed && inf.user_pointer.mutex_home){
//                 once_pressed = 1-once_pressed;
//                 joystick_handler(2);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//             }

//             else if(once_pressed && !(inf.user_pointer.mutex_home)  && inf.user_pointer.pointingValue<option_list_length-1){
//                 once_pressed = 1-once_pressed;
//                 inf.user_pointer.pointingValue+=1;
//                 memset(inf.user_pointer.p_filename, '\0', sizeof(inf.user_pointer.p_filename));
//                 memcpy(inf.user_pointer.p_filename, option_list[inf.user_pointer.pointingValue], sizeof(option_list[inf.user_pointer.pointingValue]));
//                 inf.text_pos.width_start=50;
//                 inf.text_pos.height_start=30;
//                 for(int i=0; i<option_list_length; i++){
//                     //if(strcmp(inf.user_pointer.p_filename, option_list[1])==0) {Paint_DrawString_EN(inf.text_pos.width_start, inf.text_pos.height_start, inf.config.theme, &Font20, ((strcmp(inf.user_pointer.p_filename, option_list[(inf.user_pointer.pointingValue)]) == 0)?inf.config.col_code:WHITE), BLACK);}
//                  Paint_DrawString_EN(inf.text_pos.width_start, inf.text_pos.height_start, option_list[i], &Font20, ((strcmp(inf.user_pointer.p_filename, option_list[i]) == 0)?inf.config.col_code:WHITE), BLACK);
//                 inf.text_pos.height_start+=50;
//                 }
//                 LCD_2IN_Display((uint8_t*)ScreenBuffer);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//             }
//         }

//          else if(isPressed(DEV_Digital_Read(JOYSTICK_RIGHT))){
//             if(once_pressed && !(inf.user_pointer.mutex_home) && strcmp(inf.user_pointer.p_filename, "Theme")){
//                 once_pressed = 1-once_pressed;
//                 memcpy(inf.config.theme, get_col_names(inf.config.theme, 1), 10);
//                 printf("%s\n", inf.config.theme);
//                 //Paint_DrawString_EN(inf.text_pos.width_start, inf.text_pos.height_start, get_col_names(inf.config.theme, 1), &Font20, ((strcmp(inf.user_pointer.p_filename, option_list[1]) == 0)?inf.config.col_code:WHITE), BLACK);
//                 //LCD_2IN_Display((uint8_t*)ScreenBuffer);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//         }
//         }

//         else if(isPressed(DEV_Digital_Read(AUTO_RUN))){
//             if(once_pressed && inf.user_pointer.mutex_home){
//                 once_pressed = 1-once_pressed;
//                 reset_usb_boot(0, 0);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//         }
//         }

//         if(isPressed(DEV_Digital_Read(VIEW))){
//             if(once_pressed && inf.user_pointer.mutex_home){
//                 once_pressed = 1-once_pressed;
//                 view_script_screen();
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//                 inf.user_pointer.mutex_home = false;
//                 inf.user_pointer.screen_val=HOME_SCREEN;
//             }
//         }

//         if(isPressed(DEV_Digital_Read(ESC))){
//             if(once_pressed && !(inf.user_pointer.mutex_home)){
//                 once_pressed = 1-once_pressed;
//                 inf.user_pointer.pointingValue=0;
//                 inf.scroll_val=1;
//                 free(ScreenBuffer);
//                 ScreenBuffer=NULL;
//                 redraw(true);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//                 inf.user_pointer.mutex_home = true;
//             }
//         }

//         if(isPressed(DEV_Digital_Read(JOYSTICK_ENTER))){ //run command
//             if(once_pressed && inf.user_pointer.mutex_home){
//                 inf.user_pointer.mutex_home=false;
//                 once_pressed = 1-once_pressed;
//                 int e=0;
//                 Paint_ClearWindows(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, BLUE);
//                 LCD_2IN_Display((uint8_t*)ScreenBuffer);
//                 char *f_script=lfs_read(inf.user_pointer.p_filename, &e);
//                 parsing_mechanism(f_script, e);
//                 redraw(true);
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//                 inf.user_pointer.mutex_home=true;
//             }
//         }

//         if(isPressed(DEV_Digital_Read(MORE))){
//             if(once_pressed && inf.user_pointer.mutex_home){
//                 once_pressed = 1-once_pressed;
//                 inf.scroll_val=1;
//                 inf.user_pointer.pointingValue=0;
//                 settings_screen();
//                 toggle = 1-toggle;
//                 sleep_ms(150);
//                 inf.user_pointer.mutex_home = false;
//             }
//         }

//         else if(toggle){
//             once_pressed = 1-once_pressed;
//             toggle = 1-toggle;
//         }

//         }
//     free(ScreenBuffer);
//     ScreenBuffer = NULL;
// }


// void draw_menu_tray(){
//     Paint_DrawRectangle(45, 10, SCREEN_HEIGHT, SCREEN_WIDTH, CYAN , DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//     // now draw the icons
//     //Paint_DrawImage1(usb, 10, 20, 20, 40);

//     //Paint_DrawLine(10, 35, 38, 35, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); //X_DIFFERENCE = 20, Y_DIFFERENCE = 25
//     Paint_DrawString_EN(10, 30, "AUTO", &Font12, WHITE, BLACK);

//     Paint_DrawRectangle(10, 80, 30, 105, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY); 
//     Paint_DrawString_EN(10, 90, "MORE", &Font12, WHITE, BLACK);

//     Paint_DrawRectangle(10, 140, 30, 165, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//     Paint_DrawString_EN(10, 150, "VIEW", &Font12, WHITE, BLACK);

//     Paint_DrawRectangle(10, 200, 30, 225, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//     Paint_DrawString_EN(10, 210, "ESC", &Font12, WHITE, BLACK);

// }

// void setup_screen(){
//     DEV_Module_Init();
//     DEV_SET_PWM(90);
//     LCD_2IN_Init(HORIZONTAL);
//     LCD_2IN_Clear(BLACK);
//     LCD_2IN_SetBacklight(1023);
//     if((ScreenBuffer = (UWORD*)malloc(IMAGE_SIZE))==NULL){
//         printf("Unable to reserve memory space for screen.\n");
//     }
//     Paint_NewImage((UBYTE *)ScreenBuffer,SCREEN_WIDTH,SCREEN_HEIGHT, 0, BLACK);
//     Paint_SetScale(65);
//     Paint_SetRotate(ROTATE_270);
// }

// void bootlogo_screen(){
//     Paint_Clear(BLACK);
//     Paint_DrawString_EN(10, 120, "Welcome to Ducky pie", &Font16, WHITE, BLACK);
//     LCD_2IN_Display((uint8_t*)ScreenBuffer);
//     DEV_Delay_ms(150);
//     Paint_Clear(BLACK);
// }

// // only called when user moves joystick up or down
// void redraw(bool from_start){
//     if(from_start){
//         free(ScreenBuffer);
//         ScreenBuffer=NULL;
//         setup_screen();
//         Paint_ClearWindows(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, BLACK);
//         draw_menu_tray();
//     }
//     memcpy(inf.user_pointer.p_filename, inf.file_name[inf.user_pointer.pointingValue], sizeof(inf.file_name[inf.user_pointer.pointingValue]));
    
//     inf.text_pos.height_start=20;
//     inf.text_pos.width_start=50;
//     for(int i=0;i<inf.file_count; i++){
//             draw_files_directory(inf.file_name[i], inf.text_pos.width_start, inf.text_pos.height_start, define_color(i), &Font20);
//             inf.text_pos.height_start+=60;
//     }
//     LCD_2IN_Display((uint8_t*)ScreenBuffer);

// }

// void view_script_screen(){
//     free(ScreenBuffer);
//     ScreenBuffer=NULL;
//     char *show_buffer;
//     int size=0;
//     show_buffer = lfs_read(inf.user_pointer.p_filename, &size);
//     if(show_buffer==NULL){printf("not able to read file\n");redraw(true); return;}
//     char tempo[size];
//     memcpy(tempo, show_buffer, size);
//     free(show_buffer);
//     show_buffer=NULL;
//     setup_screen();
//     Paint_ClearWindows(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, BLACK);
//     draw_menu_tray();
//     inf.text_pos.height_start=20;
//     inf.text_pos.width_start=50;
//     int c=0, s=0;
//     //Paint_DrawString_EN(inf.text_pos.width_start, inf.text_pos.height_start, show_buffer, &Font12, WHITE, BLACK);
//     for(int i=0; i<size; i++){
//         //printf("%c", show_buffer[i]);
//         if(tempo[i]=='\n' || i==size-1){
//             s=i-c;
//             char temp[s];
//             memset(temp, '\0', s+1);
//             if(s<=17){
//                 for(int k=0;k<s;k++){
//                 temp[k]=tempo[c++];
//             }
//             }
//             else{
//                 for(int k=0;k<=17;k++){
//                 temp[k]=tempo[c++];
//                 }
//             }
//             c=i+1;
//             draw_files_directory(temp, inf.text_pos.width_start,
//             inf.text_pos.height_start, WHITE, &Font16);
//             inf.text_pos.height_start+=20;
//             memset(temp, '\0', s+1);
//         }
//     }
//     LCD_2IN_Display((uint8_t*)ScreenBuffer);
// }

// // show up setting(more option) screen
// void settings_screen(){
//     free(ScreenBuffer);
//     ScreenBuffer=NULL;
//     setup_screen();
//     Paint_ClearWindows(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, BLACK);
//     Paint_DrawRectangle(45, 10, SCREEN_HEIGHT, SCREEN_WIDTH, CYAN , DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//     Paint_DrawRectangle(10, 200, 30, 225, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
//     Paint_DrawString_EN(10, 210, "ESC", &Font12, WHITE, BLACK);
//     memset(inf.user_pointer.p_filename, '\0', sizeof(inf.user_pointer.p_filename));
//     memcpy(inf.user_pointer.p_filename, option_list[inf.user_pointer.pointingValue], sizeof(option_list[inf.user_pointer.pointingValue])); //default
//     inf.text_pos.width_start=50;
//     inf.text_pos.height_start=30;
//     for(int i=0; i<option_list_length; i++){
//     Paint_DrawString_EN(inf.text_pos.width_start, inf.text_pos.height_start, option_list[i], &Font20, ((strcmp(inf.user_pointer.p_filename, option_list[(i)]) == 0)?inf.config.col_code:WHITE), BLACK);
//     inf.text_pos.height_start+=50;
//     }
//     LCD_2IN_Display((uint8_t*)ScreenBuffer);
//     inf.user_pointer.mutex_home=false;

// }

// void joystick_handler(int c){
//     //works on some cases
//     switch (c){
//         case 1: if(inf.scroll_val!=1 || inf.user_pointer.pointingValue!=0){  //user is in first page
//                     if(inf.user_pointer.pointingValue!=0){ //do not go before zero
//                         inf.user_pointer.pointingValue-=1;
//                         redraw(false);
//                     }
//                     else{
//                         inf.scroll_val-=1;
//                         get_files(&inf);
//                         inf.user_pointer.pointingValue=UI_BUFFER_SIZE-1;
//                         redraw(true);
//                     }
//                     temp-=1;
//                 }
//                 else{
//                     int no=inf.no_items;
//                     int alg=1;
//                     while(1){
//                     no-=1;
//                     if(no%4!=0)
//                     alg+=1;
//                     else break;
//                     }
//                     inf.user_pointer.pointingValue=alg-1;
//                     inf.scroll_val=(inf.no_items/4)+1;
//                     int temp_pos = (int)(inf.file_position)-3;
//                     temp_pos = (inf.no_items+temp_pos-alg-1);
//                     inf.file_position=(lfs_off_t)temp_pos;
//                     temp=inf.no_items;
//                     get_files(&inf);
//                     redraw(true);
//                 }
//                 break;

//         case 2: if(temp<inf.no_items){
//                     if(inf.user_pointer.pointingValue<UI_BUFFER_SIZE-1){
//                         inf.user_pointer.pointingValue+=1;
//                         redraw(false);
//                     }
//                     else{
//                         inf.scroll_val+=1;
//                         get_files(&inf); //get next four
//                         inf.user_pointer.pointingValue=0;
//                         redraw(true);
//                     }
//                     temp+=1;
//                 }

//                 else{ //goto the start
//                     temp=1;
//                     inf.scroll_val=1;
//                     get_files(&inf);
//                     inf.user_pointer.pointingValue=0;
//                     redraw(true);
//                 }
//                 break;
            
//     }
// }