#ifndef SCRIPT_INJECTION
#define SCRIPT_INJECTION

#include "tusb.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "bsp/board.h"
#include "usb_descriptors.h"
#include "bsp/board_api.h"
#include "class/hid/hid.h"
#include "pico/bootrom.h"
#include "pico/stdio.h"

void script_injection(int ins_code, char* instruction, int strrlen, bool first);
void tud_exit();

//------------------------including keymap here-----------------
// a dictionary map sort of mechanism

typedef struct _map{
    char *letter;
    uint8_t key1;
    uint8_t key2;
}map;


static map key_codes[]={
    // here i can make an array of structure like a map
    {"a",HID_KEY_A, HID_KEY_NONE},
    {"b",HID_KEY_B, HID_KEY_NONE},
    {"c",HID_KEY_C, HID_KEY_NONE},
    {"d",HID_KEY_D, HID_KEY_NONE},
    {"e",HID_KEY_E, HID_KEY_NONE},
    {"f",HID_KEY_F, HID_KEY_NONE},
    {"g",HID_KEY_G, HID_KEY_NONE},
    {"h",HID_KEY_H, HID_KEY_NONE},
    {"i",HID_KEY_I, HID_KEY_NONE},
    {"j",HID_KEY_J, HID_KEY_NONE},
    {"k",HID_KEY_K, HID_KEY_NONE},
    {"l",HID_KEY_L, HID_KEY_NONE},
    {"m",HID_KEY_M, HID_KEY_NONE},
    {"n",HID_KEY_N, HID_KEY_NONE},
    {"o",HID_KEY_O, HID_KEY_NONE},
    {"p",HID_KEY_P, HID_KEY_NONE},
    {"q",HID_KEY_Q, HID_KEY_NONE},
    {"r",HID_KEY_R, HID_KEY_NONE},
    {"s",HID_KEY_S, HID_KEY_NONE},
    {"t",HID_KEY_T, HID_KEY_NONE},
    {"u",HID_KEY_U, HID_KEY_NONE},
    {"v",HID_KEY_V, HID_KEY_NONE},
    {"w",HID_KEY_W, HID_KEY_NONE},
    {"x",HID_KEY_X, HID_KEY_NONE},
    {"y",HID_KEY_Y, HID_KEY_NONE},
    {"z",HID_KEY_Z, HID_KEY_NONE},
    {"A",HID_KEY_A, HID_KEY_NONE},
    {"B",HID_KEY_B, HID_KEY_NONE},
    {"C",HID_KEY_C, HID_KEY_NONE},
    {"D",HID_KEY_D, HID_KEY_NONE},
    {"E",HID_KEY_E, HID_KEY_NONE},
    {"F",HID_KEY_F, HID_KEY_NONE},
    {"G",HID_KEY_G, HID_KEY_NONE},
    {"H",HID_KEY_H, HID_KEY_NONE},
    {"I",HID_KEY_I, HID_KEY_NONE},
    {"J",HID_KEY_J, HID_KEY_NONE},
    {"K",HID_KEY_K, HID_KEY_NONE},
    {"L",HID_KEY_L, HID_KEY_NONE},
    {"M",HID_KEY_M, HID_KEY_NONE},
    {"N",HID_KEY_N, HID_KEY_NONE},
    {"O",HID_KEY_O, HID_KEY_NONE},
    {"P",HID_KEY_P, HID_KEY_NONE},
    {"Q",HID_KEY_Q, HID_KEY_NONE},
    {"R",HID_KEY_R, HID_KEY_NONE},
    {"S",HID_KEY_S, HID_KEY_NONE},
    {"T",HID_KEY_T, HID_KEY_NONE},
    {"U",HID_KEY_U, HID_KEY_NONE},
    {"V",HID_KEY_V, HID_KEY_NONE},
    {"W",HID_KEY_W, HID_KEY_NONE},
    {"X",HID_KEY_X, HID_KEY_NONE},
    {"Y",HID_KEY_Y, HID_KEY_NONE},
    {"Z",HID_KEY_Z, HID_KEY_NONE},
    {"0",HID_KEY_0 , HID_KEY_NONE},
    {"1",HID_KEY_1 , HID_KEY_NONE},
    {"2",HID_KEY_2 , HID_KEY_NONE},
    {"3",HID_KEY_3 , HID_KEY_NONE},
    {"4",HID_KEY_4 , HID_KEY_NONE},
    {"5",HID_KEY_5 , HID_KEY_NONE},
    {"6",HID_KEY_6 , HID_KEY_NONE},
    {"7",HID_KEY_7 , HID_KEY_NONE},
    {"8",HID_KEY_8 , HID_KEY_NONE},
    {"9",HID_KEY_9 , HID_KEY_NONE},
    {" ", HID_KEY_SPACE, HID_KEY_NONE},
    {"-", HID_KEY_MINUS, HID_KEY_NONE},
    {"=", HID_KEY_EQUAL, HID_KEY_NONE},
    {"[", HID_KEY_BRACKET_LEFT, HID_KEY_NONE},
    {"]", HID_KEY_BRACKET_RIGHT, HID_KEY_NONE},
    {"\\", HID_KEY_BACKSLASH, HID_KEY_NONE},
    {";", HID_KEY_SEMICOLON, HID_KEY_NONE},
    {"'", HID_KEY_APOSTROPHE, HID_KEY_NONE},
    {",", HID_KEY_COMMA, HID_KEY_NONE},
    {".", HID_KEY_PERIOD, HID_KEY_NONE},
    {"/", HID_KEY_SLASH, HID_KEY_NONE},
    {"/",HID_KEY_SLASH, HID_KEY_NONE},
    {"+", HID_KEY_SHIFT_LEFT, HID_KEY_EQUAL},
    {"*", HID_KEY_SHIFT_LEFT, HID_KEY_8},
    {"`", HID_KEY_GRAVE, HID_KEY_NONE},\
    {"?", HID_KEY_SHIFT_LEFT, HID_KEY_SLASH},
    {":", HID_KEY_SHIFT_LEFT, HID_KEY_SEMICOLON},
    {"{", HID_KEY_SHIFT_LEFT, HID_KEY_BRACKET_LEFT},
    {"}", HID_KEY_SHIFT_LEFT, HID_KEY_BRACKET_RIGHT},
    {"!", HID_KEY_SHIFT_LEFT, HID_KEY_1},
    {"@", HID_KEY_SHIFT_LEFT, HID_KEY_2},
    {"#", HID_KEY_SHIFT_LEFT, HID_KEY_3},
    {"$", HID_KEY_SHIFT_LEFT, HID_KEY_4},
    {"%%", HID_KEY_SHIFT_LEFT, HID_KEY_5},
    {"^", HID_KEY_SHIFT_LEFT, HID_KEY_6},
    {"&", HID_KEY_SHIFT_LEFT, HID_KEY_7},
    {"_", HID_KEY_SHIFT_LEFT, HID_KEY_MINUS},
    {"SHIFT", HID_KEY_SHIFT_LEFT, HID_KEY_NONE},
    {"ALT", HID_KEY_ALT_RIGHT, HID_KEY_NONE},  
    {"CTRL", HID_KEY_CONTROL_LEFT, HID_KEY_NONE},
    {"ENTER", HID_KEY_ENTER, HID_KEY_NONE},
    {"ESCAPE", HID_KEY_ESCAPE, HID_KEY_NONE},
    {"BACKSPACE", HID_KEY_BACKSPACE, HID_KEY_NONE},
    {"TAB", HID_KEY_TAB, HID_KEY_NONE},
    {"CAPS_LOCK", HID_KEY_CAPS_LOCK, HID_KEY_NONE},
    {"SHIFT", HID_KEY_SHIFT_LEFT, HID_KEY_NONE},
    {"F1", HID_KEY_F1, HID_KEY_NONE},
    {"F2", HID_KEY_F2, HID_KEY_NONE},
    {"F3", HID_KEY_F3, HID_KEY_NONE},
    {"F4", HID_KEY_F4, HID_KEY_NONE},
    {"F5", HID_KEY_F5, HID_KEY_NONE},
    {"F6", HID_KEY_F6, HID_KEY_NONE},
    {"F7", HID_KEY_F7, HID_KEY_NONE},
    {"F8", HID_KEY_F8, HID_KEY_NONE},
    {"F9", HID_KEY_F9, HID_KEY_NONE},
    {"F10", HID_KEY_F10, HID_KEY_NONE},
    {"F11", HID_KEY_F11, HID_KEY_NONE},
    {"F12", HID_KEY_F12, HID_KEY_NONE},
    {"PRINT_SCREEN", HID_KEY_PRINT_SCREEN, HID_KEY_NONE},
    {"SCROLL_LOCK", HID_KEY_SCROLL_LOCK, HID_KEY_NONE},
    {"PAUSE", HID_KEY_PAUSE, HID_KEY_NONE},
    {"INSERT", HID_KEY_INSERT, HID_KEY_NONE},
    {"HOME", HID_KEY_HOME, HID_KEY_NONE},
    {"PG_UP", HID_KEY_PAGE_UP, HID_KEY_NONE},
    {"DEL", HID_KEY_DELETE, HID_KEY_NONE},
    {"END", HID_KEY_END, HID_KEY_NONE},
    {"PG_DOWN", HID_KEY_PAGE_DOWN, HID_KEY_NONE},
    {"ARROW_RIGHT", HID_KEY_ARROW_RIGHT, HID_KEY_NONE},
    {"ARROW_LEFT", HID_KEY_ARROW_LEFT, HID_KEY_NONE},
    {"ARROW_DOWN", HID_KEY_ARROW_DOWN, HID_KEY_NONE},
    {"ARROW_UP", HID_KEY_ARROW_UP, HID_KEY_NONE},
    {"NUM_LOCK", HID_KEY_NUM_LOCK, HID_KEY_NONE},
    {"CTRL_LEFT", HID_KEY_CONTROL_LEFT, HID_KEY_NONE},
    {"SHIFT_LEFT", HID_KEY_SHIFT_LEFT, HID_KEY_NONE},
    {"ALT_LEFT", HID_KEY_ALT_LEFT, HID_KEY_NONE},
    {"OS", HID_KEY_GUI_LEFT, HID_KEY_NONE},
    {"CTRL_RIGHT", HID_KEY_CONTROL_RIGHT, HID_KEY_NONE},
    {"SHIFT_RIGHT", HID_KEY_SHIFT_RIGHT, HID_KEY_NONE},
    {"ALT_RIGHT", HID_KEY_ALT_RIGHT, HID_KEY_NONE},
    //---------------system commands(MADE FOR WINDOWS /some can work on linux)-------------
    {"POWEROFF", HID_KEY_POWER, HID_KEY_NONE},
    {"SLEEP",HID_USAGE_DESKTOP_SYSTEM_SLEEP, HID_KEY_NONE},
    {"MUTE", HID_KEY_MUTE, HID_KEY_NONE}
    
};

