#ifndef APP_MACRO_H
#define APP_MACRO_H

#include "stdint.h"
#include "stdbool.h"

// 结构体：定义单帧宏指令操作数据 (大驼峰)
typedef struct {
    uint16_t key_value_;
    uint16_t x_position_;
    uint16_t y_position_;
    uint8_t mouse_left_;
    uint8_t mouse_right_;
    uint32_t delay_after_ms_; // 发送该帧后的非阻塞延时时间
} MacroFrame;

void appMacroInit(void);
void appMacroUpdate(void);
bool appMacroPlay(const MacroFrame* macro_sequence, uint16_t sequence_length);
bool appMacroIsRunning(void);

#endif // APP_MACRO_H