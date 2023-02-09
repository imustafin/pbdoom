#include "panel.h"
#include "version.h"
#include "../render.h"
#include "../../events.h"

icontext_menu *main_menu;

imenu render_submenu[] = {
  {
    ITEM_ACTIVE,
    10,
    "DynamicA2"
  },
  {
    ITEM_ACTIVE,
    11,
    "DitherAreaPattern2Level"
  },
  {
    ITEM_ACTIVE,
    12,
    "DitherArea(2, PATTERN)"
  },
  {
    ITEM_ACTIVE,
    13,
    "No Dither"
  },
  0
};

imenu main_menu_imenu[] = {
  {
    ITEM_SUBMENU,
    1,
    "Render",
    render_submenu
  },
  {
    ITEM_ACTIVE,
    2,
    "About"
  },
  0
};
void send_render_event(ink_render_mode mode) {
  pbdoom_event e;
  e.a = mode;
  e.b = 0;
  e.type = PBDOOM_EVENT_SWITCH_RENDER_MODE;
  pbdoom_post_event(e);
}

void main_menu_handler(int i) {
  switch (i) {
  case -1:
    main_menu = NULL;
    break;
  case 2:
    main_menu = NULL;
    Dialog(
           0,
           "About pbdoom",
           "Pbdoom " GIT_TAG "\n"
           "by Ilgiz Mustafin, based on SDL Doom by Sam Lantinga\n"
           "Get the Source code and other info at \n"
           "https://github.com/imustafin/pbdoom"
           ,
           "Ok", NULL, NULL
           );
    break;
  case 10:
    send_render_event(DYNAMIC_A2);
    main_menu = NULL;
    break;
  case 11:
    send_render_event(DITHER_AREA_PATTERN_2_LEVEL);
    main_menu = NULL;
    break;
  case 12:
    send_render_event(DITHER_MANUAL_2_PATTERN);
    main_menu = NULL;
    break;
  case 13:
    send_render_event(NO_DITHER);
    main_menu = NULL;
    break;
  }
}

void make_main_menu() {
  icontext_menu *menu = CreateContextMenu("main_menu");
  menu->hproc = main_menu_handler;
  menu->menu = main_menu_imenu;

  main_menu = menu;
}

int bx;
int by;
int bs;

void panel_frame_install(int x, int y, int w, int h) {
  panel_frame.x = x;
  panel_frame.y = y;
  panel_frame.w = w;
  panel_frame.h = h;

  bx = w - h;
  by = y;
  bs = h;
}

void panel_frame_uninstall() {
  
}

static ifont *title_font;
static void set_title_font() {
  if (!title_font) {
    title_font = OpenFont(DEFAULTFONT, 30, 0);
  }
  SetFont(title_font, BLACK);
}

static ifont *button_font;
static void set_button_font() {
  if (!button_font) {
    button_font = OpenFont("LiberationSans", 30, 0);
  }
  SetFont(button_font, BLACK);
}

void panel_frame_draw() {
  DrawRect(panel_frame.x, panel_frame.y, panel_frame.w, panel_frame.h, BLACK);

  set_title_font();
  DrawTextRect(panel_frame.x, panel_frame.y,
               panel_frame.w, panel_frame.h,
               "pbdoom " GIT_TAG,
               ALIGN_CENTER | VALIGN_MIDDLE
               );

  set_button_font();
  DrawRect(bx, by, bs, bs, BLACK);
  DrawTextRect(bx, by, bs, bs, "☰", ALIGN_CENTER | VALIGN_MIDDLE);
}

static int handle(int t, int a, int b) {
  if (t == EVT_POINTERUP) {
    if (a >= bx && a <= bx + bs && b >= by && b <= by + bs) {
      make_main_menu();
      OpenContextMenu(main_menu);
      return true;
    }
  }
  return false;
}

frame panel_frame = {
  .install = *panel_frame_install,
  .uninstall = *panel_frame_uninstall,
  .draw = *panel_frame_draw,
  .handle = *handle
};