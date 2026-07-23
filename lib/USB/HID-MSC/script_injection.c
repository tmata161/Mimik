#include "script_injection.h"

#define TYPE_DELAY 50 //fixed length delay between key presses
#define strokelength 6

void key_release();
void key_press(map kcode);
void capslock_on();
void capslock_off();
bool is_caps_on=0;
int kcode[2];

// a function that will check each character from the dictionary and return its equivalent hid command
map get_key_code(char *keycharacters){
    int i=0;
    for(i; i<key_length; i++){
        if(strcmp(keycharacters, key_codes[i].letter)==0)
            break;
    }
    if(i<key_length)return (map){"0",key_codes[i].key1, key_codes[i].key2};
    else return (map){"0",HID_KEY_NONE,HID_KEY_NONE};
}


/*Instruction code->
    0: REM
    1: PRESS
    2: TYPE
    3: DELAY
    4: REPEAT
*/
//ins_code: instruction code
//instruction: the instruction
//str_len: length of instruction
//first: flag that determines first initialization of tud_task()
void script_injection(int ins_code, char* instruction, int str_len, bool first){
    static unsigned int repeat= 1;
    tusb_init();
    int _c=1;

    //running tud stack multiple times to get it ready
    if(first){for(int i=0; i<=1000000; (tud_task(),i++));}
    //check if device is suspended; only 5 attempts for wakeup function
    while(tud_suspended()&&(_c++)<=5)
    {
        tud_remote_wakeup();
        if(_c>=5)return; //on fifth function call just return
    }
    while(repeat>0)
    {
       
    if(tud_hid_ready())
    {
        switch(ins_code){
            //press
            case 1:{
                //tokenize the keys for multiple presses
                unsigned int max_tokens=0;//maximum number of keys supported for now is 2
                char* token = strtok(instruction, " ");
                uint8_t stored_token_code[2]={HID_KEY_NONE, HID_KEY_NONE};
                while(token!=NULL && max_tokens < 2)
                {
                    //convert to upper case and remove white spaces
                    for(int i=0; i<strlen(token);\
                    (token[i]=(token[i]==' ')?'\0':toupper(token[i]),\
                    i++));
                    stored_token_code[max_tokens++] = get_key_code(token).key1;
                    token=strtok(NULL, " "); //get the next token
                }
                //key press here
                
                key_press((map){"0", stored_token_code[0], stored_token_code[1]});
                break;
            }

            //type
            case 2:
                for(int i=0; i<str_len; i++)
                {
                    // the whole caps lock logic will be applied here
                    char tmp[2];
                    if(isupper(instruction[i])) capslock_on();
                    else if(islower(instruction[i])) capslock_off();
                    tmp[0]=instruction[i];
                    tmp[1]='\0';
                    key_press(get_key_code(tmp));
                }
                break;

            //delay
            case 3:
                int num = atoi(instruction);
                board_delay(num); //for milliseconds
                break;

            case 4:
                repeat = atoi(instruction);
                return; //re
            default:
                DEBUG("Unknown instruction\n");
                break;
            }
            tud_task();
            DEBUG("code %d, instruction: %s, repeatation %d\n", ins_code, instruction, repeat);
    }
    repeat--;
}
repeat=1;
return;
}


//!--------------------------------------------------------------------+
//!                 Keystroke Injection Code
//!--------------------------------------------------------------------+


void key_press(map kcode){
    uint8_t keystrokes[strokelength]={HID_KEY_NONE};
    keystrokes[0] = kcode.key1;
    keystrokes[1] = kcode.key2;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keystrokes);
    board_delay(TYPE_DELAY);
    key_release();
    board_delay(TYPE_DELAY);
}

void key_release(){
  // report null character to host device
  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
}

//turn on the caps lock
void capslock_on(){
    if(!is_caps_on) key_press((map){"0", HID_KEY_CAPS_LOCK, HID_KEY_NONE});
    return;
}

//turn off caps lock
void capslock_off(){
    if(is_caps_on) key_press((map){"0",HID_KEY_CAPS_LOCK, HID_KEY_NONE});
}
//!--------------------------------------------------------------------+
//!                             TUD Code
//!--------------------------------------------------------------------+
// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{return;}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{   
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    // keyboard interface
  if (instance == 0 && report_type == HID_REPORT_TYPE_OUTPUT && bufsize>0)
  {
        
    if(buffer[0] & KEYBOARD_LED_CAPSLOCK)
        {
            is_caps_on=true;
        }
        else
        {
            is_caps_on=false;
        }
    
    }
    else return;
    
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en){}
// Invoked when usb bus is resumed
void tud_resume_cb(){return;}

void tud_exit(){
    tud_deinit(0);
}