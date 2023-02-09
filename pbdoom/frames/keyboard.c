#include "keyboard.h"
#include "../render.h"
#include "../gui.h"

#include "../../events.h"

typedef struct {
  int doom_code;
  char *title;
  int x;
  int y;
  int w;
  int h;
  int pressed;
} button_t;

#define BUTTONS_N 9
button_t buttons[BUTTONS_N] = {
  {KEY_UPARROW, "↑"},
  {KEY_DOWNARROW, "↓"},
  {KEY_LEFTARROW, "←"},
  {KEY_RIGHTARROW, "→"},
  {KEY_ENTER, "ENTER"},
  {'y', "Y"},
  {'n', "N"},
  {KEY_RCTRL, "CTRL"},
  {' ', "␣"},
};

void compute_button_positions() {
  int sw = ScreenWidth();
  int sh = ScreenHeight();

  int b = 70 * dp;

  int i_up = 0;
  int i_down = 1;
  int i_left = 2;
  int i_right = 3;
  int i_enter = 4;
  int i_y = 5;
  int i_n = 6;
  int i_ctrl = 7;
  int i_space = 8;

  int ox = b;

  // content height is 3 buttons
  int ch = 3 * b;
  int ym = (keyboard_frame.h - ch) / 2;
  int y = keyboard_frame.y;

  buttons[i_up].x = ox + b - 1;
  buttons[i_up].y = y + ym;
  buttons[i_up].w = b;
  buttons[i_up].h = b;

  buttons[i_left].x = ox + 0;
  buttons[i_left].y = y + ym + b;
  buttons[i_left].w = b;
  buttons[i_left].h = b;

  buttons[i_right].x = ox + (2 * b) - 2;
  buttons[i_right].y = y + ym + b;
  buttons[i_right].w = b;
  buttons[i_right].h = b;

  buttons[i_down].x = ox + b - 1;
  buttons[i_down].y = y + ym + 2 * b;
  buttons[i_down].w = b;
  buttons[i_down].h = b;

  buttons[i_enter].x = ox + (4 * b);
  buttons[i_enter].y = y + ym + b;
  buttons[i_enter].w = 2 * b;
  buttons[i_enter].h = 2 * b;

  buttons[i_y].x = ox + (4 * b);
  buttons[i_y].y = y + ym;
  buttons[i_y].w = b;
  buttons[i_y].h = b;

  buttons[i_n].x = ox + (5 * b);
  buttons[i_n].y = y + ym;
  buttons[i_n].w = b;
  buttons[i_n].h = b;

  buttons[i_ctrl].w = 3 * b;
  buttons[i_ctrl].h = 2 * b;
  buttons[i_ctrl].x = sw - ox - buttons[i_ctrl].w;
  buttons[i_ctrl].y = y + ym;

  buttons[i_space].w = 3 * b;
  buttons[i_space].h = b;
  buttons[i_space].x = sw - ox - buttons[i_space].w;
  buttons[i_space].y = y + ym + 2 * b;
}

void keyboard_frame_install(int x, int y, int w, int h) {
  keyboard_frame.x = x;
  keyboard_frame.y = y;
  keyboard_frame.w = w;
  keyboard_frame.h = h;

  compute_button_positions();
}

void keyboard_frame_uninstall() {
  
}


static ifont *font;

static void set_font() {
  if (!font) {
    font = OpenFont(DEFAULTFONTM, 30, 0);
  }
  SetFont(font, BLACK);
}

void keyboard_frame_draw() {
  set_font();
  for (int i = 0; i < BUTTONS_N; i++) {
    button_t *b = &buttons[i];
    FillArea(b->x, b->y, b->w, b->h, WHITE);
    DrawTextRect(b->x, b->y, b->w, b->h, b->title, ALIGN_CENTER | VALIGN_MIDDLE);
    DrawRect(b->x, b->y, b->w, b->h, BLACK);
  }
  DrawRect(keyboard_frame.x, keyboard_frame.y, keyboard_frame.w, keyboard_frame.h, BLACK);
}

int keyboard_frame_handle(int t, int index, int cnt) {
  if (t != EVT_MTSYNC) {
    return false;
  }

  int presses[BUTTONS_N];
  for (int i = 0; i < BUTTONS_N; i++) {
    presses[i] = 0;
  }

  iv_mtinfo *mt_all = GetTouchInfoI(index);

  for (int i = 0; i < cnt; i++) {
    iv_mtinfo *mt = mt_all + i;

    if (!mt->active) {
      continue;
    }

    int x = mt->x;
    int y = mt->y;

    for (int j = 0; j < BUTTONS_N; j++) {
      button_t *b = &buttons[j];

      int pressed = (x >= b->x && x <= b->x + b->w)
        && (y >= b->y && y <= b->y + b->h);
      if (pressed) {
        presses[j] = 1;
      }
    }
  }

  int result = 0;
  for (int i = 0; i < BUTTONS_N; i++) {
    button_t *b = &buttons[i];

    int pressed = presses[i];

    if (pressed != b->pressed) {
      b->pressed = pressed;

      int e_type;
      pbdoom_event e;
      e.a = b->doom_code;
      e.b = 0;
      e.type = pressed ? PBDOOM_EVENT_KEYDOWN : PBDOOM_EVENT_KEYUP;

      pbdoom_post_event(e);
      result = 1;
    }
  }

  return result;
}

frame keyboard_frame = {
  .install = *keyboard_frame_install,
  .uninstall = *keyboard_frame_uninstall,
  .draw = *keyboard_frame_draw,
  .handle = *keyboard_frame_handle
};
