#include <math.h>

#include "game.h"
#include "panel.h"

#include "../../sdldoom-1.10/v_video.h"
#include "../../sdldoom-1.10/i_system.h"

void game_frame_install(int x, int y, int w, int h) {
  game_frame.x = x;
  game_frame.y = y;
  game_frame.w = w;
  game_frame.h = h;
}

void game_frame_uninstall() {
  
}

ink_render_mode i_render_mode = DYNAMIC_A2;

// All <r, g, b> in [0, 1]
typedef struct {
  int r;
  int g;
  int b;
} rgb;

rgb ink_palette[256];

void game_frame_set_render_mode(ink_render_mode x) {
  i_render_mode = x;
}

void game_frame_set_palette(byte *palette) {
  for (int i = 0; i < 256; i++) {
    ink_palette[i].r = gammatable[usegamma][*palette++];
    ink_palette[i].g = gammatable[usegamma][*palette++];
    ink_palette[i].b = gammatable[usegamma][*palette++];
  }
}

double game_alpha = 1;
double game_beta = 0;
double game_gamma = 1;

int adjust(double c) {
  double linear = c * game_alpha + game_beta;
  int x = pow(linear / 255, game_gamma) * 255;

  if (x < 0) {
    return 0;
  }
  if (x > 255) {
    return 255;
  }
  return x;
}

int adjusted_color(int idx) {
  rgb c = ink_palette[idx];

  int r = adjust(c.r);
  int g = adjust(c.g);
  int b = adjust(c.b);

  return (r << 16) | (g << 8) | b;
}

static void draw() {
  // do nothing, real draws come through game_frame_draw_screen
}

void game_frame_draw_screen(unsigned char *screen) {
  if (main_menu) {
    return;
  }

  int by_w = game_frame.w / SCREENWIDTH;
  int by_h = game_frame.h / SCREENHEIGHT;
  int k = by_w < by_h ? by_w : by_h; // size of one pixel
  int mx = (game_frame.w - (k * SCREENWIDTH)) / 2;
  int my = (game_frame.h - (k * SCREENHEIGHT)) / 2;

  unsigned char *line = screen;

  for (int i = 0; i < SCREENHEIGHT; i++) {
    for (int j = 0; j < SCREENWIDTH; j++) {
      FillArea(game_frame.x + mx + j * k,
               game_frame.y + my + i * k,
               k, k,
               adjusted_color(line[j]));
    }

    line += SCREENWIDTH;
  }

  int ux = game_frame.x + mx;
  int uy = game_frame.y + my;
  int uw = k * SCREENWIDTH;
  int uh = k * SCREENHEIGHT;

  switch (i_render_mode) {
  case DYNAMIC_A2:
    DynamicUpdateA2(ux, uy, uw, uh);
    break;
  case DITHER_AREA_PATTERN_2_LEVEL:
    DitherAreaPattern2Level(ux, uy, uw, uh);
    PartialUpdate(ux, uy, uw, uh);
    break;
  case DITHER_MANUAL_2_PATTERN:
    DitherArea(ux, uy, uw, uh, 2, DITHER_PATTERN);
    PartialUpdate(ux, uy, uw, uh);
    break;
  case NO_DITHER:
    PartialUpdate(ux, uy, uw, uh);
    break;
  default:
    I_Error("Unknown i_render_mode %d", i_render_mode);
  }
}

frame game_frame = {
  .install = *game_frame_install,
  .uninstall = *game_frame_uninstall,
  .draw = *draw
};
