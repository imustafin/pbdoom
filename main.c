#include "inkview.h"

#include "events.h"

#include "sdldoom-1.10/i_main.h"
#include "sdldoom-1.10/doomdef.h"

char **new_argv;
int new_argc;

#define BUTTON_FONT_SIZE 6

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
  {KEY_UPARROW, "↑", 200, 1400, 100, 100},
  {KEY_DOWNARROW, "↓", 200, 1500, 100, 100},
  {KEY_LEFTARROW, "←", 100, 1500, 100, 100},
  {KEY_RIGHTARROW, "→", 300, 1500, 100, 100},
  {KEY_ENTER, "ENTER", 400, 1400, 200, 200},
  {'y', "Y", 400, 1200, 100, 100},
  {'n', "N", 500, 1200, 100, 100},
  {KEY_RCTRL, "CTRL", 900, 1200, 200, 200},
  {' ', "␣", 800, 1400, 400, 100}
};

void draw_button(button_t *b) {
  if (b->pressed) {
    FillArea(b->x, b->y, b->w, b->h, BLACK);
  } else {
    FillArea(b->x, b->y, b->w, b->h, WHITE);
    DrawTextRect(b->x, b->y, b->w, b->h, b->title, ALIGN_CENTER | VALIGN_MIDDLE);
    DrawRect(b->x, b->y, b->w, b->h, BLACK);
  }
  PartialUpdate(b->x, b->y, b->w, b->h);
}

void draw_buttons() {
  for (int i = 0; i < BUTTONS_N; i++) {
    draw_button(&buttons[i]);
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
      pbdoom_event e = {
        pressed ? PBDOOM_EVENT_KEYDOWN : PBDOOM_EVENT_KEYUP,
        b->doom_code,
        0
      };

      pbdoom_post_event(e);

      b->pressed = pressed;
      draw_button(b);
    }
  }
}

void setup_app() {
  ifont *button_font = OpenFont("LiberationSans", BUTTON_FONT_SIZE, 0);
  SetFont(button_font, BLACK);

  SetPanelType(PANEL_ENABLED);
  ClearScreen();
  DrawPanel(NULL, "", "", 0);
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
