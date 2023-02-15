#ifndef PBDOOM_GUI_H
#define PBDOOM_GUI_H

void gui_init();
void gui_open_color_settings();
void gui_open_keyboard();

extern int dpi;
double dp;

typedef enum {
  size_m, // 6 inch
  size_l, // 10 inch
} gui_size_t;

extern gui_size_t gui_size;

#endif /* PBDDOM_GUI_H */
