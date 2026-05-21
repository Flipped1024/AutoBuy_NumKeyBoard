#ifndef BSP_EC11_H
#define BSP_EC11_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

/* HW pin definitions, adjust according to actual CubeMX config */
#define EC11_PORT_A   GPIOA
#define EC11_PIN_A    GPIO_PIN_0

#define EC11_PORT_B   GPIOA
#define EC11_PIN_B    GPIO_PIN_1

#define EC11_PORT_BTN GPIOA
#define EC11_PIN_BTN  GPIO_PIN_2

bool bspReadEc11PinB(void);
bool bspReadEc11PinBtn(void);

#endif