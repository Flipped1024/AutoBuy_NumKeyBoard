#include "bsp_ec11.h"

bool bspReadEc11PinB(void) {
    return HAL_GPIO_ReadPin(EC11_PORT_B, EC11_PIN_B) == GPIO_PIN_SET;
}

bool bspReadEc11PinBtn(void) {
    return HAL_GPIO_ReadPin(EC11_PORT_BTN, EC11_PIN_BTN) == GPIO_PIN_SET;
}