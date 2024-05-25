#include "tusb.h"
#include "bsp/board.h"
#include "usb_descriptors.h"
#include <stdlib.h>
#include "class/hid/hid.h"
#include "keymap.h"
//#include "../bootd"


#define TYPE_DELAY 10
// DEFAULT 261 //ms
#define USB 21
#define DEBUG_ON 1
#define strokelength 6
#define ASCII_START 48
void parsing_mechanism(char *script, int script_size);
void run();
void release();
void toggle_capslock();
void op_press(int l_no);
void op_write(int l_no);

typedef struct state{
bool is_caps_on;
}sys_state;

uint8_t keystrokes[strokelength]={0};

typedef struct stc{
    char *opcode;
    char *instruction;
    int size;
}parser;
sys_state system_state;
parser *parse;

//--------------------------------------------------------------------+
// USB device
//--------------------------------------------------------------------+

void hid_init(){
    board_init();
    tusb_init();
}
void hid_run()
{
    tud_task();
  // check if device is suspended
  if(!tud_suspended()){
    tud_hid_ready();
  }
  else tud_remote_wakeup();
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{return;}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{   printf("tud_hid_get_report_cb\n");
    return 0;}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    if(report_type==HID_REPORT_TYPE_OUTPUT && report_id==REPORT_ID_KEYBOARD){
        if(buffer[0]==3) system_state.is_caps_on=true;
        else if(buffer[0]==1) system_state.is_caps_on=false;
    }
    //KEYBOARD_LED_CAPSLOCK
}
//lets check callback function
void tud_mount_cb(void)
{}
// Invoked when device is unmounted
void tud_umount_cb(void)
{}
// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{}
// Invoked when usb bus is resumed
void tud_resume_cb(void)
{}

void release(){
  // report null character to host device
  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
}


void parsing_mechanism(char *demo_script, int script_size){
    int e=0;
    bool is_opcode=true;
    int script_length=0;
    
    parse = (parser*)malloc(script_size);
    memset(parse, '\0', script_size);
    
    for(int i=0; i<script_size; i++){
        if(demo_script[i]==' ' && i==e)e+=1; //safety mechanism for unwanted spaces
        else if(demo_script[i]=='\n' && i==e)e+=1; //safety mechanism for unwanted next line
        
        else if(demo_script[i]==' ' && is_opcode){
            is_opcode=false;
            parse[script_length].opcode = (char *)malloc(i-e);
            memset(parse[script_length].opcode, '\0', 10);
            int temp_counter=0;
            for(int j=e; j<i; j++){
                parse[script_length].opcode[temp_counter++]=demo_script[j];
            }
            e=i+1;
        }
        
        else if((demo_script[i]=='\n' || i==script_size-1)){
            parse[script_length].instruction=(char*)malloc(i-e+5);
            memset(parse[script_length].instruction, '\0', i-e);
            int temp_counter=0;
            for(int j=e; j<i; j++){
                parse[script_length].instruction[temp_counter++]=demo_script[j];
            }
            parse[script_length].size=temp_counter;
            
            if(demo_script[(int)(script_size)-2] == '\n'){ // a safety mechanism if there is \n at last of file
                demo_script[(int)(script_size)-2]='\0';
            }
            e=i+1;
            is_opcode=true;
            script_length+=1;
        }
    }
   // if(demo_script[(int) (sizeof(demo_script)-2)] == '\n') script_length-=1;
    printf("script length-> %d\nlength of demo script-> %lu\n", script_length, script_size);
    int repeat_count=0;

    for(int i=0; i<script_length;i=(repeat_count==0)?i+1:i){
        board_delay(300);
    
        if(DEBUG_ON)printf("opcode: %s\tinstruction: %s\tinstruction length-> %d script length %d val of i-> %d\n", parse[i].opcode, parse[i].instruction, parse[i].size, script_length, i);
         if(strcmp(parse[i].opcode, "PRESS") == 0 || strcmp(parse[i].opcode, "press") == 0){
             op_press(i);
         }

        else if(strcmp(parse[i].opcode, "DELAY") == 0 || strcmp(parse[i].opcode, "delay") == 0){
            // Delay should be in ms
            int dec_place=1, value=0;
            for(int z=parse[i].size-1; z>=0; z--){
                value=value+(((int)parse[i].instruction[z]-ASCII_START)*dec_place);
                dec_place*=10;
            }
            board_delay(dec_place);
         }

        else if(strcmp(parse[i].opcode, "WRITE") == 0 || strcmp(parse[i].opcode, "write") == 0){
             op_write(i);    
         }

        else if(strcmp(parse[i].opcode, "CAPS") == 0 || strcmp(parse[i].opcode, "caps") == 0){ //to turn on capslock
             if(parse[i].instruction[1]=='F'){ //then turn off capslock
                if((system_state.is_caps_on)) toggle_capslock();
             }
             else if(parse[i].instruction[1]=='N'){
                if(!(system_state.is_caps_on)) toggle_capslock();
             }
         }


        else if(strcmp(parse[i].opcode, "REPEAT") == 0 || strcmp(parse[i].opcode, "repeat") == 0){
            // REPEAT next line n times
            int dec_place=1, value=0;
            for(int z=parse[i].size-1; z>=0; z--){
                value=value+(((int)parse[i].instruction[z]-ASCII_START)*dec_place);
                dec_place*=10;
            }
            //delete current line
                free(parse[i].opcode);
                parse[i].opcode=NULL;
                free(parse[i].instruction);
                parse[i].instruction=NULL;
                repeat_count=value+1;
                i+=1; //goto nextline

        }
        else if(strcmp(parse[i].opcode, "REM") == 0 || strcmp(parse[i].opcode, "rem") == 0) continue;
        
        else continue;
    

        if(repeat_count>0){
            repeat_count-=1;
            printf("repeating , count %d, line-> %d\n", repeat_count, i);
        }

        else{
            repeat_count=0;
            free(parse[i].opcode);
            parse[i].opcode=NULL;
            free(parse[i].instruction);
            parse[i].instruction=NULL;
            }
      
    }

    free(parse);
    parse=NULL;

}

