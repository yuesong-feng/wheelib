#include "rwlock.h"

int main(int argc, char const *argv[]) {
  rwlock_t rwlock;
  rwlock_init(&rwlock);
  rwlock_tryrdlock(&rwlock);
  rwlock_tryrdlock(&rwlock);
  rwlock_unlock(&rwlock);
  rwlock_unlock(&rwlock);
  rwlock_destroy(&rwlock);
  return 0;
}
