#include "inkview.h"

#include "render.h"
#include "../events.h"
#include "../sdldoom-1.10/v_video.h"
#include "../sdldoom-1.10/i_system.h"

void pbdoom_draw(unsigned char *screen) {
  game_frame_draw_screen(screen);
}

void pbdoom_set_palette(byte *palette) {
  game_frame_set_palette(palette);
}

void pbdoom_render_set_mode(ink_render_mode mode) {
  game_frame_set_render_mode(mode);
}