// const map2 special_keys[]={
//     {"!", HID_KEY_SHIFT_LEFT, HID_KEY_1},
//     {"@", HID_KEY_SHIFT_LEFT, HID_KEY_2},
//     {"#", HID_KEY_SHIFT_LEFT, HID_KEY_3},
//     {"$", HID_KEY_SHIFT_LEFT, HID_KEY_4},
//     {"%", HID_KEY_SHIFT_LEFT, HID_KEY_5},
//     {"^", HID_KEY_SHIFT_LEFT, HID_KEY_6},
//     {"&", HID_KEY_SHIFT_LEFT, HID_KEY_7},
//     {"*", HID_KEY_SHIFT_LEFT, HID_KEY_8},
//     {"(", HID_KEY_SHIFT_LEFT, HID_KEY_9},
//     {")", HID_KEY_SHIFT_LEFT, HID_KEY_0},

//     {"~", HID_KEY_SHIFT_LEFT, HID_KEY_GRAVE},
//     {"{", HID_KEY_SHIFT_LEFT, HID_KEY_BRACKET_LEFT},
//     {"}", HID_KEY_SHIFT_LEFT, HID_KEY_BRACKET_RIGHT},
//     {":", HID_KEY_SHIFT_LEFT, HID_KEY_SEMICOLON},
//     {"\"", HID_KEY_SHIFT_LEFT, HID_KEY_APOSTROPHE},
//     {"|", HID_KEY_SHIFT_LEFT, HID_KEY_BACKSLASH},
//     {"<", HID_KEY_SHIFT_LEFT, HID_KEY_COMMA},
//     {">", HID_KEY_SHIFT_LEFT, HID_KEY_PERIOD},
//     {"?", HID_KEY_SHIFT_LEFT, HID_KEY_SLASH},
//     {"_", HID_KEY_SHIFT_LEFT, HID_KEY_MINUS}
// };

#define key_length (sizeof(key_codes)/sizeof(map))
map get_key_code(char *keycharacters);

#endif