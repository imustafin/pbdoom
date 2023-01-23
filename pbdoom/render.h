#ifndef PBDOOM_RENDER_H
#define PBDOOM_RENDER_H

#include "inkview.h"

#include "../sdldoom-1.10/doomtype.h"
#include "../sdldoom-1.10/doomdef.h"

typedef enum {
  DYNAMIC_A2,
  DITHER_AREA_PATTERN_2_LEVEL,
  DITHER_MANUAL_2_PATTERN,
  NO_DITHER
} ink_render_mode;

void pbdoom_draw(unsigned char *screen);
void pbdoom_set_palette(byte *palette);
void pbdoom_render_set_mode(ink_render_mode mode);

int ink_palette[256];
extern ink_render_mode i_render_mode;
extern icontext_menu *main_menu;

// Top panel height
extern const int panel_height;

#endif /* PBDOOM_RENDER_H */
