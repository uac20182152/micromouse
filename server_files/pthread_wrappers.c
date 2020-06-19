#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
void Pthread_mutex_lock(pthread_mutex_t*mutex) {
  int rc = pthread_mutex_lock(mutex);
  assert(rc == 0);
}
void Pthread_mutex_unlock(pthread_mutex_t*mutex) {
  int rc = pthread_mutex_unlock(mutex);
  assert(rc == 0);
}
void Pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex) {
  int rc = pthread_cond_wait(cond, mutex);
  assert(rc == 0);
}
void Pthread_cond_signal(pthread_cond_t *cond) {
  int rc = pthread_cond_signal(cond);
  assert(rc == 0);
}
void Pthread_create(pthread_t *pthread, void *(*start_routine) (void *), void *arg) {
  int rc=pthread_create(pthread, NULL, start_routine, arg);
  assert(rc == 0);
}
void Pthread_join(pthread_t pthread, void **retval){
  pthread_join(pthread, retval);
}
