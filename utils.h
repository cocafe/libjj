#ifndef __JJ_UTILS_H__
#define __JJ_UTILS_H__

#include <errno.h>
#include <time.h>

#include "compiler.h"

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x)           (sizeof(x) / sizeof((x)[0]))
#endif

#if !defined(__always_inline)
#define __always_inline         inline __attribute__((always_inline))
#endif

#if !defined(__unused)
#define __unused                __attribute__((unused))
#endif

#if !defined(UNUSED_PARAM)
#define UNUSED_PARAM(x)         (void)(x)
#endif

#ifndef __min
#define __min(a, b)                             \
        ({                                      \
                __typeof__((a)) _a = (a);       \
                __typeof__((b)) _b = (b);       \
                _a < _b ? _a : _b;              \
        })
#endif /* __min */

#ifndef __max
#define __max(a, b)                             \
        ({                                      \
                __typeof__((a)) _a = (a);       \
                __typeof__((b)) _b = (b);       \
                _a > _b ? _a : _b;              \
        })
#endif /* __max */

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define millis() (uint64_t)({                                           \
                        struct timespec _ts;                            \
                        clock_gettime(CLOCK_MONOTONIC, &_ts);           \
                        _ts.tv_sec * 1000 + _ts.tv_nsec / (1000 * 1000); \
                        })

static inline int ptr_word_write(void *data, size_t word_sz, int64_t val)
{
        switch (word_sz) {
        case 1:
                *(int8_t *)data = (int8_t)val;
                break;

        case 2:
                *(int16_t *)data = (int16_t)val;
                break;

        case 4:
                *(int32_t *)data = (int32_t)val;
                break;

        case 8:
                *(int64_t *)data = val;
                break;

        default:
                return -EINVAL;
        }

        return 0;
}

static inline int ptr_unsigned_word_write(void *data, size_t word_sz, uint64_t val)
{
        switch (word_sz) {
        case 1:
                *(uint8_t *)data = (uint8_t)val;
                break;

        case 2:
                *(uint16_t *)data = (uint16_t)val;
                break;

        case 4:
                *(uint32_t *)data = (uint32_t)val;
                break;

        case 8:
                *(uint64_t *)data = val;
                break;

        default:
                return -EINVAL;
        }

        return 0;
}

static inline int ptr_unsigned_word_read(void *data, size_t word_sz, uint64_t *val)
{
        switch (word_sz) {
        case 1:
                *val = *(uint8_t *)data;
                break;

        case 2:
                *val = *(uint16_t *)data;
                break;

        case 4:
                *val = *(uint32_t *)data;
                break;

        case 8:
                *val = *(uint64_t *)data;
                break;

        default:
                return -EINVAL;
        }

        return 0;
}

// perform arithmetic extend
static inline int ptr_signed_word_read(void *data, size_t word_sz, int64_t *val)
{
        switch (word_sz) {
        case 1:
                *val = *(int8_t *)data;
                break;

        case 2:
                *val = *(int16_t *)data;
                break;

        case 4:
                *val = *(int32_t *)data;
                break;

        case 8:
                *val = *(int64_t *)data;
                break;

        default:
                return -EINVAL;
        }

        return 0;
}

static inline struct timespec ms_to_timespec(uint32_t ms)
{
        struct timespec ts;

        ts.tv_sec = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000;

        return ts;
}

#endif //__JJ_UTILS_H__