#include <math.h>

#include "keyboard.h"
#include "../render.h"
#include "../gui.h"

#include "../../events.h"

typedef struct {
  int doom_code;
  char *title;
  int modes;
  int x;
  int y;
  int w;
  int h;
  int pressed;
} button_t;

enum {
  button_up = 0,
  button_down,
  button_left,
  button_right,
  button_enter,
  button_y,
  button_n,
  button_ctrl,
  button_space,
  button_one,
  button_two,
  button_three,
  button_four,
  button_five,
  button_six,
  button_seven,
  button_turn_left,
  button_turn_right,
  BUTTONS_N
} button_i;


keyboard_mode current_mode = keyboard_mode_none;

const keyboard_mode joystick_modes = keyboard_mode_level;
int jx, jy, jw, jh; // joystick panel position
int jcx, jcy; // center of joystick extreme circle
int jr; // radius of joystick extremes
int jmti; // currently grabbed joystick MT index, -1 for no grabbed

button_t buttons[BUTTONS_N] = {
  {KEY_UPARROW, "↑", keyboard_mode_menu},
  {KEY_DOWNARROW, "↓", keyboard_mode_menu},
  {KEY_LEFTARROW, "←", keyboard_mode_menu},
  {KEY_RIGHTARROW, "→", keyboard_mode_menu},
  {KEY_ENTER, "ENTER", keyboard_mode_menu},
  {'y', "Y", keyboard_mode_message_input},
  {'n', "N", keyboard_mode_message_input},
  {KEY_RCTRL, "CTRL", keyboard_mode_level},
  {' ', "␣", keyboard_mode_level},
  {'1', "1", keyboard_mode_level},
  {'2', "2", keyboard_mode_level},
  {'3', "3", keyboard_mode_level},
  {'4', "4", keyboard_mode_level},
  {'5', "5", keyboard_mode_level},
  {'6', "6", keyboard_mode_level},
  {'7', "7", keyboard_mode_level},
  {KEY_LEFTARROW, "←", keyboard_mode_level},
  {KEY_RIGHTARROW, "→", keyboard_mode_level}
};

void compute_button_positions() {
  int sw = keyboard_frame.w;

  int b;
  int ox;
  if (gui_size > size_m) {
    b = 70 * dp;
    ox = b;
  } else {
    b = 60 * dp;
    ox = 5 * dp;
  }

  // content height is 3 buttons
  int ch = 3 * b;

  // buttons valign middle
  int ym = (keyboard_frame.h - ch) / 2;
  int y = keyboard_frame.y;

  buttons[button_up].x = ox + b - 1;
  buttons[button_up].y = y + ym;
  buttons[button_up].w = b;
  buttons[button_up].h = b;

  buttons[button_left].x = ox + 0;
  buttons[button_left].y = y + ym + b - 1;
  buttons[button_left].w = b;
  buttons[button_left].h = b;

  buttons[button_right].x = ox + (2 * b) - 2;
  buttons[button_right].y = y + ym + b - 1;
  buttons[button_right].w = b;
  buttons[button_right].h = b;

  buttons[button_down].x = ox + b - 1;
  buttons[button_down].y = y + ym + 2 * b - 2;
  buttons[button_down].w = b;
  buttons[button_down].h = b;

  buttons[button_enter].w = 3 * b;
  buttons[button_enter].h = 3 * b;
  buttons[button_enter].x = sw - ox - buttons[button_enter].w;
  buttons[button_enter].y = y + ym;

  buttons[button_y].x = ox + (4 * b);
  buttons[button_y].y = y + ym;
  buttons[button_y].w = b;
  buttons[button_y].h = b;

  buttons[button_n].x = ox + (5 * b);
  buttons[button_n].y = y + ym;
  buttons[button_n].w = b;
  buttons[button_n].h = b;

  buttons[button_ctrl].w = 3 * b;
  buttons[button_ctrl].h = 1 * b;
  buttons[button_ctrl].x = sw - ox - buttons[button_ctrl].w;
  buttons[button_ctrl].y = y + ym + b;

  buttons[button_turn_right].w = 1.5 * b;
  buttons[button_turn_right].h = b;
  buttons[button_turn_right].x = sw - ox - buttons[button_turn_right].w;
  buttons[button_turn_right].y = y + ym + 1;

  buttons[button_turn_left].w = 1.5 * b + 1;
  buttons[button_turn_left].h = b;
  buttons[button_turn_left].x = buttons[button_turn_right].x - buttons[button_turn_left].w + 1;
  buttons[button_turn_left].y = y + ym + 1;


  buttons[button_space].w = 3 * b;
  buttons[button_space].h = b;
  buttons[button_space].x = sw - ox - buttons[button_space].w;
  buttons[button_space].y = y + ym + 2 * b - 1;

  int nb;
  if (gui_size > size_m) {
    nb = 60 * dp;
  } else {
    nb = 48 * dp;
  }
  int cwb = 3 * nb - 2;
  int chb = 3 * nb - 2;
  int wb = buttons[button_ctrl].x - (buttons[button_right].x + b);
  int onx = buttons[button_right].x + buttons[button_right].w + (wb - cwb) / 2;
  int ony = y + (keyboard_frame.h - chb) / 2;
  int hb = sw;
  for(button_t *n = buttons + button_two; n <= buttons + button_seven; n++) {
    n->w = nb;
    n->h = nb;
    n->x = onx + ((n - buttons - button_two) % 3) * (nb - 1);
    n->y = ony + ((n - buttons - button_two) / 3) * (nb - 1);
  }
  buttons[button_one].w = nb;
  buttons[button_one].h = nb;
  buttons[button_one].x = onx + 1 * (nb - 1);
  buttons[button_one].y = ony + 2 * (nb - 1);
}

