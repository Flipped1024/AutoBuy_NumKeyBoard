#include "app_ec11.h"
#include "bsp_ec11.h"
#include "stm32f1xx_hal.h"

/* EXTI software debounce threshold in milliseconds */
#define DEBOUNCE_DELAY_MS 2

static Ec11Device ec11_dev;

void appEc11Init(void) {
    ec11_dev.count_ = 0;
    ec11_dev.direction_ = EC11_DIR_NONE;
    ec11_dev.button_state_ = EC11_BTN_RELEASED;
}

void appEc11ExtiCallback(uint16_t gpio_pin) {
    static uint32_t last_a_trigger = 0;
    static uint32_t last_btn_trigger = 0;
    uint32_t current_tick = HAL_GetTick();

    /* Process encoder rotation */
    if (gpio_pin == EC11_PIN_A) {
        if ((current_tick - last_a_trigger) > DEBOUNCE_DELAY_MS) {
            if (bspReadEc11PinB()) {
                ec11_dev.count_++;
                ec11_dev.direction_ = EC11_DIR_CW;
            } else {
                ec11_dev.count_--;
                ec11_dev.direction_ = EC11_DIR_CCW;
            }
            last_a_trigger = current_tick;
        }
    } 
    /* Process encoder button */
    else if (gpio_pin == EC11_PIN_BTN) {
        if ((current_tick - last_btn_trigger) > DEBOUNCE_DELAY_MS) {
            if (bspReadEc11PinBtn()) {
                ec11_dev.button_state_ = EC11_BTN_RELEASED;
            } else {
                ec11_dev.button_state_ = EC11_BTN_PRESSED;
            }
            last_btn_trigger = current_tick;
        }
    }
}

int32_t appEc11GetCount(void) {
    return ec11_dev.count_;
}

Ec11Direction appEc11GetDirection(void) {
    return ec11_dev.direction_;
}

Ec11ButtonState appEc11GetButtonState(void) {
    return ec11_dev.button_state_;
}