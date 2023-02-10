#include <math.h>

#include "color_settings.h"
#include "../gui.h"
#include "game.h"

static int x, y, w, h;

static const double MIN_ALPHA = -255;
static const double MAX_ALPHA = 255;
static const double MIN_BETA = 0;
static const double MAX_BETA = 10;
static const double MIN_GAMMA = -10;
static const double MAX_GAMMA = 10;

typedef struct {
  double *val;
  char *name;
  const double def;
  double min;
  double max;
  double step;
  double initial; // value when installed
  int x, y, w; // wisget position
  int s; // button size
  int nx, ny, nw, nh; // name
  int mx, my; // minus button
  int px, py; // plus button
  int tx, ty, tw, th; // text
  int gx, gy, gw, gh; // gauge
  int dx, dy, dw, dh; // tick for default value
  int hx, hy, hw, hh; // value pick handle (touch target) button
} value;

const int I_ALPHA = 0;
const int I_BETA = 1;
const int I_GAMMA = 2;
value values[] = {
  {
    &game_alpha,
    "Alpha",
    GAME_DEFAULT_ALPHA,
    0,
    10,
    1
  },
  {
    &game_beta,
    "Beta",
    GAME_DEFAULT_BETA,
    -100,
    100,
    10
  },
  {
    &game_gamma,
    "Gamma",
    GAME_DEFAULT_GAMMA,
    0.1,
    5,
    0.1
  }
};

int value_to_screen(value *v, double a) {
  double len = v->max - v->min;
  double filled_ratio = (a - v->min) / len;
  return (v->gx + filled_ratio * v->gw);
}

void update_value_h_pos(value *v) {
  v->hx = value_to_screen(v, *(v->val)) - v->hw / 2;
}

void update_value_from_screen(value *v, int x) {
  x += v->hw / 2;
  x -= v->gx;
  double filled_ratio = (double) x / v->gw;
  double len = v->max - v->min;
  double val = filled_ratio * len + v->min;
  if (val < v->min) {
    val = v->min;
  }
  if (val > v->max) {
    val = v->max;
  }
  *(v->val) = val;
}

const int V_S = 50; // height of value element in dps

// v->x and v->y should be set
void install_value(value *v) {
  v->initial = *(v->val);
  int m = 8 * dp;

  int s = V_S * dp;
  v->s = s;

  int ww = v->w;

  v->nx = v->x + m;
  v->ny = v->y;
  v->nw = 125 * dp;
  v->nh = s;

  v->gx = v->nx + v->nw + m + m + s / 2;
  ww -= v->nw + m + m + m + s;

  // from right to left
  int xx = v->x + v->w;
  v->tw = 75 * dp;
  v->tx = xx - v->tw;
  v->ty = v->y;
  v->th = s;

  xx -= v->tw + m;
  ww -= v->tw + m;

  v->mx = xx - s;
  v->my = v->y;

  xx -= s + m;
  ww -= s + m;

  v->px = xx - s;
  v->py = v->y;

  ww -= s + 2 * m;

  int gg = 2 * dp; // gauge line thickness
  int gp = (s - gg) / 2; // gauge line padding
  v->gw = ww;
  v->gy = v->y + gp;
  v->gh = gg;

  v->hy = v->y;
  v->hw = s;
  v->hh = s;
  update_value_h_pos(v);

  v->dw = gg;
  v->dx = value_to_screen(v, v->def) - v->dw / 2;
  v->dh = 8 * dp;
  v->dy = v->y + (s - v->dh) / 2;
}

int rx, ry, rw, rh; // reset button
int okx, oky, okw, okh; // ok button
int cx, cy, cw, ch; // cancel button

int vvx, vvy, vvh, vvw; // all values together

static void install(int a_x, int a_y, int a_w, int a_h) {
  x = a_x;
  y = a_y;
  w = a_w;
  h = a_h;

  color_settings_frame.x = x;
  color_settings_frame.y = y;
  color_settings_frame.w = w;
  color_settings_frame.h = h;

  int bh = 100 * dp;

  cx = x;
  cw = w / 3;
  ch = bh;
  cy = y + h - bh;

  rx = x + w / 3;
  rw = w / 3;
  rh = bh;
  ry = y + h - bh;

  okx = x + 2 * w / 3;
  oky = y + h - bh;
  okw = w / 3;
  okh = bh;

  int m = 12 * dp;
  int hh = h - bh;
  int vsh = 3 * V_S * dp + 2 * m;
  int mvs = (hh - vsh) / 2;

  int xx = x + 5 * dp;
  int yy = y + mvs;
  vvx = xx;
  vvy = yy;
  vvw = w - 20 * dp;
  vvh = vsh;

  for(int i = I_ALPHA; i <= I_GAMMA; i++) {
    value *v = values + i;

    v->x = xx;
    v->y = yy;
    v->w = vvw;
    install_value(v);
    yy += m + v->s;
  }
}

static void uninstall() {
  
}

const int NUM_BUF_S = 10;
char num_buf[NUM_BUF_S];

void draw_value_text(value *v) {
  snprintf(num_buf, NUM_BUF_S, "%.1f", *(v->val));
  DrawTextRect(v->tx, v->ty, v->tw, v->th, num_buf, ALIGN_RIGHT | VALIGN_MIDDLE);
}

