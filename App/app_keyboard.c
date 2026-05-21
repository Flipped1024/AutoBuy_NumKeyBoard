#include "app_keyboard.h"
#include "bsp_key.h"
#include "app_macro.h"
#include "app_ec11.h"
#include "main.h"
#include <string.h> 
#include "usb_device.h"
#include "usbd_hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef struct {
    uint8_t modifier; 
    uint8_t reserved;    
    uint8_t keycode[6];  
} Keyboard_Report_t;

static Keyboard_Report_t keyboard_report = {0};

#define MACRO_STD_DELAY 25
#define HID_SCAN_INTERVAL_MS 10 

typedef enum
{
    MODE_NORMAL_KEYBOARD,
    MODE_MACRO_KEYBOARD
} KeyboardMode;

static KeyboardMode current_mode_ = MODE_MACRO_KEYBOARD;

// {键值, X, Y, 左键, 右键, 帧后延时}
static const MacroFrame MACRO_SEQUENCE_50[] = {
    {79, 0, 0, 0, 0, MACRO_STD_DELAY},
    {0, 1160, 560, 0, 0, MACRO_STD_DELAY},
    {0, 1160, 560, 1, 0, MACRO_STD_DELAY},
    {0, 1160, 560, 0, 0, MACRO_STD_DELAY},
    {0, 960, 670, 0, 0, MACRO_STD_DELAY},
    {0, 960, 670, 1, 0, MACRO_STD_DELAY},
    {0, 960, 670, 0, 0, MACRO_STD_DELAY},
    {0, 860, 560, 0, 0, MACRO_STD_DELAY},
    {0, 860, 560, 1, 0, MACRO_STD_DELAY},
    {0, 860, 560, 0, 0, MACRO_STD_DELAY},
    {79, 0, 0, 0, 0, MACRO_STD_DELAY},
    {0, 0, 0, 0, 0, 0}};

static const MacroFrame MACRO_SEQUENCE_100[] = {
    {79, 0, 0, 0, 0, MACRO_STD_DELAY},
    {0, 1210, 560, 0, 0, MACRO_STD_DELAY},
    {0, 1210, 560, 1, 0, MACRO_STD_DELAY},
    {0, 1210, 560, 0, 0, MACRO_STD_DELAY},
    {0, 960, 670, 0, 0, MACRO_STD_DELAY},
    {0, 960, 670, 1, 0, MACRO_STD_DELAY},
    {0, 960, 670, 0, 0, MACRO_STD_DELAY},
    {0, 860, 560, 0, 0, MACRO_STD_DELAY},
    {0, 860, 560, 1, 0, MACRO_STD_DELAY},
    {0, 860, 560, 0, 0, MACRO_STD_DELAY},
    {79, 0, 0, 0, 0, MACRO_STD_DELAY},
    {0, 0, 0, 0, 0, 0}};

#define SEQ_LEN(seq) (sizeof(seq) / sizeof(MacroFrame))

typedef struct {
    GPIO_TypeDef* port_;
    uint16_t pin_;
    uint8_t hid_code_;
} KeyMap;

static const KeyMap NUMPAD_MAP[] = {
    {K_NUM_GPIO_Port, K_NUM_Pin, 0x53},           // Num Lock
    {K_DIVISION_GPIO_Port, K_DIVISION_Pin, 0x54}, // /
    {K_MULTIPLE_GPIO_Port, K_MULTIPLE_Pin, 0x55}, // *
    {K_MINUS_GPIO_Port, K_MINUS_Pin, 0x56},       // -
    {K_PLUS_GPIO_Port, K_PLUS_Pin, 0x57},         // +
    {K_ENTER_GPIO_Port, K_ENTER_Pin, 0x58},       // Enter
    {K_1_GPIO_Port, K_1_Pin, 0x59},
    {K_2_GPIO_Port, K_2_Pin, 0x5A},
    {K_3_GPIO_Port, K_3_Pin, 0x5B},
    {K_4_GPIO_Port, K_4_Pin, 0x5C},
    {K_5_GPIO_Port, K_5_Pin, 0x5D},
    {K_6_GPIO_Port, K_6_Pin, 0x5E},
    {K_7_GPIO_Port, K_7_Pin, 0x5F},
    {K_8_GPIO_Port, K_8_Pin, 0x60},
    {K_9_GPIO_Port, K_9_Pin, 0x61},
    {K_0_GPIO_Port, K_0_Pin, 0x62},
    {K_DOT_GPIO_Port, K_DOT_Pin, 0x63},           // .
    {K_BACK_GPIO_Port, K_BACK_Pin, 0x2A}          // Backspace
};
#define NUMPAD_KEY_COUNT (sizeof(NUMPAD_MAP) / sizeof(KeyMap))

