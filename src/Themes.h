//#include"bootducky.h"
#include<string.h>

struct col_name{
    int code;
    char name[10];
};

 struct col_name interface_colour[]={
    {0xFFFF, "WHITE"},
    {0x0000, "BLACK"},
    {0x001F, "BLUE"},
    {0XF81F, "BRED"},
    {0XFFE0, "GRED"},
    {0X07FF, "GBLUE"},
    {0xF800, "RED"},
    {0xF81F, "MAGNETA"},
    {0x07E0, "GREEN"},
    {0x7FFF, "CYAN"},
    {0xFFE0, "YELLOW"},
    {0XBC40, "BROWN"},
    {0XFC07, "BRRED"},
    {0X8430, "GRAY"}
};
#define Color_LIST_SIZE (sizeof(interface_colour)/sizeof(struct col_name))


// file name is stored i want color code
int get_col_code(char *name){
    int i;
    for(i=0; i<Color_LIST_SIZE; i++){
        if(strcmp(name, interface_colour[i].name)==0)
            break;
    }
    return interface_colour[i].code;
}

// pass 1 to go down the list and 0 to go up
char *get_col_names(char *_n, int next){
    int i=0;
    // get particular position of color name from the list object
    for(i=0;i<Color_LIST_SIZE; i++){
        if(strcmp(_n, interface_colour[i].name)==0){
            break;
        }
    }
    if(next && i<Color_LIST_SIZE){
        return interface_colour[i+1].name;
    }
    else if (next && i>=Color_LIST_SIZE){
        return interface_colour[0].name;
    }
    else if (!next && i>0){
        return interface_colour[i-1].name;
    }
    else if(!next && i<=0){
        return interface_colour[Color_LIST_SIZE-1].name;
    }
}