void compute_joystick_position() {
  jx = keyboard_frame.x;
  jy = keyboard_frame.y;
  jw = buttons[button_two].x - jx - 10 * dp;
  jh = keyboard_frame.h;
  jr = 75 * dp;
  jcx = jx + jw / 2;
  jcy = jy + jh / 2;
}

void keyboard_frame_install(int x, int y, int w, int h) {
  keyboard_frame.x = x;
  keyboard_frame.y = y;
  keyboard_frame.w = w;
  keyboard_frame.h = h;

  compute_button_positions();
  compute_joystick_position();
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

void draw_joystick_panel() {
  DrawTextRect(jx, jy, jw, jh, "Movement panel", ALIGN_CENTER | VALIGN_BOTTOM);
  DrawLine(jx + jw, jy, jx + jw, jy + jh, BLACK);
  DrawCircle(jcx, jcy, jr, BLACK);
  DrawCircle(jcx, jcy, jr - 2, WHITE);
}

static void draw() {
  set_font();
  for (int i = 0; i < BUTTONS_N; i++) {
    button_t *b = &buttons[i];
    if (current_mode & b->modes) {
      FillArea(b->x, b->y, b->w, b->h, WHITE);
      DrawTextRect(b->x, b->y, b->w, b->h, b->title, ALIGN_CENTER | VALIGN_MIDDLE);
      DrawRect(b->x, b->y, b->w, b->h, BLACK);
    }
  }
  if (current_mode & joystick_modes) {
    draw_joystick_panel();
  }
  DrawRect(keyboard_frame.x, keyboard_frame.y, keyboard_frame.w, keyboard_frame.h, BLACK);
}

void set_pressed(int i, boolean pressed) {
  button_t *b = &buttons[i];

  b->pressed = pressed;

  int e_type;
  pbdoom_event e;
  e.a = b->doom_code;
  e.b = 0;
  e.type = pressed ? PBDOOM_EVENT_KEYDOWN : PBDOOM_EVENT_KEYUP;

  pbdoom_post_event(e);
}

int handle_no_joy_grabbed(iv_mtinfo *mt_all, int count) {
  for (int i = 0; i < count; i++) {
    iv_mtinfo *mt = mt_all + i;

    int x = mt->x;
    int y = mt->y;

    if (x >= jx && y >= jy && x <= jx + jw && y <= jy + jh) {
      jmti = i;
      return i;
    }
  }

  return -1;
}

int last_dx, last_dy;

void send_joy_event(int dx, int dy) {
  if (dx == last_dx && dy == last_dy) {
    return;
  }
  last_dx = dx;
  last_dy = dy;
  pbdoom_event e;
  e.type = PBDOOM_EVENT_JOY;
  e.b = dx;
  e.c = dy;
  pbdoom_post_event(e);
}

void handle_joy_grabbed(iv_mtinfo *joy_mt) {
  int x = joy_mt->x;
  int y = joy_mt->y;

  double dx = x - jcx;
  double dy = y - jcy;
  double len = sqrt(dx * dx + dy * dy);

  if (len > jr) {
    double k = jr / len;
    dx *= k;
    dy *= k;
  }

  send_joy_event(dx / jr * 100, -dy / jr * 100);
}

int handle_joystick_panel(iv_mtinfo *mt_all, int count) {
  if (jmti >= 0) {
    if (count == 0 || !((mt_all + jmti)->active)) {
      jmti = -1;
      send_joy_event(0, 0);
      return -1;
    } else {
      handle_joy_grabbed(mt_all + jmti);
      return jmti;
    }
  } else {
    return handle_no_joy_grabbed(mt_all, count);
  }
}

static int handle(int t, int index, int cnt) {
  if (t != EVT_MTSYNC) {
    return false;
  }

  iv_mtinfo *mt_all = GetTouchInfoI(index);
  int joystick_handled_mt = -1;

  if ((current_mode & joystick_modes)) {
    joystick_handled_mt = handle_joystick_panel(mt_all, cnt);
  }

  int presses[BUTTONS_N];
  for (int i = 0; i < BUTTONS_N; i++) {
    presses[i] = 0;
  }


  for (int i = 0; i < cnt; i++) {
    if (i == joystick_handled_mt) {
      continue;
    }

    iv_mtinfo *mt = mt_all + i;

    if (!mt->active) {
      continue;
    }

    int x = mt->x;
    int y = mt->y;

    for (int j = 0; j < BUTTONS_N; j++) {
      button_t *b = &buttons[j];

      if (!(b->modes & current_mode)) {
        continue;
      }

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
      set_pressed(i, pressed);
    }
  }

  return true;
}


void unpress_not_in_mode() {
  for (int i = 0; i < BUTTONS_N; i++) {
    button_t *b = &buttons[i];

    if (!(b->modes & current_mode)) {
      set_pressed(i, false);
    }
  }
}

void redraw() {
  FillArea(keyboard_frame.x, keyboard_frame.y, keyboard_frame.w, keyboard_frame.h, WHITE);
  draw();
  PartialUpdate(keyboard_frame.x, keyboard_frame.y, keyboard_frame.w, keyboard_frame.h);
}

void set_keyboard_mode(keyboard_mode mode) {
  if (current_mode != mode) {
    current_mode = mode;
    unpress_not_in_mode();
    redraw();
  }
}


frame keyboard_frame = {
  .install = *keyboard_frame_install,
  .uninstall = *keyboard_frame_uninstall,
  .draw = *draw,
  .handle = *handle
};