static void processNormalKeyboard(void);

void appKeyboardInit(void)
{
    bspKeyInit();
    appMacroInit();
}

void appKeyboardUpdate(void)
{
    
    static Ec11ButtonState last_btn_state = EC11_BTN_RELEASED;
    
    Ec11ButtonState current_btn_state = appEc11GetButtonState();
    KeyEvent event = bspKeyScan();

    if (current_btn_state == EC11_BTN_PRESSED && last_btn_state == EC11_BTN_RELEASED)
    {
        current_mode_ = (current_mode_ == MODE_NORMAL_KEYBOARD) ? MODE_MACRO_KEYBOARD : MODE_NORMAL_KEYBOARD;
    }
    last_btn_state = current_btn_state; // 更新上一帧状态

    if (event == KEY_EVENT_TOGGLE_MODE)
    {
        current_mode_ = (current_mode_ == MODE_NORMAL_KEYBOARD) ? MODE_MACRO_KEYBOARD : MODE_NORMAL_KEYBOARD;
    }

    if (current_mode_ == MODE_MACRO_KEYBOARD)
    {
        if (!appMacroIsRunning())
        {
            switch (event)
            {
            case KEY_EVENT_50_PRESSED:
                appMacroPlay(MACRO_SEQUENCE_50, SEQ_LEN(MACRO_SEQUENCE_50));
                break;
            case KEY_EVENT_100_PRESSED:
                appMacroPlay(MACRO_SEQUENCE_100, SEQ_LEN(MACRO_SEQUENCE_100));
                break;
            case KEY_EVENT_200_PRESSED:
                // appMacroPlay(MACRO_SEQUENCE_200, SEQ_LEN(MACRO_SEQUENCE_200));
                break;
            default:
                break;
            }
        }
    }
    else if (current_mode_ == MODE_NORMAL_KEYBOARD)
    {
        processNormalKeyboard();
    }

    appMacroUpdate();
}

/**
 * @brief 普通键盘模式下的矩阵/阵列扫描与 USB HID 报文组装发送逻辑
 */
static void processNormalKeyboard(void)
{
    static uint32_t last_scan_time = 0;
    static uint8_t last_report[8] = {0};
    uint8_t current_report[8] = {0};

    uint32_t current_tick = HAL_GetTick();
    
    if (current_tick - last_scan_time < HID_SCAN_INTERVAL_MS)
    {
        return;
    }
    last_scan_time = current_tick;

    // USB HID 键盘报文格式:
    // Byte 0: 控制键 (Ctrl/Shift/Alt/GUI) - 默认为 0
    // Byte 1: 保留字 - 默认为 0
    // Byte 2-7: 普通按键 (最多 6 键无冲)
    uint8_t key_index = 2; 
    
    for (int i = 0; i < NUMPAD_KEY_COUNT; i++)
    {
        if (HAL_GPIO_ReadPin(NUMPAD_MAP[i].port_, NUMPAD_MAP[i].pin_) == GPIO_PIN_RESET)
        {
            current_report[key_index++] = NUMPAD_MAP[i].hid_code_;
            if (key_index >= 8) 
            {
                break;
            }
        }
    }

    if (memcmp(last_report, current_report, 8) != 0)
    {
        memcpy(last_report, current_report, 8);

        USBD_HID_SendReport(&hUsbDeviceFS, current_report, 8);
    }
}

/**
  * @brief  向电脑发送指定按键
  * @param  modifier: 修饰键状态值 (如 KEY_MOD_LSHIFT)
  * @param  key_code: 标准 USB HID 键码 (如 'A' 对应 0x04)
  */
void Keyboard_Send_Key(uint8_t modifier, uint8_t key_code)
{
    memset(&keyboard_report, 0, sizeof(Keyboard_Report_t));
    
    keyboard_report.modifier = modifier;
    keyboard_report.keycode[0] = key_code; // 放入第一个键槽
    
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyboard_report, sizeof(Keyboard_Report_t));
    HAL_Delay(20); 
    
    memset(&keyboard_report, 0, sizeof(Keyboard_Report_t));
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyboard_report, sizeof(Keyboard_Report_t));
    HAL_Delay(20);
}