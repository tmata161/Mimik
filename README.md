# Project Mimik
Hello internet this is my first embedded project.

Mimik is a simple script injection device that acts as a Human Interface Device(HID) when plugged in to a computer.

# Build guide [Testesd only on Linux]

Step 1: Clone the [pico-sdk](https://github.com/raspberrypi/pico-sdk.git)\
Step 2: Install CMake (at least version 3.13), python 3, a native compiler, and a GCC cross compiler\
&emsp;&emsp; ```sudo apt install cmake python3 build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib```\
Step 3: Set **PICO_SDK_PATH** to the SDK location in your environment.\
Step 4: Create CMake **build/** directory\
&emsp;&emsp; ```mkdir build```\
Step 5: Run the cmake command\
&emsp;&emsp; ```cmake -B build```\
Step 6: Go the the build directory and build the binaries\
&emsp;&emsp; ```cd build && make```\
Step 7: A new file with name **mimik.uf2** will be generated.\
Step 8: Now boot into **USB Mass Storage Mode** while holding the **BOOTSEL** button while plugging in Raspberry Pi Pico.
Step 9: A new USB flash drive named **RPI-RP2** will appear. Copy the **mimik.uf2** file and paste it in this drive. 

# List of commands supported (for now)
PRESS: presses max 2 keys at a time\
TYPE: for typing\
DELAY: delay for (ms)\
REPEAT n: repeat the next following line for (n) times

[All commands keys are not case sensitive]

# List of Keys supported
**Character keys**\
&emsp;    a-z\
&emsp;    A-Z\
&emsp;    0-9\
&emsp;    F1-F12\
&emsp;    Other character keys

**Modifier keys**\
&emsp;    SHIFT\
&emsp;    ALT\
&emsp;    CTRL    \
&emsp;    ENTER\
&emsp;    ESCAPE\
&emsp;    BACKSPACE\
&emsp;    TAB\
&emsp;    CAPS_LOCK\
&emsp;    SHIFT\
&emsp;    PRINT_SCREEN\
&emsp;    SCROLL_LOCK\
&emsp;    PAUSE\
&emsp;    INSERT\
&emsp;    HOME\
&emsp;    PG_UP\
&emsp;    DEL\
&emsp;    END \
&emsp;    PG_DOWN\
&emsp;    ARROW_RIGHT\
&emsp;    ARROW_LEFT\
&emsp;    ARROW_DOWN\
&emsp;    ARROW_UP\
&emsp;    NUM_LOCK\
&emsp;    CTRL_LEFT\
&emsp;    SHIFT_LEFT\
&emsp;    ALT_LEFT\
&emsp;    OS\
&emsp;    CTRL_RIGHT\
&emsp;    SHIFT_RIGHT\
&emsp;    ALT_RIGHT

**Special keys**\
&emsp;    POWEROFF\
&emsp;    SLEEP\
&emsp;    MUTE\

# Hardwares Used

# Circuit Diagram