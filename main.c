#include "inkview.h"

#include "events.h"
#include "pbdoom/render.h"

#include "sdldoom-1.10/i_main.h"
#include "sdldoom-1.10/doomdef.h"

#include "version.h"

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
  BTYPE_MENU
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

#define BUTTONS_N 10
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
  {BTYPE_MENU, 0, "☰"}
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

void send_render_event(ink_render_mode mode) {
  pbdoom_event e;
  e.a = mode;
  e.b = 0;
  e.type = PBDOOM_EVENT_SWITCH_RENDER_MODE;
  pbdoom_post_event(e);
}

void main_menu_handler(int i) {
  switch (i) {
  case -1:
    main_menu = NULL;
    break;
  case 2:
    main_menu = NULL;
    Dialog(
      0,
      "About pbdoom",
      "Pbdoom " GIT_TAG "\n"
      "by Ilgiz Mustafin, based on SDL Doom by Sam Lantinga\n"
      "Get the Source code and other info at \n"
      "https://github.com/imustafin/pbdoom"
      ,
      "Ok", NULL, NULL
    );
    break;
  case 10:
    send_render_event(DYNAMIC_A2);
    main_menu = NULL;
    break;
  case 11:
    send_render_event(DITHER_AREA_PATTERN_2_LEVEL);
    main_menu = NULL;
    break;
  case 12:
    send_render_event(DITHER_MANUAL_2_PATTERN);
    main_menu = NULL;
    break;
  case 13:
    send_render_event(NO_DITHER);
    main_menu = NULL;
    break;
  }
}

imenu render_submenu[] = {
  {
    ITEM_ACTIVE,
    10,
    "DynamicA2"
  },
  {
    ITEM_ACTIVE,
    11,
    "DitherAreaPattern2Level"
  },
  {
    ITEM_ACTIVE,
    12,
    "DitherArea(2, PATTERN)"
  },
  {
    ITEM_ACTIVE,
    13,
    "No Dither"
  },
  0
};

imenu main_menu_imenu[] = {
  {
    ITEM_SUBMENU,
    1,
    "Render",
    render_submenu
  },
  {
    ITEM_ACTIVE,
    2,
    "About"
  },
  0
};


void make_main_menu() {
  icontext_menu *menu = CreateContextMenu("main_menu");
  menu->hproc = main_menu_handler;
  menu->menu = main_menu_imenu;

  main_menu = menu;
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
      } else if (b->btype == BTYPE_MENU) {
        if (pressed) {
          make_main_menu();
          OpenContextMenu(main_menu);
        }
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
  int i_menu = 9;

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

  buttons[i_menu].w = panel_height;
  buttons[i_menu].h = panel_height;
  buttons[i_menu].x = ScreenWidth() - panel_height;
  buttons[i_menu].y = 0;
}

void draw_panel() {
  DrawRect(0, 0, ScreenWidth(), panel_height, BLACK);
  DrawRect(ScreenWidth() - panel_height, 0, panel_height, panel_height, BLACK);

  DrawTextRect(0, 0, ScreenWidth(), panel_height,
    "pbdoom " GIT_TAG,
    ALIGN_CENTER | VALIGN_MIDDLE
  );
}

void draw_gui() {
  draw_buttons();
  draw_panel();
}

void setup_app() {
  SetPanelType(PANEL_DISABLED);

  ClearScreen();

  compute_button_positions();
  draw_gui();
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
