#ifndef __LIBJJ_HEX_H__
#define __LIBJJ_HEX_H__

#include <stdint.h>

static void u82hex(uint8_t val, char *hex) {
        static const char hex_digits[] = "0123456789ABCDEF";

        hex[0] = hex_digits[(val >> 4) & 0x0F];
        hex[1] = hex_digits[val & 0x0F];
}

static int hex2u8(char c, uint8_t *v) {
        switch (c) {
        case '0' ... '9':
                *v = c - '0';
                break;

        case 'A' ... 'F':
                *v = c - 'A' + 10;
                break;

        case 'a' ... 'f':
                *v = c - 'a' + 10;
                break;

        default:
                return -EINVAL;
        }

        return 0;
}

static ssize_t hex2u8s(const char *hex, uint8_t *out, size_t outsize) {
        size_t hex_length = strlen(hex);

        if (hex_length % 2 != 0)
                return -EINVAL;

        size_t byte_count = hex_length / 2;
        if (byte_count > outsize)
                return -ENOSPC;

        for (size_t i = 0; i < byte_count; i++) {
                uint8_t hi = 0, lo = 0;
                int err;

                err = hex2u8(hex[2 * i], &hi);
                err |= hex2u8(hex[2 * i + 1], &lo);

                if (err)
                        return -EINVAL;

                out[i] = (hi << 4) | lo;
        }

        return (ssize_t)byte_count;
}

#endif // __LIBJJ_HEX_H__