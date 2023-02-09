#ifndef PBDOOM_RENDER_H
#define PBDOOM_RENDER_H

#include "inkview.h"

#include "../sdldoom-1.10/doomtype.h"
#include "../sdldoom-1.10/doomdef.h"

#include "frames/game.h"


void pbdoom_draw(unsigned char *screen);
void pbdoom_set_palette(byte *palette);
void pbdoom_render_set_mode(ink_render_mode mode);

#endif /* PBDOOM_RENDER_H */
