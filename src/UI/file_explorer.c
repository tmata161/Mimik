// file manager page

//------------Include Section----------
#include"UI.h"
//only for development purpose
#include"pico/bootrom.h"
#include"../../lib/USB/HID-MSC/script_injection.h"
#include <ctype.h>
//------------------------------------


//-----------Function Declaration--------------
void go();
void buttonInterruptFileExplorer();  //handles all button interrupts
void handleOK(fileExplorerObject* exobj);
void handleESC(fileExplorerObject* exobj);
void fetchFiles(fileExplorerObject* exobj, char cDir[50], FF_DIR* gDIR);
void openDirectory(FF_DIR* gDIR, char* cDir);
void closeDirectory(FF_DIR* gDIR, char* cDir);
void initPage(fileExplorerObject* exobj);
void drawFiles(fileExplorerObject* exobj);
void pageMechanism(fileExplorerObject* exobj);
void handleException(fileExplorerObject* exobj, char* exception_message);
//----------------------------------------------

//------------new code-------------------



//entry section of code for every folder
fileExplorerObject file_explorer(fileExplorerObject obj){
    FF_DIR gDIR; //a global directory structure variable
    fileExplorerObject exobj; //contain entries of all objects present inside a directory
    memset(ScreenBuffer, '\0', IMAGE_SIZE);
    exobj=obj; //make a local copy of the object

    //initialize fileExplorerObject
    initPage(&exobj);

    //open directory and fetch files
    openDirectory(&gDIR, exobj.navigationLocation);
    fetchFiles(&exobj, exobj.navigationLocation, &gDIR);

    //only for testing
    // printf("Total Files in %s : %d\n",exobj.navigationLocation, exobj.total_file_count);
    // for(int i=0; i<exobj.total_file_count; i++){
    //     printf("%d\t%s\n", i, exobj.files[i].fname);
    // }printf("\n");
    //xxxxxxxxxxxxxxxx

    if(exobj.total_file_count==0){
        notification("empty folder");
        while(true){
            if(!DEV_Digital_Read(ESC)){
                handleESC(&exobj);
                break;
            }
        }
        return exobj;
    }
    pageMechanism(&exobj);
    buttonInterruptFileExplorer(&exobj);
    closeDirectory(&gDIR, exobj.navigationLocation);
    return exobj;
}

// *-------------------------------------------------------------------------Button handler---------------------------------
void buttonInterruptFileExplorer(fileExplorerObject* exobj){

    //setting up pins
    SET_Infrared_PIN(JOYSTICK_UP);
    SET_Infrared_PIN(JOYSTICK_DOWN);
    SET_Infrared_PIN(OK);
    SET_Infrared_PIN(SET);
    SET_Infrared_PIN(USB_MODE);
    SET_Infrared_PIN(ESC);

    int pageSwitch=-1; //0: Setting page; 1: Injection Page

    while(1){

        //JOYSTICK UP
        if((!DEV_Digital_Read(JOYSTICK_UP))){
            exobj->global_file_pointer[exobj->index_of_gfp]-=1;
            exobj->global_file_pointer[exobj->index_of_gfp]=(exobj->global_file_pointer[exobj->index_of_gfp]==0xFFFFFFFF)?
                                    exobj->total_file_count-1 : exobj->global_file_pointer[exobj->index_of_gfp];
            pageMechanism(exobj);
           // printf("file pointer: %u\nfile name: %s\n\n", exobj.fP, exobj.files[exobj.fP].fname);
            HALT(JOYSTICK_UP);
        }

        //JOYSTICK DOWN
        else if((!DEV_Digital_Read(JOYSTICK_DOWN))){
            exobj->global_file_pointer[exobj->index_of_gfp]+=1;
            exobj->global_file_pointer[exobj->index_of_gfp]=(exobj->global_file_pointer[exobj->index_of_gfp] >= exobj->total_file_count)? \
                                        0: \
                                        exobj->global_file_pointer[exobj->index_of_gfp];
            pageMechanism(exobj);
            //printf("file pointer: %u\nfile name: %s\n\n", exobj.fP, exobj.files[exobj.fP].fname);
            HALT(JOYSTICK_DOWN);
        }

        //OK
        else if((!DEV_Digital_Read(OK))){
            printf("ok called: %s\n", exobj->navigationLocation);
            handleOK(exobj);
            HALT(OK);
            break;
        }

        //SET
        else if((!DEV_Digital_Read(SET))){
            //abrubtly reset usb
            reset_usb_boot(0,0);
            HALT(SET);
            break;
        }

        //ESC
        else if((!DEV_Digital_Read(ESC))){
            printf("esc called: %s\n", exobj->navigationLocation);
            handleESC(exobj);
            HALT(ESC);
            break;
        }

        //NONE
        else if((!DEV_Digital_Read(USB_MODE))){
            //notification("USB mass storage mode, unmount to reboot the device");
            start_animation("USB DRIVE MODE");
            exobj->return_back=0;
            break;
        }

    //for memory card detection
        //  if(!detectCard()){
        //  notification("No memory card  detected :(");
        //  exit(1); //halt program indefinitely
        //  }
    
    }
    //page switch
    // switch(pageSwitch){
    //     case 0: entrySetting();break;
    //     case 1: notification("this functionality has not been implemented yet"); break;
    //     default:break;
    //     break;
    // }
    return;
}
//this function handles esc button event
void handleESC(fileExplorerObject* exobj){
    //move to the parent folder
    int len=strlen(exobj->navigationLocation);
    if(exobj->navigationLocation[len-1]!='/'){
         for(int i=len-1; i>=0; i--){ 
            //stop the loop if "/" is encountered
            if(exobj->navigationLocation[i]!='/'){
                exobj->navigationLocation[i]='\0';
            }
            else{
                //remove unwanted "/"
                if(i!=0)exobj->navigationLocation[i]='\0';
                break;
            }
        }
        exobj->global_file_pointer[exobj->index_of_gfp]=0;
        exobj->index_of_gfp--;
    }
    return;
}


