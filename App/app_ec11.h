#ifndef APP_EC11_H
#define APP_EC11_H

#include <stdint.h>

typedef enum {
    EC11_DIR_NONE = 0,
    EC11_DIR_CW,
    EC11_DIR_CCW
} Ec11Direction;

typedef enum {
    EC11_BTN_RELEASED = 0,
    EC11_BTN_PRESSED
} Ec11ButtonState;

typedef struct {
    int32_t count_;
    Ec11Direction direction_;
    Ec11ButtonState button_state_;
} Ec11Device;

void appEc11Init(void);
void appEc11ExtiCallback(uint16_t gpio_pin);
int32_t appEc11GetCount(void);
Ec11Direction appEc11GetDirection(void);
Ec11ButtonState appEc11GetButtonState(void);

#endif // APP_EC11_H