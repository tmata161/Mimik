#include "script_injection.h"

#define TYPE_DELAY 25
#define strokelength 6
#define INVALID_TOKEN -1

void start_injection();
void key_release();
void key_press(uint8_t key);



// regexes
char *keywords_string[]={"REM", "DELAY", "REPEAT", "STRING", "KEY", "COMBOKEY", NULL};
char *seperators_string[]={"(", ")", "[", "]", NULL}; 
char *operators_string[]={"+", "-", "*", "/", NULL}; //mathematical expression are only allowed for delay and repeat commands

typedef enum _tokenKinds{
    KEYWORD,
    ATOM, //non keywords
    OPERATOR,
    SEPERATOR,
    END_OF_FILE,
}TokenKinds;

typedef struct _tokenstream{
    char *word;
    TokenKinds tokenKind;
}Tokens;

typedef struct _lexer{
    Tokens *tokens; //stores a list of tokens
    int totalTokens;
}Lexer;

static Lexer mylexer; 

void tokenizer(char* source, int tokenType);
void addENDtoken(Lexer* lex);
int identifyRegex(char* source, char* (patternlist[]));
void detokenizer(Lexer* ptr, int no_of_tok);
void stringSplitter(char* source, int size);
void exp_evaluator();



void script_injection(char* source){
    stdio_init_all();
    board_init(); // initializes usb and other peripherals for communcation
    tud_init(0);
    board_delay(2000);
    // this function will split the string and tokenize
    stringSplitter(source, strlen(source));

    // tinyusb will run as device mode
    tud_task();
    //start key injection
    start_injection();
    //free the memory
    detokenizer(&mylexer, mylexer.totalTokens);
    printf("code ended successfully\n");
}


//!--------------------------------------------------------------------+
//!                 Keystroke Injection Code
//!--------------------------------------------------------------------+


void start_injection()
{
  // check if device is suspended
  if(!tud_suspended()){
    if(tud_hid_ready()){
        exp_evaluator(); //an expression evaluator
  }
}
else tud_remote_wakeup();
}
void key_press(uint8_t key){ //index is the index of myLexer object
    uint8_t keystrokes[strokelength]={0};
    keystrokes[0] = key;
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keystrokes);
    board_delay(TYPE_DELAY);
    key_release();
}

void key_release(){
  // report null character to host device
  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
}

//!--------------------------------------------------------------------+
//!                             Tokenization code
//!--------------------------------------------------------------------+

int tok_at(int i){return mylexer.tokens[i].tokenKind;}
char* tok_str_at(int i){return mylexer.tokens[i].word;}
bool is_tok_end(int i){return mylexer.tokens[i].tokenKind==END_OF_FILE;}
void space_key(){key_press(0x2c);}

void exp_evaluator(){

    printf("----------------injection started----------------\n");
    char tempBuff[2]={'\0'};
    //go through all the tokens
    for(int i=0; mylexer.tokens[i].tokenKind != END_OF_FILE; i++){
        if(tok_at(i)==KEYWORD){
            if (strcmp(tok_str_at(i), "REM")==0){ printf("REM\n");}
            if (strcmp(tok_str_at(i), "DELAY")==0){
                i+=1; //skip
                int num=atoi(tok_str_at(i)); //convert string number into integer
                printf("DELAY: delaying for %d sec\n", num);
                board_delay(num*1000);
            }
            if (strcmp(tok_str_at(i), "KEY")==0){
                i+=1; //skip i to the next token
                printf("KEY: keycode of %s is %x\n", tok_str_at(i), get_key_code(tok_str_at(i)));
               key_press(get_key_code(tok_str_at(i)));
            }
            if (strcmp(tok_str_at(i), "STRING")==0){
                i+=1; //skip i to the next token
                // read character of the following atom one by one and get the keycode
                while(tok_at(i)==ATOM){
                for(int j=0; j<strlen(tok_str_at(i)); j++){
                    tempBuff[0]=tok_str_at(i)[j];
                    tempBuff[1]='\0';
                    printf("STRING: keycode of %c is %x\n", tok_str_at(i)[j], get_key_code(tempBuff));
                    key_press(get_key_code(tempBuff));
                    board_delay(TYPE_DELAY);
                }
                i++;
                space_key();
                board_delay(100);
            }
            i-=1;
            }
        }
        
    }
}

