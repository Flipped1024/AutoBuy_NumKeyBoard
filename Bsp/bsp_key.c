#include "bsp_key.h"

#define KEY_DEBOUNCE_MS 20

void bspKeyInit(void) {

}

KeyEvent bspKeyScan(void) {
    static uint32_t last_scan_time = 0;
    static uint8_t K_0_last = GPIO_PIN_SET;
    static uint8_t K_DOT_last = GPIO_PIN_SET;
    static uint8_t K_ENTER_last = GPIO_PIN_SET;

    uint32_t current_tick = HAL_GetTick();
    
    if (current_tick - last_scan_time < KEY_DEBOUNCE_MS) {
        return KEY_EVENT_NONE;
    }
    last_scan_time = current_tick;

    uint8_t K_0_now = HAL_GPIO_ReadPin(K_0_GPIO_Port, K_0_Pin);
    uint8_t K_DOT_now = HAL_GPIO_ReadPin(K_DOT_GPIO_Port, K_DOT_Pin);
    uint8_t K_ENTER_now = HAL_GPIO_ReadPin(K_ENTER_GPIO_Port, K_ENTER_Pin);

    KeyEvent event = KEY_EVENT_NONE;

    if (K_0_last == GPIO_PIN_SET && K_0_now == GPIO_PIN_RESET) {
        event = KEY_EVENT_50_PRESSED;
    } else if (K_DOT_last == GPIO_PIN_SET && K_DOT_now == GPIO_PIN_RESET) {
        event = KEY_EVENT_100_PRESSED;
    } else if (K_ENTER_last == GPIO_PIN_SET && K_ENTER_now == GPIO_PIN_RESET) {
        event = KEY_EVENT_200_PRESSED;
    }

    K_0_last = K_0_now;
    K_DOT_last = K_DOT_now;
    K_ENTER_last = K_ENTER_now;

    return event;
}