/*Instruction code->
    0: REM
    1: PRESS
    2: TYPE
    3: DELAY
    4: REPEAT
*/
int is_instructionCode_valid(char* code){
    if(strcmp(code, "TYPE")==0 || strcmp(code, "type")==0)
        return 2;
    else if(strcmp(code, "DELAY")==0 || strcmp(code, "delay")==0)
        return 3;
    else if(strcmp(code, "PRESS")==0 || strcmp(code, "press")==0)
        return 1;
    else if(strcmp(code, "REPEAT")==0 || strcmp(code, "repeat")==0)
        return 4;
    return -1;
}

//this function handles ok button event
void handleOK(fileExplorerObject* exobj){
    //chk if current entity is file or folder
    if(exobj->total_file_count>0){
        FILINFO temp_file_info=exobj->files[exobj->global_file_pointer[exobj->index_of_gfp]];

        //if the selected item is a directory
       if(temp_file_info.fattrib == AM_DIR && temp_file_info.fname!=0)
       {
            //add a slash to the target directory
            if(strlen(exobj->navigationLocation)>1)strcat(exobj->navigationLocation, "/");
                //add new location to the directory array
                strcat(exobj->navigationLocation, temp_file_info.fname);
                exobj->index_of_gfp++;
                exobj->global_file_pointer[exobj->index_of_gfp]=0; //otherwise it gets initialize to 1 after object copy
        }

        // if selected item is a file
        else if(temp_file_info.fattrib == AM_ARC &&  temp_file_info.fname!=0)
        {
            char tempPath[50];
            memset(tempPath,'\0', 50);
            memcpy(tempPath, exobj->navigationLocation, strlen(exobj->navigationLocation));
            if(strlen(exobj->navigationLocation)>1)strcat(tempPath, "/");
            strcat(tempPath, temp_file_info.fname);

            // check if its a .mik file
            int is_valid = endsWith(temp_file_info.fname, ".mik");
            if(!is_valid){
                notification("Not a valid script file");
                DEV_Delay_ms(1500);
                return;
            }
            
            //check if it is a text file and within memory bounds
            int file_size = get_file_size(&fat, tempPath);
            printf("file size of %s is %d\n", tempPath, file_size);
            if(file_size>FILE_SIZE_LIMIT || file_size==0)
            {
                char message[55];
                snprintf(message, sizeof(message), "file is either empty or size is not within %d bytes", FILE_SIZE_LIMIT);
                notification(message);
                DEV_Delay_ms(1500);
                return;
            }
            //do the script injection thing
            else{
                //---------injection animation------
                //start_animation("INJECTING PAYLOAD");
                notification("INJECTING PAYLOAD");
                //---------------------------------
                char script_buffer[(sizeof(char)*file_size)+1];
                readFile(&fat, tempPath, file_size, script_buffer);
                script_buffer[file_size]='\n'; //add a new line character at the end of file
                parser lex;
                lex.line_number=0;
                int counter=0;
                bool is_first=true;
                //read the script letter by letter
                for(int i=0; i<file_size; i++)
                {
                    //get the instruction code (i.e. read until space or newline is found)
                    //if no instruction code is present or instruction code is unknown then search for newline character
                    if(script_buffer[i]!=' ' && script_buffer[i]!='\n' && script_buffer[i]!='\r' && counter<INSTRUCTION_LENGTH_LIMIT)
                    {
                        lex.buffer[counter++]=script_buffer[i];
                    }
                    else
                    {
                        int tmp_code;
                        if((tmp_code=is_instructionCode_valid(lex.buffer))==-1)
                        {
                            //not valid instruction shift buffer pointer to next line
                            while(script_buffer[i++]!='\n');
                        }
                        else
                        {
                            //valid instruction line
                            counter=0;
                            memset(lex.buffer,'\0', INSTRUCTION_LENGTH_LIMIT);
                            // the instruction code is valid ; copy instruction in buffer and call execution method
                            for(int j=i+1; (script_buffer[j]!='\n' && script_buffer[j-1]!='\n' && counter<INSTRUCTION_LENGTH_LIMIT-1); j++)
                            {
                                lex.buffer[counter++] = script_buffer[j];
                            }
                            lex.line_number+=1;
                            //ERROR("Line: %d keycode: %d instruction: %s instruction length: %d\n", lex.line_number,tmp_code, lex.buffer, counter);
                            script_injection(tmp_code, lex.buffer, counter, is_first);
                            is_first=false;
                        }
                    memset(lex.buffer, '\0', INSTRUCTION_LENGTH_LIMIT);
                    i-=1;
                    counter=0;
                    }
                }
                stop_animation();
                tud_exit();
            }
        }
    }
    return;
}
// !-------------------------------------------------------------------------Button handler end---------------------------------



