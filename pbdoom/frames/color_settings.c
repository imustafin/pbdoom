#include <math.h>

#include "color_settings.h"
#include "../gui.h"
#include "game.h"

static int x, y, w, h;
static int rx, ry, rw, rh; // reset button

static const double MIN_ALPHA = -255;
static const double MAX_ALPHA = 255;
static const double MIN_BETA = 0;
static const double MAX_BETA = 10;
static const double MIN_GAMMA = -10;
static const double MAX_GAMMA = 10;

typedef struct {
  double *val;
  double min;
  double max;
  double step;
  int x;
  int y;
  int s;
  int mx;
  int my;
  int px;
  int py;
  int tx;
  int ty;
  int tw;
  int th;
} value;

const int V_S = 50; // how many dp is value button

const int I_ALPHA = 0;
const int I_BETA = 1;
const int I_GAMMA = 2;
value values[] = {
  {
    &game_alpha,
    0,
    255,
    1
  },
  {
    &game_beta,
    -255,
    255,
    10
  },
  {
    &game_gamma,
    0.1,
    5,
    0.1
  }
};

// v->x and v->y should be set
void compute_value(value *v) {
  int m = 8 * dp;

  int s = V_S * dp;
  v->s = s;

  int xx = v->x;
  int yy = v->y;

  v->mx = xx;
  v->my = yy;

  xx += s + m;

  v->px = xx;
  v->py = yy;

  xx += s + m;

  v->tx = xx;
  v->ty = yy;
  v->tw = 100 * dp;
  v->th = s;
}

int okx, oky, okw, okh;

static void install(int a_x, int a_y, int a_w, int a_h) {
  x = a_x;
  y = a_y;
  w = a_w;
  h = a_h;

  color_settings_frame.x = x;
  color_settings_frame.y = y;
  color_settings_frame.w = w;
  color_settings_frame.h = h;

  int xx = x + 5 * dp;
  int yy = y + 5 * dp;
  int m = 12 * dp;

  values[I_ALPHA].x = xx;
  values[I_ALPHA].y = yy;
  compute_value(values + I_ALPHA);
  yy += m + values[I_ALPHA].s;

  values[I_BETA].x = xx;
  values[I_BETA].y = yy;
  compute_value(values + I_BETA);
  yy += m + values[I_BETA].s;

  values[I_GAMMA].x = xx;
  values[I_GAMMA].y = yy;
  compute_value(values + I_GAMMA);
  yy += m + values[I_GAMMA].s;

  okx = xx;
  oky = yy;
  okw = 100 * dp;
  okh = 48 * dp;
}

static void uninstall() {
  
}

const int NUM_BUF_S = 10;
char num_buf[NUM_BUF_S];

void draw_value_text(value *v) {
  snprintf(num_buf, NUM_BUF_S, "%.1f", *(v->val));
  DrawTextRect(v->tx, v->ty, v->tw, v->th, num_buf, ALIGN_LEFT | VALIGN_MIDDLE);
}
static void draw_value(value *v) {
  DrawRect(v->mx, v->my, v->s, v->s, BLACK);
  DrawTextRect(v->mx, v->my, v->s, v->s, "-", ALIGN_CENTER | VALIGN_MIDDLE);

  DrawRect(v->px, v->py, v->s, v->s, BLACK);
  DrawTextRect(v->px, v->py, v->s, v->s, "+", ALIGN_CENTER | VALIGN_MIDDLE);

  draw_value_text(v);
}

int handle_value(value *v, int x, int y) {
  boolean changed = false;

  if (x >= v->mx && x <= v->mx + v->s && y >= v->my && y <= v->my + v->s) {
    if (*(v->val) - v->step >= v->min) {
      *(v->val) -= v->step;
      changed = true;
    }
  } else if (x >= v->px && x <= v->px + v->s && y >= v->py && y <= v->py + v->s) {
    if (*(v->val) + v->step <= v->max) {
      *(v->val) += v->step;
      changed = true;
    }
  }
  if (changed) {
    FillArea(v->tx, v->ty, v->tw, v->th, WHITE);
    draw_value_text(v);
    PartialUpdate(v->tx, v->ty, v->tw, v->th);
    return true;
  } else {
    return false;
  }
}

static void draw() {
  DrawRect(x, y, w, h, BLACK);
  draw_value(values + I_ALPHA);
  draw_value(values + I_BETA);
  draw_value(values + I_GAMMA);

  DrawRect(okx, oky, okw, okh, BLACK);
  DrawTextRect(okx, oky, okw, okh, "Ok", ALIGN_CENTER | VALIGN_MIDDLE);
}

static int handle(int t, int a, int b) {
  if (t == EVT_POINTERUP) {
    if (a >= okx && a <= okx + okw && b >= oky && b <= oky + okh) {
      gui_open_keyboard();
      return true;
    }
    return handle_value(values + I_ALPHA, a, b) ||
      handle_value(values + I_BETA, a, b) ||
      handle_value(values + I_GAMMA, a, b);
  }
  return false;
}

frame color_settings_frame = {
  .install = *install,
  .uninstall = *uninstall,
  .draw = *draw,
  .handle = *handle
};
