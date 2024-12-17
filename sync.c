#include <time.h>
#include <semaphore.h>
#include "logging.h"

int sem_safe_wait(sem_t *sem, uint32_t timeout_ms)
{
        struct timespec ts = { };
        int err;

        if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                pr_err("clock_gettime()\n");
                return -EINVAL;
        }

        ts.tv_sec += timeout_ms / 1000;
        ts.tv_nsec += (timeout_ms % 1000) * 1000000;

        if (ts.tv_nsec >= 1000000000) {
                ts.tv_sec += ts.tv_nsec / 1000000000;
                ts.tv_nsec %= 1000000000;
        }

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
