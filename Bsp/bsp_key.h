#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "main.h"

typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_50_PRESSED,
    KEY_EVENT_100_PRESSED,
    KEY_EVENT_200_PRESSED,
    KEY_EVENT_TOGGLE_MODE 
} KeyEvent;

void bspKeyInit(void);
KeyEvent bspKeyScan(void);

#endif // BSP_KEY_H