void tokenizer(char* source, int tokenType){ 
    if (mylexer.tokens==NULL){
        // allocate new memory block to store tokens
        mylexer.tokens = (Tokens*) malloc(sizeof(Tokens));
    }
    else{
        // if the memory block was already allocated then increase
        // the block size to store more tokens
        mylexer.tokens = (Tokens*) realloc(mylexer.tokens, sizeof(Tokens)*(mylexer.totalTokens+1));
    }

    //allocate mem for char
    mylexer.tokens[mylexer.totalTokens].word = (char*) malloc(strlen(source)+1);
    strcpy(mylexer.tokens[mylexer.totalTokens].word, source);
    mylexer.tokens[mylexer.totalTokens].tokenKind = tokenType;
    mylexer.totalTokens++;
}

//adds end of file token at end
void addENDtoken(Lexer* lex){
 //add end of line token
    lex->tokens=(Tokens*) realloc(lex->tokens, sizeof(Tokens)*(lex->totalTokens+1));
    lex->tokens[lex->totalTokens].tokenKind=END_OF_FILE;
}

// identify expressing as per the pattern list
int identifyRegex(char* source, char* (patternlist[])){
    for(int i=0; patternlist[i]!=NULL; i++){
        if(strcmp(source, patternlist[i]) == 0){
            if(strcmp(source, "REM")==0) return KEYWORD;//return REM;
            else if(strcmp(source, "DELAY")==0) return KEYWORD;//return DELAY;
            else if(strcmp(source, "REPEAT")==0) return KEYWORD;//return REPEAT;
            else if(strcmp(source, "STRING")==0) return KEYWORD;//return STRING;
            else if(strcmp(source, "KEY")==0) return KEYWORD;//return KEY;
            else if(strcmp(source, "COMBOKEY")==0) return KEYWORD;//return COMBOKEY;
            else if(strcmp(source, "(")==0) return SEPERATOR;//OPEN_PARENTHESIS;
            else if(strcmp(source, ")")==0) return SEPERATOR;//CLOSED_PARENTHESIS;
            else if(strcmp(source, "[")==0) return SEPERATOR;//OPEN_SQUARE_BRACKETS;
            else if(strcmp(source, "]")==0) return SEPERATOR;//CLOSED_SQUARE_BRACKETS;
            else if(strcmp(source, "+")==0) return OPERATOR; //PLUS;
            else if(strcmp(source, "-")==0) return OPERATOR;//MINUS;
            else if(strcmp(source, "*")==0) return OPERATOR;//MULTIPLY;
            else if(strcmp(source, "/")==0) return OPERATOR;//DIVIDE;
        }
    }
    return INVALID_TOKEN;
}

//this function is for freeing up pointers
void detokenizer(Lexer* ptr, int no_of_tok){
    for(int i=0; i<no_of_tok; i++){
        free(ptr->tokens[i].word);
        ptr->tokens[i].word=NULL;
    }
    free(ptr->tokens);
    printf("successfully freed all memory\n");
}

void stringSplitter(char* source, int size){
    //seperate string on the basis of white spaces
    char* seperatedString = strtok(source, " ");
    char buffer[10]={'\0'};
    while(seperatedString !=NULL){
        //seperate string on the basis of non alphanumeric characters
        if(seperatedString != NULL){
            //go through each character
            for(int i=0, s=0; i<strlen(seperatedString); i++, s++){
                // there are only two condition: either a word is pure alphanumeric or mixed
                if(isalnum(seperatedString[i])){
                    buffer[s] = seperatedString[i];
                }
                else{
                    // there is a non alphanumeric character
                    //empty the buffer if not empty
                    if(buffer[0] != '\0'){
                        tokenizer(buffer, (KEYWORD == identifyRegex(buffer, keywords_string))?KEYWORD:ATOM);
                        memset(buffer, '\0', sizeof(buffer)/sizeof(char));
                    }
                    //check if the non alpha numeric character is of which kind
                    char buff[1] = {seperatedString[i]};
                    if(SEPERATOR == identifyRegex(buff, seperators_string)) tokenizer(buff, SEPERATOR);
                    else if(OPERATOR == identifyRegex(buff, operators_string)) tokenizer(buff, OPERATOR);
                    else tokenizer(buff, INVALID_TOKEN);
                    buff[0]='\0';
                    s=-1;
                }
            }
            //check if the stored buffer is a keyword or atom
            if(buffer[0] != '\0'){
                tokenizer(buffer, (KEYWORD == identifyRegex(buffer, keywords_string))?KEYWORD:ATOM);
                memset(buffer, '\0', sizeof(buffer)/sizeof(char));
            }
        }
        seperatedString = strtok(NULL, "\n ");
    }
    addENDtoken(&mylexer);
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
{   printf("tud_hid_get_report_cb\n");
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{}
//lets check callback function
void tud_mount_cb(void){}
// Invoked when device is unmounted
void tud_umount_cb(void){}
// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en){}
// Invoked when usb bus is resumed
void tud_resume_cb(){return;}