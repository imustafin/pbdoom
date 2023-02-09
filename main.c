#include "inkview.h"

#include "events.h"
#include "pbdoom/render.h"
#include "pbdoom/gui.c"

#include "sdldoom-1.10/i_main.h"
#include "sdldoom-1.10/doomdef.h"



char **new_argv;
int new_argc;

void setup_app() {
  SetPanelType(PANEL_DISABLED);

  ClearScreen();

  gui_init();
  FullUpdate();
}

static int main_handler(int event_type, int a, int b)
{
  if (EVT_INIT == event_type) {
    setup_app();

    doom_main(new_argc, new_argv);
  }
  else if (EVT_KEYPRESS == event_type) {
    CloseApp();
  }
  else if (EVT_EXIT == event_type) {
    pbdoom_event e = { PBDOOM_EVENT_EXIT, 0, 0 };
    pbdoom_post_event(e);
    return 1; // we will exit ourselves on next tic
  } else {
    return gui_handle(event_type, a, b);
  }
  return 0;
}



int main (int argc, char* argv[])
{
  new_argc = argc;
  new_argv = argv;

  InkViewMain(main_handler);

  return 0;
}
