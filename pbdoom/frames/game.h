#ifndef PBDOOM_FRAMES_GAME_H
#define PBDOOM_FRAMES_GAME_H

#include "frame.h"
#include "../../sdldoom-1.10/doomtype.h"
#include "../../sdldoom-1.10/doomdef.h"

extern frame game_frame;

typedef enum {
  DYNAMIC_A2,
  DITHER_AREA_PATTERN_2_LEVEL,
  DITHER_MANUAL_2_PATTERN,
  NO_DITHER
} ink_render_mode;
extern ink_render_mode i_render_mode;

void game_frame_set_render_mode(ink_render_mode i_render_mode);
void game_frame_set_palette(byte *palette);
void game_frame_draw_screen(unsigned char *screen);

extern double game_alpha;
extern double game_beta;
extern double game_gamma;

#endif /* PBDOOM_FRAMES_GAME_H */
