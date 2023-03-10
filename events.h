#ifndef __PBDOOM_EVENTS_H__
#define __PBDOOM_EVENTS_H__

typedef enum {
  PBDOOM_EVENT_KEYDOWN,
  PBDOOM_EVENT_KEYUP,
  PBDOOM_EVENT_EXIT,
  PBDOOM_EVENT_SWITCH_RENDER_MODE,
  PBDOOM_EVENT_JOY,
  PBDOOM_EVENT_MOUSE
} pbdoom_event_type;

typedef struct {
  pbdoom_event_type type;
  int a;
  int b;
  int c;
} pbdoom_event;

void pbdoom_post_event(pbdoom_event e);

pbdoom_event *pbdoom_poll_event();

#endif
