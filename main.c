#include "inkview.h"

#include "sdldoom-1.10/i_main.h"

char **new_argv;
int new_argc;

static int main_handler(int event_type, int param_one, int param_two)
{
    if (EVT_INIT == event_type) {
        ClearScreen();
        doom_main(new_argc, new_argv);
    }
    else if (EVT_KEYPRESS == event_type) {
        CloseApp();
    }
    return 0;
}


int main (int argc, char* argv[])
{
  new_argv = malloc((argc + 3) * sizeof(*new_argv));
  memmove(new_argv, argv, sizeof(*new_argv) * argc);
  new_argv[argc] = "-files";
  new_argv[argc + 1] = "/mnt/ext1/DOOM2.WAD";
  new_argv[argc + 2] = 0;
  new_argc = argc + 2;

  InkViewMain(main_handler);

  return 0;
}
