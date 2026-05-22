#include "app_macro.h"
#include "usart.h"
#include "referee_protocol.h"
#include "referee_frame_process.h"
#include "string.h"

typedef struct {
    bool is_running_;
    const MacroFrame* current_sequence_;
    uint16_t sequence_length_;
    uint16_t current_step_;
    uint32_t step_start_tick_;
    uint32_t last_tx_tick_;
} MacroContext;

static MacroContext macro_ctx_ = {0};
static custom_client_data_t custom_client_data_ = {0};
static uint8_t client_data_buffer_[8] = {0};

void appMacroInit(void) {
    macro_ctx_.is_running_ = false;
}

bool appMacroPlay(const MacroFrame* macro_sequence, uint16_t sequence_length) {
    if (macro_ctx_.is_running_ || macro_sequence == NULL || sequence_length == 0) {
        return false; 
    }
    
    macro_ctx_.current_sequence_ = macro_sequence;
    macro_ctx_.sequence_length_ = sequence_length;
    macro_ctx_.current_step_ = 0;
    macro_ctx_.is_running_ = true;
    
    uint32_t current_tick = HAL_GetTick();
    macro_ctx_.step_start_tick_ = current_tick;
    macro_ctx_.last_tx_tick_ = current_tick - 33; // 确保立刻发送第一帧
    
    return true;
}

bool appMacroIsRunning(void) {
    return macro_ctx_.is_running_;
}

void appMacroUpdate(void) {
    if (!macro_ctx_.is_running_) {
        return;
    }

    uint32_t current_tick = HAL_GetTick();

    // 1. 检查当前动作持续时间是否结束，切换到下一个动作
    if (current_tick - macro_ctx_.step_start_tick_ >= macro_ctx_.current_sequence_[macro_ctx_.current_step_].duration_ms_) {
        macro_ctx_.current_step_++;
        macro_ctx_.step_start_tick_ = current_tick;

        if (macro_ctx_.current_step_ >= macro_ctx_.sequence_length_) {
            macro_ctx_.is_running_ = false;
            return;
        }
    }

    // 2. 按 30Hz (约33ms) 的频率连续发送，防止客户端引擎滤除噪声
    if (current_tick - macro_ctx_.last_tx_tick_ >= 33) {
        const MacroFrame* current_frame = &macro_ctx_.current_sequence_[macro_ctx_.current_step_];
        
        custom_client_data_.key_value = current_frame->key_value_;
        custom_client_data_.x_position = current_frame->x_position_;
        custom_client_data_.y_position = current_frame->y_position_;
        custom_client_data_.mouse_left = current_frame->mouse_left_;
        custom_client_data_.mouse_right = current_frame->mouse_right_;

        memcpy(client_data_buffer_, &custom_client_data_, 8);
        HAL_UART_Transmit(&huart1, referee_pack_data(0x0306, client_data_buffer_, 8), getRefSentDataLen(), 30);

        macro_ctx_.last_tx_tick_ = current_tick;
    }
}