#ifndef __PBDOOM_EVENTS_H__
#define __PBDOOM_EVENTS_H__

typedef enum {
  PBDOOM_EVENT_KEYDOWN,
  PBDOOM_EVENT_KEYUP,
  PBDOOM_EVENT_EXIT
} pbdoom_event_type;

typedef struct {
  pbdoom_event_type type;
  int a;
  int b;
} pbdoom_event;

void pbdoom_post_event(pbdoom_event e);

pbdoom_event *pbdoom_poll_event();

#endif
