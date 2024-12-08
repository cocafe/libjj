#include <time.h>
#include <semaphore.h>
#include "logging.h"

int sem_safe_wait(sem_t *sem, int timedout_sec)
{
        struct timespec ts = { };
        int err;

        if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                pr_err("clock_gettime()\n");
                return -EINVAL;
        }

        ts.tv_sec += timedout_sec;

        while ((err = sem_timedwait(sem, &ts)) == -1 && errno == EINTR)
                continue;

        if (err == -1) {
                if (errno == ETIMEDOUT) {
                        return -ETIMEDOUT;
                } else {
                        return -errno;
                }
        }

        return 0;
}
