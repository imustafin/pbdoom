#ifndef PBDOOM_FRAMES_FRAME_H
#define PBDOOM_FRAMES_FRAME_H

#include "inkview.h"

typedef void (*frame_install)(int x, int y, int w, int h);
typedef void (*frame_uninstall)();
typedef void (*frame_draw)();

typedef struct {
  int x;
  int y;
  int w;
  int h;
  frame_install install;
  frame_uninstall uninstall;
  frame_draw draw;
  iv_handler handle;
} frame;

#endif /* PBDOOM_FRAMES_FRAME_H */
