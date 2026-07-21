# Project Mimik
Hello internet this is my first embedded project.
Mimik is a simple script injection device that acts as a Human Interface Device(HID) when plugged in to a computer.

# Build guide
cmake -B build

# Run guide
make -j

# List of commands (for now)
PRESS: presses max 2 keys at a time
TYPE: for typing
DELAY: delay for (ms)

[All the commands are not case sensitive]

# List of Keys supported
**Character keys**
    a-z
    A-Z
    0-9
    F1-F12
    Other character keys

**Modifier keys**
    SHIFT
    ALT
    CTRL    
    ENTER
    ESCAPE
    BACKSPACE
    TAB
    CAPS_LOCK
    SHIFT
    PRINT_SCREEN
    SCROLL_LOCK
    PAUSE
    INSERT
    HOME
    PG_UP
    DEL
    END 
    PG_DOWN
    ARROW_RIGHT
    ARROW_LEFT
    ARROW_DOWN
    ARROW_UP
    NUM_LOCK
    CTRL_LEFT
    SHIFT_LEFT
    ALT_LEFT
    OS
    CTRL_RIGHT
    SHIFT_RIGHT
    ALT_RIGHT

**Special keys**
    POWEROFF
    SLEEP
    MUTE   