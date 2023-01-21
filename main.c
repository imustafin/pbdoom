#include "inkview.h"

#include "events.h"

#include "sdldoom-1.10/i_main.h"
#include "sdldoom-1.10/doomdef.h"

ifont *button_font;

void set_font() {
  if (!button_font) {
    button_font = OpenFont("LiberationSans", 30, 0);
  }
  SetFont(button_font, BLACK);
}

char **new_argv;
int new_argc;

typedef enum {
  BTYPE_NORMAL,
  BTYPE_RENDER,
} btype_t;

typedef struct {
  btype_t btype;
  int doom_code;
  char *title;
  int x;
  int y;
  int w;
  int h;
  int pressed;
} button_t;

#define BUTTONS_N 13
button_t buttons[BUTTONS_N] = {
  {BTYPE_NORMAL, KEY_UPARROW, "↑"},
  {BTYPE_NORMAL, KEY_DOWNARROW, "↓"},
  {BTYPE_NORMAL, KEY_LEFTARROW, "←"},
  {BTYPE_NORMAL, KEY_RIGHTARROW, "→"},
  {BTYPE_NORMAL, KEY_ENTER, "ENTER"},
  {BTYPE_NORMAL, 'y', "Y"},
  {BTYPE_NORMAL, 'n', "N"},
  {BTYPE_NORMAL, KEY_RCTRL, "CTRL"},
  {BTYPE_NORMAL, ' ', "␣"},
  {BTYPE_RENDER, DYNAMIC_A2, "DynamicA2"},
  {BTYPE_RENDER, DITHER_AREA_PATTERN_2_LEVEL, "DitherAreaPattern2Level"},
  {BTYPE_RENDER, DITHER_MANUAL_2_PATTERN, "DitherArea(2, PATTERN)"},
  {BTYPE_RENDER, NO_DITHER, "No Dither"}
};

void draw_buttons() {
  for (int i = 0; i < BUTTONS_N; i++) {
    button_t *b = &buttons[i];
    FillArea(b->x, b->y, b->w, b->h, WHITE);
    set_font();
    DrawTextRect(b->x, b->y, b->w, b->h, b->title, ALIGN_CENTER | VALIGN_MIDDLE);
    DrawRect(b->x, b->y, b->w, b->h, BLACK);
  }
}

void handle_buttons(int t, int index, int cnt) {
  if (t != EVT_MTSYNC) {
    return;
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

  for (int i = 0; i < BUTTONS_N; i++) {
    button_t *b = &buttons[i];

    int pressed = presses[i];

    if (pressed != b->pressed) {
      b->pressed = pressed;

      int e_type;
      pbdoom_event e;
      e.a = b->doom_code;
      e.b = 0;
      if (b->btype == BTYPE_NORMAL) {
        e.type = pressed ? PBDOOM_EVENT_KEYDOWN : PBDOOM_EVENT_KEYUP;
      } else if (b->btype == BTYPE_RENDER) {
        if (!pressed) {
          continue;
        }
        e.type = PBDOOM_EVENT_SWITCH_RENDER_MODE;
      }

      pbdoom_post_event(e);
    }
  }
}

void compute_button_positions() {
  int sw = ScreenWidth();
  int sh = ScreenHeight();
  printf("SW %d, SH %d\n", sw, sh);

  int b = (sw < sh ? sw : sh) / 15;

  int i_up = 0;
  int i_down = 1;
  int i_left = 2;
  int i_right = 3;
  int i_enter = 4;
  int i_y = 5;
  int i_n = 6;
  int i_ctrl = 7;
  int i_space = 8;
  int i_a2 = 9;
  int i_d1 = 10;
  int i_d2 = 11;
  int i_nd = 12;

  int ox = b / 2;

  buttons[i_up].x = ox + b - 1;
  buttons[i_up].y = sh - (5 * b);
  buttons[i_up].w = b;
  buttons[i_up].h = b;

  buttons[i_left].x = ox + 0;
  buttons[i_left].y = sh - (4 * b) - 1;
  buttons[i_left].w = b;
  buttons[i_left].h = b;

  buttons[i_right].x = ox + (2 * b) - 2;
  buttons[i_right].y = sh - (4 * b) - 1;
  buttons[i_right].w = b;
  buttons[i_right].h = b;

  buttons[i_down].x = ox + b - 1;
  buttons[i_down].y = sh - (3 * b) - 2;
  buttons[i_down].w = b;
  buttons[i_down].h = b;

  buttons[i_enter].x = ox + (4 * b);
  buttons[i_enter].y = sh - (4 * b);
  buttons[i_enter].w = 2 * b;
  buttons[i_enter].h = 2 * b;

  buttons[i_y].x = ox + (4 * b);
  buttons[i_y].y = sh - (5 * b);
  buttons[i_y].w = b;
  buttons[i_y].h = b;

  buttons[i_n].x = ox + (5 * b);
  buttons[i_n].y = sh - (5 * b);
  buttons[i_n].w = b;
  buttons[i_n].h = b;

  buttons[i_ctrl].w = 3 * b;
  buttons[i_ctrl].h = 2 * b;
  buttons[i_ctrl].x = sw - ox - buttons[i_ctrl].w;
  buttons[i_ctrl].y = sh - (5 * b);

  buttons[i_space].w = 3 * b;
  buttons[i_space].h = b;
  buttons[i_space].x = sw - ox - buttons[i_space].w;
  buttons[i_space].y = sh - (3 * b);

  buttons[i_a2].w = 2 * b;
  buttons[i_a2].h = b;
  buttons[i_a2].x = ox - 1;
  buttons[i_a2].y = sh - (7 * b);

  buttons[i_d1].w = 2 * b;
  buttons[i_d1].h = b;
  buttons[i_d1].x = 2 * ox + (2 * b);
  buttons[i_d1].y = sh - (7 * b);

  buttons[i_d2].w = 2 * b;
  buttons[i_d2].h = b;
  buttons[i_d2].x = 3 * ox + (4 * b);
  buttons[i_d2].y = sh - (7 * b);

  buttons[i_nd].w = 2 * b;
  buttons[i_nd].h = b;
  buttons[i_nd].x = 4 * ox + (6 * b);
  buttons[i_nd].y = sh - (7 * b);
}

void setup_app() {

  SetPanelType(PANEL_ENABLED);
  ClearScreen();
  DrawPanel(NULL, "", "", 0);
  compute_button_positions();
  draw_buttons();
  FullUpdate();
}

static int main_handler(int event_type, int a, int b)
{
  handle_buttons(event_type, a, b);

  if (EVT_INIT == event_type) {
    setup_app();

    doom_main(new_argc, new_argv);
  }
  else if (EVT_KEYPRESS == event_type) {
    CloseApp();
  }
  else if (EVT_EXIT == event_type) {
    pbdoom_event e = { PBDOOM_EVENT_EXIT, 0, 0 };
    pbdoom_post_event(e);
    return 1; // we will exit ourselves on next tic
  }
  return 0;
}


int main (int argc, char* argv[])
{
  new_argc = argc;
  new_argv = argv;

  InkViewMain(main_handler);

  return 0;
}
