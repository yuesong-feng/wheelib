#include "event.h"

int main(int argc, char const *argv[]) {
  event_t event;
  event_init(&event);
  event_timedwait(&event, 5 * USEC_PER_SEC);
  event_destroy(&event);
  return 0;
}
