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
#include "../pbdoom/render.h"

#include "m_swap.h"
#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

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

    case PBDOOM_EVENT_JOY:
      event.type = ev_joystick;
      event.data2 = e->b;
      event.data3 = e->c;
      D_PostEvent(&event);
      break;

    case PBDOOM_EVENT_MOUSE:
      event.type = ev_mouse;
      event.data2 = e->b;
      event.data3 = e->c;
      D_PostEvent(&event);
      break;

    case PBDOOM_EVENT_SWITCH_RENDER_MODE:
      pbdoom_render_set_mode(e->a);
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

  pbdoom_draw(screens[0]);
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
  pbdoom_set_palette(palette);
}


void I_InitGraphics(void)
{
	screens[0] = (unsigned char *) malloc (SCREENWIDTH * SCREENHEIGHT);
}
