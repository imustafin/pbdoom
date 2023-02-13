#ifndef PBDOOM_FRAMES_KEYBOARD_H
#define PBDOOM_FRAMES_KEYBOARD_H

#include "frame.h"

extern frame keyboard_frame;

typedef enum {
  keyboard_mode_none = 0,
  keyboard_mode_menu = 1,
  keyboard_mode_level = 1 << 1,
  keyboard_mode_message_input = 1 << 2,
} keyboard_mode;

void set_keyboard_mode(keyboard_mode x);

#endif /* PBDOOM_FRAMES_KEYBOARD_H */