// *-------------------------------------------------------------------------UI handler---------------------------------
// draw files on the screen
void drawFiles(fileExplorerObject* exobj){
    int height=20;
    Paint_DrawRectangle(45+5, 10+5, SCREEN_HEIGHT-5, SCREEN_WIDTH-5, BACKGROUND , DOT_PIXEL_1X1, DRAW_FILL_FULL); //draw outline rectangle
    for(int i=exobj->lower_bound; i<=exobj->upper_bound; i++){
        Paint_DrawString_EN(60, height, exobj->files[i].fname , \
            (i==exobj->global_file_pointer[exobj->index_of_gfp])?\
             &Font20 :&Font16,\
              (i==exobj->global_file_pointer[exobj->index_of_gfp])?\
              FOREGROUND_SELECTED:FORGROUND,\
               BACKGROUND);
        height+=45;
    }
    SUBMIT(ScreenBuffer);
}

//initialize file explorer object
void initPage(fileExplorerObject* exobj){
   
    exobj->lower_bound=exobj->upper_bound=exobj->total_file_count=exobj->slate_no=0;
    Paint_Clear(BACKGROUND);
    SUBMIT(ScreenBuffer);
}

//get called every time for rendering
void pageMechanism(fileExplorerObject* exobj){
    //slate calculation
    exobj->slate_no=(int)(exobj->global_file_pointer[exobj->index_of_gfp]/MAX_FILE_LIMIT);
    //upper bound and lower bound calculations
    exobj->lower_bound = exobj->slate_no * MAX_FILE_LIMIT;
    int diff = exobj->total_file_count-exobj->lower_bound;
    exobj->upper_bound = (diff>4)?\
                            exobj->lower_bound + 4:\
                            exobj->total_file_count-1;
    drawMenuTray();
    drawFiles(exobj);
}

// will get called whenever any exception occurs
void handleException(fileExplorerObject* exobj, char* exception_message){
    notification(exception_message);
        while(true){
            if(!DEV_Digital_Read(ESC)){
                handleESC(exobj);
                HALT(ESC);
                break;
            }
        }
}
// !-------------------------------------------------------------------------UI handler end-----------------------------


// *------------------------------------------------------------------------file handling logic-----------------------------
//fetch files from SD card and store it in memory
void fetchFiles(fileExplorerObject* exobj, char* navLocation, FF_DIR* gDIR){
    exobj->total_file_count=countFiles(&fat, navLocation, gDIR);
    exobj->total_file_count=(exobj->total_file_count>FILE_COPY_LIMIT)?\
                                FILE_COPY_LIMIT:\
                                exobj->total_file_count;
    if(exobj->total_file_count>0){
        copyFiles(&fat, navLocation, gDIR, exobj->files);
    }
}

void openDirectory(FF_DIR* gDIR, char* cDir){
    if(f_opendir(&fat, gDIR, cDir)!=FR_OK)DEBUG("unable to open directory");
}

void closeDirectory(FF_DIR* gDIR, char* cDir){
    if(f_closedir(gDIR)!=FR_OK)DEBUG("unable to close directory");
}
// !------------------------------------------------------------------------file handling logic end-----------------------------