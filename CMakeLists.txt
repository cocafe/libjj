cmake_minimum_required(VERSION 3.20)

project(jjcom C)

set(CMAKE_C_STANDARD 11)

set(COMMON_SRC
    bits.h
    config_opts.c
    config_opts.h
    ffs.h
    file.c
    file.h
    hash.h
    hashtable.h
    list.h
    list_sort.h
    log2.h
    logging.c
    logging.h
    malloc.c
    malloc.h
    string.c
    string.h
    utils.h
    uuid.h
    endian_wrap.h
    )

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        message(STATUS "Build type is not defined, set default one")
        set(CMAKE_BUILD_TYPE "Debug")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -ggdb3 -gdwarf-4 -O0 ")
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 ")
endif()

if ("${JJCOM_HAVE_CJSON}" STREQUAL "1")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CJSON")
        set(JSON_SRC cJSON.c cJSON.h jkey.c jkey.h json.c json.h)
endif()

if ("${JJCOM_HAVE_SOCKET}" STREQUAL "1")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SOCKET")
        if (WIN32 AND MINGW)
                set(SOCKET_SRC wsock2.c socket.h)
                set(SOCKET_LIB ws2_32)
        else()
                set(SOCKET_SRC socket.c socket.h)
                set(SOCKET_LIB )
        endif()
endif()

if ("${JJCOM_HAVE_ICONV}" STREQUAL "1")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_ICONV")
        set(ICONV_SRC iconv.c iconv.h)
        set(ICONV_LIB iconv)
endif()

if ("${JJCOM_HAVE_MATH}" STREQUAL "1")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_MATH")
        set(MATH_SRC math.c math.h)
        set(MATH_LIB m)
endif()

if ("${JJCOM_HAVE_UUID}" STREQUAL "1")
        if (LINUX OR CYGWIN)
                set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_UUID")
                set(UUID_SRC uuid.h)
                set(UUID_LIB uuid)
        endif()
endif()

add_library(jjcom ${COMMON_SRC} ${JSON_SRC} ${SOCKET_SRC} ${ICONV_SRC} ${MATH_SRC} ${UUID_SRC})
target_link_libraries(jjcom pthread ${ICONV_LIB} ${MATH_LIB} ${UUID_LIB} ${SOCKET_LIB})
