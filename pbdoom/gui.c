#include "gui.h"
#include "render.h"

#include "frames/game.h"
#include "frames/keyboard.h"
#include "frames/panel.h"

int dpi = 0;

// Mappings collected from various device listings on websites
// also consult
// https://github.com/koreader/koreader/blob/master/frontend/device/pocketbook/device.lua
void set_dpi() {
  char *s = GetDeviceModel();

  // default fallback
  dpi = 212;

  if (!strcmp(s, "PB1040")) { // PocketBook InkPad X (1040)
    dpi = 227;
  } else if(!strcmp(s, "PB632")) { // PocketBook Touch HD Plus / Touch HD 3 (632)
    dpi = 300;
  }

  // like Android's dp
  dp = (double) dpi / 160;
}

void install_frames() {
  int sw = ScreenWidth();
  int sh = ScreenHeight();

  int h = ScreenHeight();

  panel_frame.install(0, 0, sw, 60 * dp);
  h -= panel_frame.h;


  int kh = 400 * dp;
  keyboard_frame.install(0, sh - kh, sw, kh);
  h -= keyboard_frame.h;

  game_frame.install(0, panel_frame.h, sw, h);
}

void draw_frames() {
  panel_frame.draw();
  game_frame.draw();
  keyboard_frame.draw();
}

void gui_init() {
  set_dpi();
  install_frames();
  draw_frames();
}

int gui_handle(int t, int a, int b) {
  return keyboard_frame.handle(t, a, b)
    || panel_frame.handle(t, a, b);
}
