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

// PBDoom: TODO
// SDL_Surface *screen;

// Fake mouse handling.
boolean		grabMouse;

// Blocky mode,
// replace each 320x200 pixel with multiply*multiply pixels.
// According to Dave Taylor, it still is a bonehead thing
// to use ....
static int	multiply=1;


//
//  Translates the key 
//

int xlatekey(/* SDL_keysym *key */)
{
  fprintf(stderr, "xlatekey: TODO\n");
  return 0;

#if 0
    int rc;

    switch(key->sym)
    {
      case SDLK_LEFT:	rc = KEY_LEFTARROW;	break;
      case SDLK_RIGHT:	rc = KEY_RIGHTARROW;	break;
      case SDLK_DOWN:	rc = KEY_DOWNARROW;	break;
      case SDLK_UP:	rc = KEY_UPARROW;	break;
      case SDLK_ESCAPE:	rc = KEY_ESCAPE;	break;
      case SDLK_RETURN:	rc = KEY_ENTER;		break;
      case SDLK_TAB:	rc = KEY_TAB;		break;
      case SDLK_F1:	rc = KEY_F1;		break;
      case SDLK_F2:	rc = KEY_F2;		break;
      case SDLK_F3:	rc = KEY_F3;		break;
      case SDLK_F4:	rc = KEY_F4;		break;
      case SDLK_F5:	rc = KEY_F5;		break;
      case SDLK_F6:	rc = KEY_F6;		break;
      case SDLK_F7:	rc = KEY_F7;		break;
      case SDLK_F8:	rc = KEY_F8;		break;
      case SDLK_F9:	rc = KEY_F9;		break;
      case SDLK_F10:	rc = KEY_F10;		break;
      case SDLK_F11:	rc = KEY_F11;		break;
      case SDLK_F12:	rc = KEY_F12;		break;
	
      case SDLK_BACKSPACE:
      case SDLK_DELETE:	rc = KEY_BACKSPACE;	break;

      case SDLK_PAUSE:	rc = KEY_PAUSE;		break;

      case SDLK_EQUALS:	rc = KEY_EQUALS;	break;

      case SDLK_KP_MINUS:
      case SDLK_MINUS:	rc = KEY_MINUS;		break;

      case SDLK_LSHIFT:
      case SDLK_RSHIFT:
	rc = KEY_RSHIFT;
	break;
	
      case SDLK_LCTRL:
      case SDLK_RCTRL:
	rc = KEY_RCTRL;
	break;
	
      case SDLK_LALT:
      case SDLK_LMETA:
      case SDLK_RALT:
      case SDLK_RMETA:
	rc = KEY_RALT;
	break;
	
      default:
        rc = key->sym;
	break;
    }

    return rc;

#endif

}

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
  int k = 4; // size of one pixel
  FillArea(0, 0, k * SCREENWIDTH, k * SCREENHEIGHT, WHITE);
  for (int i = 0; i < SCREENHEIGHT; i++) {
    for (int j = 0; j < SCREENWIDTH; j++) {
      FillArea(j * k, i * k, k, k, ink_palette[line[j]]);
    }

    line += SCREENWIDTH;
  }
  DitherAreaPattern2Level(0, 0, k * SCREENWIDTH, k * SCREENHEIGHT);
  PartialUpdate(0, 0, k * SCREENWIDTH, k * SCREENHEIGHT);
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
  fprintf(stderr, "I_InitGraphics: TODO\n");

#if 0
    static int	firsttime=1;
    Uint16 video_w, video_h, w, h;
    Uint8 video_bpp;
    Uint32 video_flags;

    if (!firsttime)
	return;
    firsttime = 0;

    video_flags = (SDL_SWSURFACE|SDL_HWPALETTE);
    if (!!M_CheckParm("-fullscreen"))
        video_flags |= SDL_FULLSCREEN;

    if (M_CheckParm("-2"))
	multiply = 2;

    if (M_CheckParm("-3"))
	multiply = 3;

    // check if the user wants to grab the mouse (quite unnice)
    grabMouse = !!M_CheckParm("-grabmouse");

    video_w = w = SCREENWIDTH * multiply;
    video_h = h = SCREENHEIGHT * multiply;
    video_bpp = 8;

    /* We need to allocate a software surface because the DOOM! code expects
       the screen surface to be valid all of the time.  Properly done, the
       rendering code would allocate the video surface in video memory and
       then call SDL_LockSurface()/SDL_UnlockSurface() around frame rendering.
       Eventually SDL will support flipping, which would be really nice in
       a complete-frame rendering application like this.
    */
    switch (video_w/w) {
        case 3:
            multiply *= 3;
            break;
        case 2:
            multiply *= 2;
            break;
        case 1:
            multiply *= 1;
            break;
        default:
		;
    }
    if ( multiply > 3 ) {
        I_Error("Smallest available mode (%dx%d) is too large!",
						video_w, video_h);
    }
    screen = SDL_SetVideoMode(video_w, video_h, 8, video_flags);
    if ( screen == NULL ) {
        I_Error("Could not set %dx%d video mode: %s", video_w, video_h,
							SDL_GetError());
    }
    SDL_ShowCursor(0);
    SDL_WM_SetCaption("SDL DOOM! v1.10", "doom");

    /* Set up the screen displays */
    w = SCREENWIDTH * multiply;
    h = SCREENHEIGHT * multiply;
    if (multiply == 1 && !SDL_MUSTLOCK(screen) ) {
	screens[0] = (unsigned char *) screen->pixels;
    } else {
	screens[0] = (unsigned char *) malloc (SCREENWIDTH * SCREENHEIGHT);
        if ( screens[0] == NULL )
            I_Error("Couldn't allocate screen memory");
    }
#endif
}
