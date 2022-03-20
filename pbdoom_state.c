
#include "sdldoom-1.10/doomdef.h"
#include "sdldoom-1.10/d_main.h"
#include "sdldoom-1.10/i_system.h"

#include "inkview.h"

#define D_DOOM_LOOP_PERIOD (1000 / TICRATE)

void D_DoomLoop_iter_pbdoom() {
  long start = I_GetTimeMS();

  D_DoomLoop_iter();

  long end = I_GetTimeMS();

  long next = D_DOOM_LOOP_PERIOD - (end - start);
  if (next < 1) {
    next = 1;
  }

  SetWeakTimer("D_DoomLoop", D_DoomLoop_iter_pbdoom, next);
}

void start_D_DoomLoop() {
  D_DoomLoop_pre();

  D_DoomLoop_iter_pbdoom();
}
