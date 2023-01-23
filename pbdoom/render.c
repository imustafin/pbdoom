#include "inkview.h"

#include "render.h"
#include "../events.h"
#include "../sdldoom-1.10/v_video.h"
#include "../sdldoom-1.10/i_system.h"

ink_render_mode i_render_mode = DYNAMIC_A2;
const int panel_height = 100;
icontext_menu *main_menu;


void pbdoom_draw(unsigned char *screen) {
  if (main_menu) {
    return;
  }

  int k = ScreenWidth() / SCREENWIDTH; // size of one pixel
  int ox = (ScreenWidth() - (k * SCREENWIDTH)) / 2;
  FillArea(ox, panel_height, k * SCREENWIDTH, k * SCREENHEIGHT, WHITE);

  unsigned char *line = screen;

  for (int i = 0; i < SCREENHEIGHT; i++) {
    for (int j = 0; j < SCREENWIDTH; j++) {
      FillArea(j * k + ox, panel_height + i * k, k, k, ink_palette[line[j]]);
    }

    line += SCREENWIDTH;
  }

  int ux = ox;
  int uy = panel_height;
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

void pbdoom_set_palette(byte *palette) {
  for (int i = 0; i < 256; i++) {
    byte r = gammatable[usegamma][*palette++];
    byte g = gammatable[usegamma][*palette++];
    byte b = gammatable[usegamma][*palette++];

    ink_palette[i] = (r << 16) | (g << 8) | b;
  }
}

void pbdoom_render_set_mode(ink_render_mode mode) {
  i_render_mode = mode;
}
