#include "inkview.h"

#include "sdldoom-1.10/i_main.h"

char **new_argv;
int new_argc;

typedef struct {
  int x;
  int y;
  int w;
  int h;
  int pressed;
} button_t;

enum GUI_BUTTON {
  GUI_BUTTON_UP,
  GUI_BUTTON_DOWN,
  GUI_BUTTON_LEFT,
  GUT_BUTTON_RIGHT,
  GUI_BUTTON_ENTER
};

#define BUTTONS_N 5
button_t buttons[BUTTONS_N] = {
  {200, 1400, 100, 100},
  {200, 1500, 100, 100},
  {100, 1500, 100, 100},
  {300, 1500, 100, 100},
  {400, 1400, 200, 200}
};

void draw_buttons() {
  for (int i = 0; i < BUTTONS_N; i++) {
    button_t *b = &buttons[i];
    if (b->pressed) {
      FillArea(b->x, b->y, b->w, b->h, BLACK);
    } else {
      FillArea(b->x, b->y, b->w, b->h, WHITE);
      DrawRect(b->x, b->y, b->w, b->h, BLACK);
    }
    PartialUpdate(b->x, b->y, b->w, b->h);
  }
}

void handle_buttons(int t, int a, int b) {
  if (t == EVT_POINTERDOWN || t == EVT_POINTERMOVE || t == EVT_POINTERUP) {
    int x = a;
    int y = b;

    for (int i = 0; i < BUTTONS_N; i++) {
      button_t *b = &buttons[i];

      b->pressed = (x >= b->x && x <= b->x + b->w) && (y >= b->y && y <= b->y + b->h);
    }

    draw_buttons();
  }
}

static int main_handler(int event_type, int a, int b)
{
  handle_buttons(event_type, a, b);

  if (EVT_INIT == event_type) {
    ClearScreen();
    FullUpdate();

    // doom_main(new_argc, new_argv);
  }
  else if (EVT_KEYPRESS == event_type) {
    CloseApp();
  }
  return 0;
}


int main (int argc, char* argv[])
{
  new_argv = malloc((argc + 3) * sizeof(*new_argv));
  memmove(new_argv, argv, sizeof(*new_argv) * argc);
  new_argv[argc] = "-files";
  new_argv[argc + 1] = "/mnt/ext1/DOOM2.WAD";
  new_argv[argc + 2] = 0;
  new_argc = argc + 2;

  InkViewMain(main_handler);

  return 0;
}
