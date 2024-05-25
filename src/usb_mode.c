#include"../bootducky.h"
#include"../lib/Storage/storage.h"

void interrupt_reset_request();

//for test purpose only
#define Y 21

UWORD *sc_buf;

void interrupt_reset_request(){
    if(!gpio_get(Y)){
        tud_unmount_cb();
        reset_usb_boot(0,0);
    }
}

void USBMODE(){

    stdio_init_all();
    board_init();
    storage_init();

    gpio_init(Y);
    gpio_pull_up(Y);
    gpio_set_dir(Y, GPIO_IN);

    //DEV_Module_Init();
    //DEV_SET_PWM(50);
    LCD_2IN_Init(HORIZONTAL);
    LCD_2IN_Clear(BLACK);
    LCD_2IN_SetBacklight(1023);
    sc_buf = (UWORD*)malloc(IMAGE_SIZE);
    if(sc_buf==NULL){
        printf("Unable to allocate memeory for display buffer therefore stoping execution\n");
        return;
    }
    Paint_NewImage((UBYTE *)sc_buf, SCREEN_WIDTH, SCREEN_HEIGHT, 0, BLACK);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_270);

    Paint_Clear(BLACK);
    // will show usb image afterwards
    Paint_DrawString_EN(50, 120, "Running in USB MSC  MODE", &Font16, WHITE, BLACK);
    //LCD_2IN_Display((uint8_t *) sc_buf);

 
    while(1){
        // run tinyusb on msc mode and expose filesystem
        interrupt_reset_request();
        storage_loop();
    }
}