void redraw_gauge(value *v) {
  FillArea(v->gx - v->hw / 2, v->hy, v->gw + v->hw, v->hh, WHITE);
  FillArea(v->gx, v->gy, v->gw, v->gh, BLACK);

  DrawRect(v->hx, v->hy, v->hw, v->hh, BLACK);

  FillArea(v->dx, v->dy, v->dw, v->dh, BLACK);
}

static void draw_value(value *v) {
  DrawTextRect(v->nx, v->ny, v->nw, v->nh, v->name, ALIGN_LEFT | VALIGN_MIDDLE);

  redraw_gauge(v);

  DrawRect(v->mx, v->my, v->s, v->s, BLACK);
  DrawTextRect(v->mx, v->my, v->s, v->s, "-", ALIGN_CENTER | VALIGN_MIDDLE);

  DrawRect(v->px, v->py, v->s, v->s, BLACK);
  DrawTextRect(v->px, v->py, v->s, v->s, "+", ALIGN_CENTER | VALIGN_MIDDLE);

  draw_value_text(v);
}

void redraw_value(value *v) {
  FillArea(v->tx, v->ty, v->tw, v->th, WHITE);
  update_value_h_pos(v);
  draw_value_text(v);
  redraw_gauge(v);
  int ux = v->gx - v->hw;
  int uy = v->hy;
  int uw = (v->tx + v->tw) - ux + v->hw;
  int uh = v->hh;
  if (IsInA2Update()) {
    DynamicUpdateA2(ux, uy, uw, uh);
  } else {
    PartialUpdate(ux, uy, uw, uh);
  }
}

value *dragged_value;

void timer_redraw(void *value) {
  redraw_value(value);
}

int handle_value_drag(value *v, int t, int x, int y) {
  if (t == EVT_POINTERDOWN && !dragged_value
      && x >= v->hx && x <= v->hx + v->hw && y >= v->hy && y <= v->hy + v->hh) {
    dragged_value = v;
    return true;
  }
  if (t == EVT_POINTERUP && dragged_value == v) {
    dragged_value = NULL;
    return true;
  }
  if (t == EVT_POINTERDRAG && dragged_value == v) {
    double old = *(v->val);
    update_value_from_screen(v, x);
    if (old != *(v->val)) {
      SetWeakTimerEx("color_settings_redraw_value", &timer_redraw, dragged_value, 5);
    }
    return true;
  }

  return false;
}


// Handle minus/plus button presses of value
int handle_value_minus_plus(value *v, int x, int y) {
  if (x >= v->mx && x <= v->mx + v->s && y >= v->my && y <= v->my + v->s) {
    if (*(v->val) - v->step >= v->min) {
      *(v->val) -= v->step;
    } else {
      return true;
    }
  } else if (x >= v->px && x <= v->px + v->s && y >= v->py && y <= v->py + v->s) {
    if (*(v->val) + v->step <= v->max) {
      *(v->val) += v->step;
    } else {
      return true;
    }
  } else {
    return false;
  }

  redraw_value(v);
  return true;
}

void draw_values() {
  for(int i = I_ALPHA; i <= I_GAMMA; i++) {
    draw_value(values + i);
  }
}

static void draw() {
  DrawRect(x, y, w, h, BLACK);
  draw_values();

  DrawRect(rx, ry, rw, rh, BLACK);
  DrawTextRect(rx, ry, rw, rh, "Reset to default", ALIGN_CENTER | VALIGN_MIDDLE);

  DrawRect(okx, oky, okw, okh, BLACK);
  DrawTextRect(okx, oky, okw, okh, "Save", ALIGN_CENTER | VALIGN_MIDDLE);

  DrawRect(cx, cy, cw, ch, BLACK);
  DrawTextRect(cx, cy, cw, ch, "Cancel", ALIGN_CENTER | VALIGN_MIDDLE);
}

void reset() {
  for (int i = I_ALPHA; i <= I_GAMMA; i++) {
    value *v = values + i;
    *(v->val) = v->def;
    update_value_h_pos(v);
  }
  FillArea(vvx, vvy, vvw, vvh, WHITE);
  draw_values();
  PartialUpdate(vvx, vvy, vvw, vvh);
}

void cancel() {
  for (int i = I_ALPHA; i <= I_GAMMA; i++) {
    value *v = values + i;
    *(v->val) = v->initial;
  }
  gui_open_keyboard();
}

static int handle(int t, int a, int b) {
  boolean handled = handle_value_drag(values + I_ALPHA, t, a, b) ||
    handle_value_drag(values + I_BETA, t, a, b) ||
    handle_value_drag(values + I_GAMMA, t, a, b);

  if (t == EVT_POINTERUP) {
    if (a >= okx && a <= okx + okw && b >= oky && b <= oky + okh) {
      gui_open_keyboard();
      return true;
    }
    if (a >= rx && a <= rx + rw && b >= ry && b <= ry + rh) {
      reset();
      return true;
    }
    if (a >= cx && a <= cx + cw && b >= cy && b <= cy + ch) {
      cancel();
      return true;
    }
    return handle_value_minus_plus(values + I_ALPHA, a, b) ||
      handle_value_minus_plus(values + I_BETA, a, b) ||
      handle_value_minus_plus(values + I_GAMMA, a, b);
  }
  return false;
}

frame color_settings_frame = {
  .install = *install,
  .uninstall = *uninstall,
  .draw = *draw,
  .handle = *handle
};
