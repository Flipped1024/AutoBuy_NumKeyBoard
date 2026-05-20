#include "app_keyboard.h"
#include "bsp_key.h"
#include "app_macro.h"

#define MACRO_STD_DELAY 25

typedef enum {
    MODE_NORMAL_KEYBOARD,
    MODE_MACRO_KEYBOARD
} KeyboardMode;

static KeyboardMode current_mode_ = MODE_MACRO_KEYBOARD; 

// {键值, X, Y, 左键, 右键, 帧后延时}
static const MacroFrame MACRO_SEQUENCE_50[] = {
    {79, 0,    0,   0, 0, MACRO_STD_DELAY},
    {0,  1160, 560, 0, 0, MACRO_STD_DELAY},
    {0,  1160, 560, 1, 0, MACRO_STD_DELAY},
    {0,  1160, 560, 0, 0, MACRO_STD_DELAY},
    {0,  960,  670, 0, 0, MACRO_STD_DELAY},
    {0,  960,  670, 1, 0, MACRO_STD_DELAY},
    {0,  960,  670, 0, 0, MACRO_STD_DELAY},
    {0,  860,  560, 0, 0, MACRO_STD_DELAY},
    {0,  860,  560, 1, 0, MACRO_STD_DELAY},
    {0,  860,  560, 0, 0, MACRO_STD_DELAY},
    {79, 0,    0,   0, 0, MACRO_STD_DELAY},
    {0,  0,    0,   0, 0, 0}
};

static const MacroFrame MACRO_SEQUENCE_100[] = {
    {79, 0,    0,   0, 0, MACRO_STD_DELAY},
    {0,  1210, 560, 0, 0, MACRO_STD_DELAY},
    {0,  1210, 560, 1, 0, MACRO_STD_DELAY},
    {0,  1210, 560, 0, 0, MACRO_STD_DELAY},
    {0,  960,  670, 0, 0, MACRO_STD_DELAY},
    {0,  960,  670, 1, 0, MACRO_STD_DELAY},
    {0,  960,  670, 0, 0, MACRO_STD_DELAY},
    {0,  860,  560, 0, 0, MACRO_STD_DELAY},
    {0,  860,  560, 1, 0, MACRO_STD_DELAY},
    {0,  860,  560, 0, 0, MACRO_STD_DELAY},
    {79, 0,    0,   0, 0, MACRO_STD_DELAY},
    {0,  0,    0,   0, 0, 0}
};

#define SEQ_LEN(seq) (sizeof(seq) / sizeof(MacroFrame))

void appKeyboardInit(void) {
    bspKeyInit();
    appMacroInit();
}

void appKeyboardUpdate(void) {

    KeyEvent event = bspKeyScan();

    if (event == KEY_EVENT_TOGGLE_MODE) {
        if (current_mode_ == MODE_NORMAL_KEYBOARD) {
            current_mode_ = MODE_MACRO_KEYBOARD;
        } else {
            current_mode_ = MODE_NORMAL_KEYBOARD;
        }
    }

    if (current_mode_ == MODE_MACRO_KEYBOARD) {
        if (!appMacroIsRunning()) {
            switch (event) {
                case KEY_EVENT_50_PRESSED:
                    appMacroPlay(MACRO_SEQUENCE_50, SEQ_LEN(MACRO_SEQUENCE_50));
                    break;
                case KEY_EVENT_100_PRESSED:
                    appMacroPlay(MACRO_SEQUENCE_100, SEQ_LEN(MACRO_SEQUENCE_100));
                    break;
                case KEY_EVENT_200_PRESSED:
                    // 你可以仿照上方自行添加 MACRO_SEQUENCE_200 数组并在此处调用
                    break;
                default:
                    break;
            }
        }
    } else if (current_mode_ == MODE_NORMAL_KEYBOARD) {
        // TODO: 在此开发普通键盘 USB HID 逻辑
    }

    appMacroUpdate();
}