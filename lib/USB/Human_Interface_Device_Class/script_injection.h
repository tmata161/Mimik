#ifndef SCRIPT_INJECTION
#define SCRIPT_INJECTION

#include "tusb.h"
#include <stdlib.h>
#include <stdio.h>
#include<ctype.h>
#include <string.h>
#include "bsp/board.h"
#include "usb_descriptors.h"
#include "class/hid/hid.h"
#include "keymap.h"
#include "pico/bootrom.h"
#include"pico/stdio.h"

void script_injection(char* source);

#endif