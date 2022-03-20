
#include "sdldoom-1.10/doomdef.h"
#include "sdldoom-1.10/d_main.h"

#include "inkview.h"

#define D_DOOM_LOOP_FREQ (1000 / TICRATE)

void D_DoomLoop_iter_pbdoom() {
  D_DoomLoop_iter();

  SetWeakTimer("D_DoomLoop", D_DoomLoop_iter_pbdoom, D_DOOM_LOOP_FREQ);
}

void start_D_DoomLoop() {
  D_DoomLoop_pre();

  D_DoomLoop_iter_pbdoom();
}