void extra_keys(uint8_t code){
    keystrokes[1]=code;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keystrokes);
    board_delay(TYPE_DELAY);
}

void op_press(int l_no){
    // get number of words
 
    int e=0, k=0;
    char *temp;

    for(int i=0; i<parse[l_no].size; i++){  // run once per each line
            if(k>=strokelength){ //6 keystrokes are alredy complete , hence submit here
                    if(DEBUG_ON)printf("Max 6 keys allowed at once\n");
                }
            for(int z=0; z<strokelength; z++){keystrokes[i]=0;}
            if(parse[l_no].instruction[i] == ' ' && i==e) e+=1;
            else if(parse[l_no].instruction[i] == ' '){ //have encountered a space
            char temp[i-e];
            memset(temp, '\0', (i-e)+1);
            int temp_counter=0;
            for(int j=e; j<i; j++){
                temp[temp_counter++] = parse[l_no].instruction[j];
            }
            if(DEBUG_ON)printf("%s 0x0%x\n", temp, getcodes(temp));
            keystrokes[k++] = getcodes(temp);
            e=i+1;
        }
        else if(i == (parse[l_no].size-1) ) //have reached to the end of line
        {
            int temp_counter=0, ins_size=i-e+1;
            char temp[ins_size];
            memset(temp, '\0', ins_size+1);
            for(int j=e; j<=i; j++){
                temp[temp_counter++] = parse[l_no].instruction[j];
            }
            if(DEBUG_ON)printf("%s 0x0%x\n", temp, getcodes(temp));
            keystrokes[k++]=getcodes(temp);
            e=0;
        }
    }
    if(DEBUG_ON)printf("\n");
    if(k<strokelength){
        //---------sumbit here---------
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keystrokes);
        board_delay(TYPE_DELAY);
        release();
    }
// submit a null stroke
release();

}

void op_write(int l_no){
int ins_len=0;
bool reverse_back=false;
int s= parse[l_no].size;
//empty everything
while(ins_len < s){
    //convert every word to uppercase
    if(parse[l_no].instruction[ins_len]>=97 && parse[l_no].instruction[ins_len]<=122) {
        // letters are small
        if((system_state.is_caps_on)) toggle_capslock();
        parse[l_no].instruction[ins_len]-=32;
        reverse_back=true;
    }
    else{
        //letters are capital
        if(!(system_state.is_caps_on)) toggle_capslock(); //if caps lock not on then turn it on
    }
    for(int z=0; z<strokelength; z++){
        keystrokes[z]=0x0;
    }
    char temp[2];
    temp[0] = parse[l_no].instruction[ins_len];
    temp[1]='\0';

    keystrokes[0]=getcodes(temp);
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keystrokes);
    release();
    board_delay(TYPE_DELAY);
    if(DEBUG_ON)printf("0x%x ", keystrokes[0]);
    keystrokes[0]=HID_KEY_NONE;
    keystrokes[1]=HID_KEY_NONE;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keystrokes);
    release();
    board_delay(TYPE_DELAY);
    
    if(reverse_back) parse[l_no].instruction[ins_len]+=32;
    ins_len+=1;
}
if(DEBUG_ON)printf("\n");
release();

}

void toggle_capslock(){
    for(int z=0; z<strokelength; z++){
        keystrokes[z]=0x0;
    }
    keystrokes[0]=HID_KEY_CAPS_LOCK;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keystrokes);
    release();
    board_delay(TYPE_DELAY);
}