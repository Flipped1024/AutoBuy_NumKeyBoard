#ifndef APP_MACRO_H
#define APP_MACRO_H

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    uint16_t key_value_;
    uint16_t x_position_;
    uint16_t y_position_;
    uint8_t mouse_left_;
    uint8_t mouse_right_;
    uint32_t duration_ms_;  // 修改：表示该状态需要以30Hz频率持续发送的总时长
} MacroFrame;

void appMacroInit(void);
void appMacroUpdate(void);
bool appMacroPlay(const MacroFrame* macro_sequence, uint16_t sequence_length);
bool appMacroIsRunning(void);

#endif // APP_MACRO_H