// a dictionary map sort of mechanism

#include "tusb.h"
// void extra_keys(uint8_t code);
// uint8_t getextracodes(char *keycharacters);
typedef struct _m{
    char *letter;
    uint8_t key;
}map;

// //special keys
// typedef struct _n{
// char *letter;
// uint8_t key1;
// uint8_t key2;
// }map2;

const map keycodes[]={
    // here i can make an array of structure like a map
    {"a",HID_KEY_A},
    {"b",HID_KEY_B},
    {"c",HID_KEY_C},
    {"d",HID_KEY_D},
    {"e",HID_KEY_E},
    {"f",HID_KEY_F},
    {"g",HID_KEY_G},
    {"h",HID_KEY_H},
    {"i",HID_KEY_I},
    {"j",HID_KEY_J},
    {"k",HID_KEY_K},
    {"l",HID_KEY_L},
    {"m",HID_KEY_M},
    {"n",HID_KEY_N},
    {"o",HID_KEY_O},
    {"p",HID_KEY_P},
    {"q",HID_KEY_Q},
    {"r",HID_KEY_R},
    {"s",HID_KEY_S},
    {"t",HID_KEY_T},
    {"u",HID_KEY_U},
    {"v",HID_KEY_V},
    {"w",HID_KEY_W},
    {"x",HID_KEY_X},
    {"y",HID_KEY_Y},
    {"z",HID_KEY_Z},
    {"A",HID_KEY_A},
    {"B",HID_KEY_B},
    {"C",HID_KEY_C},
    {"D",HID_KEY_D},
    {"E",HID_KEY_E},
    {"F",HID_KEY_F},
    {"G",HID_KEY_G},
    {"H",HID_KEY_H},
    {"I",HID_KEY_I},
    {"J",HID_KEY_J},
    {"K",HID_KEY_K},
    {"L",HID_KEY_L},
    {"M",HID_KEY_M},
    {"N",HID_KEY_N},
    {"O",HID_KEY_O},
    {"P",HID_KEY_P},
    {"Q",HID_KEY_Q},
    {"R",HID_KEY_R},
    {"S",HID_KEY_S},
    {"T",HID_KEY_T},
    {"U",HID_KEY_U},
    {"V",HID_KEY_V},
    {"W",HID_KEY_W},
    {"X",HID_KEY_X},
    {"Y",HID_KEY_Y},
    {"Z",HID_KEY_Z},
    {"0",HID_KEY_0 },
    {"1",HID_KEY_1 },
    {"2",HID_KEY_2 },
    {"3",HID_KEY_3 },
    {"4",HID_KEY_4 },
    {"5",HID_KEY_5 },
    {"6",HID_KEY_6 },
    {"7",HID_KEY_7 },
    {"8",HID_KEY_8 },
    {"9",HID_KEY_9 },
    {" ", HID_KEY_SPACE},
    {"-", HID_KEY_MINUS},
    {"=", HID_KEY_EQUAL},
    {"[", HID_KEY_BRACKET_LEFT},
    {"]", HID_KEY_BRACKET_RIGHT},
    {"\\", HID_KEY_BACKSLASH},
    {";", HID_KEY_SEMICOLON},
    {"'", HID_KEY_APOSTROPHE},
    {",", HID_KEY_COMMA},
    {".", HID_KEY_PERIOD},
    {"/", HID_KEY_SLASH},
    {"/",HID_KEY_SLASH},
    {"+", HID_KEY_KEYPAD_ADD},
    {"*", HID_KEY_KEYPAD_MULTIPLY},
    {"`", HID_KEY_GRAVE},
    {"SHIFT", HID_KEY_SHIFT_LEFT},
    {"ALT", HID_KEY_ALT_RIGHT},  
    {"CTRL", HID_KEY_CONTROL_LEFT},
    {"ENTER", HID_KEY_ENTER},
    {"ESCAPE", HID_KEY_ESCAPE},
    {"BACKSPACE", HID_KEY_BACKSPACE},
    {"TAB", HID_KEY_TAB},
    {"CAPS_LOCK", HID_KEY_CAPS_LOCK},
    {"SHIFT", HID_KEY_SHIFT_LEFT},
    {"F1", HID_KEY_F1},
    {"F2", HID_KEY_F2},
    {"F3", HID_KEY_F3},
    {"F4", HID_KEY_F4},
    {"F5", HID_KEY_F5},
    {"F6", HID_KEY_F6},
    {"F7", HID_KEY_F7},
    {"F8", HID_KEY_F8},
    {"F9", HID_KEY_F9},
    {"F10", HID_KEY_F10},
    {"F11", HID_KEY_F11},
    {"F12", HID_KEY_F12},
    {"PRINT_SCREEN", HID_KEY_PRINT_SCREEN},
    {"SCROLL_LOCK", HID_KEY_SCROLL_LOCK},
    {"PAUSE", HID_KEY_PAUSE},
    {"INSERT", HID_KEY_INSERT},
    {"HOME", HID_KEY_HOME},
    {"PG_UP", HID_KEY_PAGE_UP},
    {"DEL", HID_KEY_DELETE},
    {"END", HID_KEY_END},
    {"PG_DOWN", HID_KEY_PAGE_DOWN},
    {"ARROW_RIGHT", HID_KEY_ARROW_RIGHT},
    {"ARROW_LEFT", HID_KEY_ARROW_LEFT},
    {"ARROW_DOWN", HID_KEY_ARROW_DOWN},
    {"ARROW_UP", HID_KEY_ARROW_UP},
    {"NUM_LOCK", HID_KEY_NUM_LOCK},
    {"CTRL_LEFT", HID_KEY_CONTROL_LEFT},
    {"SHIFT_LEFT", HID_KEY_SHIFT_LEFT},
    {"ALT_LEFT", HID_KEY_ALT_LEFT},
    {"OS", HID_KEY_GUI_LEFT},
    {"CTRL_RIGHT", HID_KEY_CONTROL_RIGHT},
    {"SHIFT_RIGHT", HID_KEY_SHIFT_RIGHT},
    {"ALT_RIGHT", HID_KEY_ALT_RIGHT},
    //---------------system commands(MADE FOR WINDOWS /some can work on linux)-------------
    {"POWEROFF", HID_KEY_POWER},
    {"SLEEP",HID_USAGE_DESKTOP_SYSTEM_SLEEP},
    {"MUTE", HID_KEY_MUTE}
    
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

#define length (sizeof(keycodes)/sizeof(map))
// a function that will check each character from the dictionary and return its equivalent hid command
uint8_t get_key_code(char *keycharacters){
    int i=0;
    for(i; i<length; i++){
        if(strcmp(keycharacters, keycodes[i].letter)==0)
            break;
    }
    if(i<length)
        return keycodes[i].key;
    else return 0x00;//getextracodes(keycharacters);
}

// #define length1 (sizeof(special_keys)/sizeof(map2))
// uint8_t getextracodes(char *keys){
//     int i=0;
//     for(i; i<length1; i++){
//         if(strcmp(keys, special_keys[i].letter)==0){
//             break;
//         }
//     }
//     if(i<length1){
//         //press shift already
//         extra_keys(special_keys[i].key1);

//         return special_keys[i].key2;
//     }
//     else return HID_KEY_NONE;
// }
