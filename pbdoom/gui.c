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
void set_dpi(int gdpi) {
  if (gdpi > 0) {
    dpi = gdpi;
  } else {
    char *s = GetDeviceModel();

    // default fallback
    dpi = 212;

    if (!strcmp(s, "PB1040")) { // PocketBook InkPad X (1040)
      dpi = 227;
    } else if(!strcmp(s, "PB632")) { // PocketBook Touch HD Plus / Touch HD 3 (632)
      dpi = 300;
    }
  }

  // like Android's dp
  dp = (double) dpi / 160;
}

int gui_w, gui_h;
gui_size_t gui_size;

// screen diagonal in inches
void set_gui_size() {
  double h = gui_h / dpi;
  double w = gui_w / dpi;

  // screen diagonal in inches, squared
  double dsq = h * h + w * w;

  if (dsq <= (10 * 10) - 0.9) {
    gui_size = size_m;
  } else {
    gui_size = size_l;
  }
}

int controls_h() {
  if (gui_size > size_m) {
    return 400 * dp;
  } else {
    return 250 * dp;
  }
}

frame *controls_frame;
void switch_controls_frame(frame *new_frame) {
  if (controls_frame) {
    controls_frame->uninstall();
  }

  int ch;
  if (gui_size > size_m) {
    ch = 400 * dp;
  } else {
    ch = 250 * dp;
  }
  controls_frame = new_frame;
  controls_frame->install(0, gui_h - ch, gui_w, ch);
}

void install_frames() {
  int sw = gui_w;
  int sh = gui_h;

  int h = gui_h;

  int ph;
  if (gui_size > size_m) {
    ph = 60 * dp;
  } else {
    ph = 48 * dp;
  }
  panel_frame.install(0, 0, sw, ph);
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

// pass -1 for default
void gui_init(int gw, int gh, int gdpi) {
  set_dpi(gdpi);
  gui_w = gw > 0 ? gw : ScreenWidth();
  gui_h = gh > 0 ? gh : ScreenHeight();
  set_gui_size();
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
