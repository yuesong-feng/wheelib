#include "sema.h"

int main(int argc, char const *argv[]) {

  sema_t sema;
  sema_init(&sema, 1);
  sema_post(&sema);
  sema_timedwait(&sema, 1 * USEC_PER_SEC);
  sema_timedwait(&sema, 1 * USEC_PER_SEC);
  sema_timedwait(&sema, 1 * USEC_PER_SEC);
  sema_destroy(&sema);
  return 0;
}
