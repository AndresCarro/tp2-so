// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard_driver.h>
#include <pipe.h>
#include <lib.h>

// Retreived from https://www.millisecond.com/support/docs/current/html/language/scancodes.htm
static char default_keys[] = {
                        0, // Error
                        27, // Escape
                        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
                        8, // Backspace
                        '\t', // Tab
                        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
                        '\n', // Enter
                        0, // LControl
                        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
                        0, // LShift
                        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
                        0, // RShift
                        0, // Print Screen
                        0, // LAlt
                        ' ',
                        0, // CapsLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
                        0, // NumLock
                        0, // ScrollLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // Numpad keys
                     };

static char caps_keys[] = {
                        0, // Error
                        27, // Escape
                        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
                        8, // Backspace
                        '\t', // Tab
                        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
                        '\n', // Enter
                        0, // LControl
                        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
                        0, // LShift
                        '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',
                        0, // RShift
                        0, // Print Screen
                        0, // LAlt
                        ' ',
                        0, // CapsLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
                        0, // NumLock
                        0, // ScrollLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // Numpad keys
                     };

static char shift_keys[] = {
                        0, // Error
                        27, // Escape
                        '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
                        8, // Backspace
                        '\t', // Tab
                        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
                        '\n', // Enter
                        0, // LControl
                        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
                        0, // LShift
                        '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
                        0, // RShift
                        0, // Print Screen
                        0, // LAlt
                        ' ',
                        0, // CapsLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
                        0, // NumLock
                        0, // ScrollLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // Numpad keys
                     };


static char shift_caps_keys[] = {
                        0, // Error
                        27, // Escape
                        '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
                        8, // Backspace
                        '\t', // Tab
                        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}',
                        '\n', // Enter
                        0, // LControl
                        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~',
                        0, // LShift
                        '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?',
                        0, // RShift
                        0, // Print Screen
                        0, // LAlt
                        ' ',
                        0, // CapsLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F1-F10
                        0, // NumLock
                        0, // ScrollLock
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // Numpad keys
                     };

static Pipe * keyboard_pipe = NULL;
static int shift = 0, caps_lock = 0, control = 0, alt = 0;

static void add(char key);
static char translate(uint16_t key);
static uint8_t pressed(uint16_t scancode, uint16_t key);

Pipe * keyboard_init() {
    keyboard_pipe = pipe_open();
    return keyboard_pipe;
}

void keyboard_handler() {
    if (!(read_port(0x64) & 0x01))
        return;

    uint16_t scancode = read_port(0x60);
    uint16_t key = scancode & 0x7F;

    if (pressed(scancode, key)) {
        if (control && key == 32) { // Ctrl + D
            add(-1);
        } else {
            add(translate(key));
        }
    }
}

static uint8_t pressed(uint16_t scancode, uint16_t key) {
    if ((scancode & 0x80) == 0x80) {
        switch (key) {
            case 0x1d:
                control = 0;
                break;
            case 0x2a:
            case 0x36:
                shift = 0;
                break;
            case 0x38:
                alt = 0;
                break;
            default:
                break;
        }
    } else {
        switch (key) {
            case 0x1d:
                control = 1;
                break;
            case 0x3a:
                caps_lock = !caps_lock;
                break;
            case 0x2a:
            case 0x36:
                shift = 1;
                break;
            case 0x38:
                alt = 1;
                break;
            default:
                return 1;
        }
    }
    return 0;
}

static char translate(uint16_t key) {
    if (caps_lock && !shift) {
        return caps_keys[key];
    } else if (caps_lock && shift) {
        return shift_caps_keys[key];
    } else if (!caps_lock && !shift) {
        return default_keys[key];
    } else {
        return shift_keys[key];
    }
}

static void add(char key) {
    char str[1] = {key};
    pipe_write(keyboard_pipe, str, 1);
}