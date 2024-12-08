#ifndef __LIBJJ_SYNC_H__
#define __LIBJJ_SYNC_H__

#include <semaphore.h>

int sem_safe_wait(sem_t *sem, int timedout_sec);

#endif // __LIBJJ_SYNC_H__