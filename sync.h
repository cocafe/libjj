#ifndef __LIBJJ_SYNC_H__
#define __LIBJJ_SYNC_H__

#include <semaphore.h>

int sem_safe_wait(sem_t *sem, uint32_t timeout_ms);
int sem_safe_wait_flag(sem_t *sem, uint32_t timeout_ms, unsigned *flag);
void sem_post_flag(sem_t *sem, unsigned *flag);

#endif // __LIBJJ_SYNC_H__