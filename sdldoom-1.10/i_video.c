// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for SDL library
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>

#include "../events.h"

#include "m_swap.h"
#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"


#include "inkview.h"

ink_render_mode i_render_mode = DYNAMIC_A2;

// Fake mouse handling.
boolean		grabMouse;

// Blocky mode,
// replace each 320x200 pixel with multiply*multiply pixels.
// According to Dave Taylor, it still is a bonehead thing
// to use ....
static int	multiply=1;


void I_ShutdownGraphics(void)
{
  fprintf(stderr, "I_ShutdownGraphics: TODO\n");
}



//
// I_StartFrame
//
void I_StartFrame (void)
{
    // er?

}

void I_GetEvent(pbdoom_event *e)
{
  event_t event;

  switch (e->type)
    {
    case PBDOOM_EVENT_KEYDOWN:
      event.type = ev_keydown;
      event.data1 = e->a;
      D_PostEvent(&event);
      break;

    case PBDOOM_EVENT_KEYUP:
      event.type = ev_keyup;
      event.data1 = e->a;
      D_PostEvent(&event);
      break;

    case PBDOOM_EVENT_SWITCH_RENDER_MODE:
      i_render_mode = e->a;
      break;

    case PBDOOM_EVENT_EXIT:
      I_Quit();
      break;
    }
}

//
// I_StartTic
//
void I_StartTic (void)
{
  pbdoom_event *e;
  while ((e = pbdoom_poll_event())) {
    I_GetEvent(e);
  }
}


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
    // what is this?
}

int ink_palette[256];

//
// I_FinishUpdate
//
void I_FinishUpdate (void)
{
  static int	lasttic;
  int		tics;
  int		i;

  // draws little dots on the bottom of the screen
  if (devparm)
    {

      i = I_GetTime();
      tics = i - lasttic;
      lasttic = i;
      if (tics > 20) tics = 20;

      for (i=0 ; i<tics*2 ; i+=2)
        screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
      for ( ; i<20*2 ; i+=2)
        screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;
    }

  unsigned char *line = screens[0];
  int k = ScreenWidth() / SCREENWIDTH; // size of one pixel
  int ox = (ScreenWidth() - (k * SCREENWIDTH)) / 2;
  FillArea(ox, 0, k * SCREENWIDTH, k * SCREENHEIGHT, WHITE);
  for (int i = 0; i < SCREENHEIGHT; i++) {
    for (int j = 0; j < SCREENWIDTH; j++) {
      FillArea(j * k + ox, i * k, k, k, ink_palette[line[j]]);
    }

    line += SCREENWIDTH;
  }
  switch (i_render_mode) {
  case DYNAMIC_A2:
    DynamicUpdateA2(ox, 0, k * SCREENWIDTH, k * SCREENHEIGHT);
    break;
  case DITHER_AREA_PATTERN_2_LEVEL:
    DitherAreaPattern2Level(ox, 0, k * SCREENWIDTH, k * SCREENHEIGHT);
    PartialUpdate(ox, 0, k * SCREENWIDTH, k * SCREENHEIGHT);
    break;
  case DITHER_MANUAL_2_PATTERN:
    DitherArea(ox, 0, k * SCREENWIDTH, k * SCREENHEIGHT, 2, DITHER_PATTERN);
    PartialUpdate(ox, 0, k * SCREENWIDTH, k * SCREENHEIGHT);
    break;
  case NO_DITHER:
    PartialUpdate(ox, 0, k * SCREENWIDTH, k * SCREENHEIGHT);
    break;
  default:
    I_Error("Unknown i_render_mode %d", i_render_mode);
  }
}


//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}


//
// I_SetPalette
//
void I_SetPalette (byte* palette)
{
  for (int i = 0; i < 256; i++) {
    byte r = gammatable[usegamma][*palette++];
    byte g = gammatable[usegamma][*palette++];
    byte b = gammatable[usegamma][*palette++];

    ink_palette[i] = (r << 16) | (g << 8) | b;
  }
}


void I_InitGraphics(void)
{
	screens[0] = (unsigned char *) malloc (SCREENWIDTH * SCREENHEIGHT);
}
