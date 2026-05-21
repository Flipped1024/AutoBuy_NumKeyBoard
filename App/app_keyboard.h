#ifndef APP_KEYBOARD_H
#define APP_KEYBOARD_H


#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LGUI   0x08 

#define KEY_NONE       0x00
#define KEY_A          0x04
#define KEY_B          0x05
#define KEY_1          0x1E
#define KEY_2          0x1F
#define KEY_ENTER      0x28
#define KEY_ESCAPE     0x29

void appKeyboardInit(void);
void appKeyboardUpdate(void);

#endif // APP_KEYBOARD_H