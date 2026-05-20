#include "app_keyboard.h"
#include "bsp_key.h"
#include "app_macro.h"

#define MACRO_STD_DELAY 25

typedef enum {
    MODE_NORMAL_KEYBOARD,
    MODE_MACRO_KEYBOARD
} KeyboardMode;

static KeyboardMode current_mode_ = MODE_MACRO_KEYBOARD; 

// 数据驱动：将冗余的代码逻辑转化为紧凑的数据数组
// 格式: {键值, X, Y, 左键, 右键, 帧后延时}
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

// 长度宏定义辅助计算
#define SEQ_LEN(seq) (sizeof(seq) / sizeof(MacroFrame))

void appKeyboardInit(void) {
    bspKeyInit();
    appMacroInit();
}

void appKeyboardUpdate(void) {
    // 1. 扫描按键事件 (非阻塞)
    KeyEvent event = bspKeyScan();

    // 2. 模式切换处理 (此处预留接口，用户可自定义触发事件)
    if (event == KEY_EVENT_TOGGLE_MODE) {
        if (current_mode_ == MODE_NORMAL_KEYBOARD) {
            current_mode_ = MODE_MACRO_KEYBOARD;
        } else {
            current_mode_ = MODE_NORMAL_KEYBOARD;
        }
    }

    // 3. 执行当前模式的具体逻辑
    if (current_mode_ == MODE_MACRO_KEYBOARD) {
        // 如果宏正在运行，丢弃新的按键输入防止错乱
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

    // 4. 驱动底层状态机 (非阻塞，必须在 while(1) 中高频调用)
    appMacroUpdate();
}