#include <sys/types.h>
#include <pthread.h>
void Pthread_mutex_lock(pthread_mutex_t*mutex);
void Pthread_mutex_unlock(pthread_mutex_t*mutex);
void Pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex);
void Pthread_cond_signal(pthread_cond_t *cond);
void Pthread_create(pthread_t *pthread, void *(*start_routine) (void *), void *arg);
void Pthread_join(pthread_t pthread, void **retval);
