#include "inkview.h"

#include "gui.h"
#include "render.h"

#include "frames/game.h"
#include "frames/keyboard.h"
#include "frames/panel.h"
#include "frames/color_settings.h"

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

int controls_h() {
  return 400 * dp;
}

frame *controls_frame;
void switch_controls_frame(frame *new_frame) {
  if (controls_frame) {
    controls_frame->uninstall();
  }
  int ch = 400 * dp;
  controls_frame = new_frame;
  controls_frame->install(0, ScreenHeight() - ch, ScreenWidth(), ch);
}

void install_frames() {
  int sw = ScreenWidth();
  int sh = ScreenHeight();

  int h = ScreenHeight();

  panel_frame.install(0, 0, sw, 60 * dp);
  h -= panel_frame.h;

  switch_controls_frame(&keyboard_frame);
  h -= keyboard_frame.h;

  game_frame.install(0, panel_frame.h, sw, h);
}

void draw_frames() {
  panel_frame.draw();
  game_frame.draw();

  if (controls_frame) {
    controls_frame->draw();
  }
}

void gui_init() {
  set_dpi();
  install_frames();
  draw_frames();
}

void redraw_frame(frame *f) {
  int x = f->x;
  int y = f->y;
  int w = f->w;
  int h = f->h;
  FillArea(x, y, w, h, WHITE);
  f->draw();
  PartialUpdate(x, y, w, h);
}

void gui_open_color_settings() {
  switch_controls_frame(&color_settings_frame);
  redraw_frame(controls_frame);
}

void gui_open_keyboard() {
  switch_controls_frame(&keyboard_frame);
  redraw_frame(controls_frame);
}

int gui_handle(int t, int a, int b) {
  return (controls_frame && controls_frame->handle(t, a, b))
    || panel_frame.handle(t, a, b);
}
