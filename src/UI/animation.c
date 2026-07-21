// here i will handle the injection animation
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/regs/addressmap.h"
#include "UI.h"
#define PICO_USE_STACK_GUARDS 1

volatile int _r;
void update_animation(){
    char* message = (char*) multicore_fifo_pop_blocking() ;
    int radius=10;
    int pos_x=60;
    int pos_y=160;
    int count=1;
    //while(1)
    while (_r!=123)
    {
        //check for clipping
        if(count>=7){pos_x=60;pos_y=160;count=1;}
        Paint_Clear(BLACK);
        Paint_DrawString_EN(30, 100, message, &Font16, WHITE, BLACK);
        Paint_DrawRectangle(50, 140, 180, 180, WHITE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
        Paint_DrawCircle(pos_x, pos_y, radius, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        //update the positions
        pos_x = pos_x+(2*radius);
        count+=1;
        board_delay(200);
        SUBMIT(ScreenBuffer);
    }
    printf("core 1 exit\n");
    return;
}

//api function to start animation while injecting script
void start_animation(char* message){
    _r=61;
    // turn on core one; using a custom stack location for core 1
    multicore_launch_core1_with_stack(update_animation, (uint32_t*)0x20026000, 128);
    //multicore_launch_core1(update_animation);
    multicore_fifo_push_blocking((uint32_t) message);
    
}

//api function to stop animation
void stop_animation(){
    _r=123;
}