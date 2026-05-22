#ifndef APP_MACRO_H
#define APP_MACRO_H

#include <stdint.h>
#include <stdbool.h> 

#define DUAL_KEY(key1, key2) ((uint16_t)(((key2) << 8) | (key1)))

typedef struct {
    uint16_t key_value_;   
    uint16_t x_position_;
    uint16_t y_position_;
    uint8_t  mouse_left_;
    uint8_t  mouse_right_;
    uint32_t duration_ms_;
} MacroFrame;

void appMacroInit(void);
void appMacroUpdate(void);

bool appMacroPlay(const MacroFrame *sequence, uint16_t length);
bool appMacroIsRunning(void);

#endif // APP_MACRO_H