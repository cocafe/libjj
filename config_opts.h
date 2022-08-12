#ifndef __JJ_CONFIG_OPTS_H__
#define __JJ_CONFIG_OPTS_H__

#include <stdarg.h>
#include <getopt.h>

#ifdef HAVE_ICONV
#include "iconv.h"
#endif

// TODO: specify same param multiple times to create multiple things?

#define OPTION_PATH_LEN                 (256)
#define OPTION_STR_LEN                  (128)

#define D_GENERIC                       (BIT(0U)) // reserved
#define D_SIGNED                        (BIT(1U))
#define D_UNSIGNED                      (BIT(2U))
#define D_STRING                        (BIT(3U))
#define D_DOUBLE                        (BIT(4U))
#define D_FLOAT                         (BIT(5U))

// this is an OR result, using it in case should result as SIGNED
#define D_INTEGER                       (D_SIGNED | D_UNSIGNED)

#define D_BOOLEAN                       D_UNSIGNED
#define D_BOOL_TRUE                     (1)
#define D_BOOL_FALSE                    (0)

typedef struct optstr optstr_t;
typedef struct optdesc optdesc_t;

/*
 * these strings are used to convert optarg to int
 * need to terminated by NULL
 * and NO holes are allowed
 * and first enum must start with ZERO
 */
struct optstr {
        const char *optval;
        const char *desc;
};

/*
 * struct option:
 *      char *name:     long option string
 *      int has_arg:    { no_argument, required_argument, [X]optional_argument }
 *
 *      int *flag:      1. pointer to flag variable to write
 *                      2. if null pointer, then val is the short option
 *
 *      int val:        1. value that write to flag when option triggers if @flag is not NULL
 *                      2. or shorten option char
 *
 * struct option long_opts[] = {
 *         { "verbose",        no_argument,       &verbose_output, 1   },
 *         { "config",         required_argument, NULL,            'c' },
 *         { "sip_server",     required_argument, NULL,            0   },
 *         { 0,                0,                 0,               0   },
 * };
 */

struct optdesc {
        char            short_opt;
        char            *long_opt;      // hmm, use strcmp() to find matched...

        int             has_arg;        // { no_argument, required_argument, [X]optional_argument }

        int             to_set;         // if (@short_opt == 0 && has_arg == no_argument)
                                        //         *(int *)param = to_set; // by getopt() internal

        void           *data;           // data to modify
        size_t          data_sz;
        void           *data_def;       // if @data_def != NULL, @data will be inited with @data_def
                                        // if @data_def == NULL, @data will be reset with 0 by @data_sz
        uint32_t        data_type;      // @data type

        int64_t         min;            // optarg_to_int: verify input: >= @min
                                        // maybe negative, thus int64_t

        int64_t         max;            // optarg_to_int: verify input: <= @max

        const optstr_t *optstrs;        // optstr_to_int: config string array
                                        //                cfg value is matched element index

        int             (*parse)(void *data, char *optarg, size_t vargc, ...);

        char           *help[];         // description in help text, '\n' is not required.
                                        // one element per line
                                        // must terminate with NULL as last element
                                        // first element is used in config_dump()
};

#define opt_noarg(sopt, lopt, help_msg...)                      \
optdesc_t opt_##lopt    = {                                     \
        .short_opt      = sopt,                                 \
        .long_opt       = __stringify(lopt),                    \
        .has_arg        = no_argument,                          \
        .help           = {                                     \
               help_msg,                                        \
               NULL,                                            \
        },                                                      \
}

#define opt_int_noarg(sopt, lopt, ref, val, help_msg...)        \
optdesc_t opt_##lopt    = {                                     \
        .short_opt      = sopt,                                 \
        .long_opt       = __stringify(lopt),                    \
        .has_arg        = no_argument,                          \
        .to_set         = val,                                  \
        .data           = &ref,                                 \
        .data_sz        = sizeof(ref),                          \
        .data_def       = NULL,                                 \
        .data_type      = D_SIGNED,                             \
        .help           = {                                     \
               help_msg,                                        \
               NULL,                                            \
        },                                                      \
}

#define opt_uint_noarg(sopt, lopt, ref, val, help_msg...)       \
optdesc_t opt_##lopt    = {                                     \
        .short_opt      = sopt,                                 \
        .long_opt       = __stringify(lopt),                    \
        .has_arg        = no_argument,                          \
        .to_set         = val,                                  \
        .data           = &ref,                                 \
        .data_sz        = sizeof(ref),                          \
        .data_def       = NULL,                                 \
        .data_type      = D_UNSIGNED,                           \
        .help           = {                                     \
               help_msg,                                        \
               NULL,                                            \
        },                                                      \
}

#define opt_int(sopt, lopt, ref, def, help_msg...)              \
optdesc_t opt_##lopt    = {                                     \
        .short_opt      = sopt,                                 \
        .long_opt       = __stringify(lopt),                    \
        .has_arg        = required_argument,                    \
        .data           = &ref,                                 \
        .data_sz        = sizeof(ref),                          \
        .data_def       = def,                                  \
        .data_type      = D_SIGNED,                             \
        .parse          = optarg_to_int,                        \
        .help           = {                                     \
               help_msg,                                        \
               NULL,                                            \
        },                                                      \
}

#define opt_uint(sopt, lopt, ref, def, help_msg...)             \
optdesc_t opt_##lopt    = {                                     \
        .short_opt      = sopt,                                 \
        .long_opt       = __stringify(lopt),                    \
        .has_arg        = required_argument,                    \
        .data           = &ref,                                 \
        .data_sz        = sizeof(ref),                          \
        .data_def       = def,                                  \
        .data_type      = D_UNSIGNED,                           \
        .parse          = optarg_to_int,                        \
        .help           = {                                     \
               help_msg,                                        \
               NULL,                                            \
        },                                                      \
}

#define opt_strbuf(sopt, lopt, ref, def, help_msg...)           \
optdesc_t opt_##lopt    = {                                     \
        .short_opt      = sopt,                                 \
        .long_opt       = __stringify(lopt),                    \
        .has_arg        = required_argument,                    \
        .data           = ref,                                  \
        .data_sz        = sizeof(ref),                          \
        .data_def       = def,                                  \
        .data_type      = D_STRING,                             \
        .parse          = optarg_to_str,                        \
        .help           = {                                     \
               help_msg,                                        \
               NULL,                                            \
        },                                                      \
}

/**
 * Example:
 *
 * opt_noarg('h', help, "This help message");
 * opt_strbuf('c', json_path, json_path, DEFAULT_JSON_PATH, "JSON config path");
 *
 * optdesc_t *opt_list[] = {
 *         &opt_help,
 *         &opt_json_path,
 *         NULL,
 * };
 *
 */


int optarg_to_int(void *data, char *optarg, size_t vargc, ...);
int optarg_to_str(void *data, char *optarg, size_t vargc, ...);
int optstr_to_int(void *data, char *optarg, size_t vargc, ...);

int longopts_parse(int argc, char *argv[], optdesc_t **opt_list);

#ifdef HAVE_ICONV
int wchar_longopts_parse(int argc, wchar_t *wargv[], optdesc_t **opt_list);
#endif

#endif //__JJ_CONFIG_OPTS_H__
