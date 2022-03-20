#include <stddef.h>

#include "events.h"

#define PBDOOM_EVENTS_N 100
pbdoom_event pbdoom_events[PBDOOM_EVENTS_N];

// Index for next write
int pbdoom_events_write = 0;

// Index for next read
int pbdoom_events_read = 0;

void pbdoom_post_event(pbdoom_event e) {
  pbdoom_events[pbdoom_events_write] = e;

  pbdoom_events_write = (pbdoom_events_write + 1) % PBDOOM_EVENTS_N;
}

// Returns NULL if empty
pbdoom_event *pbdoom_poll_event() {
  if (pbdoom_events_read == pbdoom_events_write) {
    return NULL;
  }

  pbdoom_event *ans = &pbdoom_events[pbdoom_events_read];

  pbdoom_events_read = (pbdoom_events_read + 1) % PBDOOM_EVENTS_N;

  return ans;
}
