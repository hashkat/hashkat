PROJECT(uv)
cmake_minimum_required(VERSION 2.8)

set(UV_COMMON_SRCS
    src/fs-poll.c src/inet.c  src/uv-common.c  src/version.c 
)

set(UV_POSIX_SRCS
    src/unix/getaddrinfo.c
    src/unix/async.c
    src/unix/loop.c
    src/unix/thread.c
    src/unix/error.c
    src/unix/proctitle.c
    src/unix/process.c
    src/unix/tty.c
    src/unix/poll.c
    src/unix/pipe.c
    src/unix/core.c
    src/unix/threadpool.c
    src/unix/stream.c
    src/unix/tcp.c
    src/unix/fs.c
    src/unix/signal.c
    src/unix/timer.c
    src/unix/dl.c
    src/unix/udp.c
    src/unix/loop-watcher.c
)

aux_source_directory("src/unix/" UV_UNIX_SRCS)
aux_source_directory("src/unix/" UV_UNIX_SRCS)
aux_source_directory("src/win/" UV_WIN32_SRCS)

include_directories(include include/uv-private src)
add_definitions(-DHAVE_CONFIG_H)

# set UV_SRCS and UV_LIBS

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0502 -DEIO_STACKSIZE=262144 -D_GNU_SOURCE)
    set(UV_SRCS ${UV_WIN32_SRCS} ${UV_COMMON_SRCS})
    set(UV_LIBS ws2_32)
else()
    # POSIX common
    add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -DEIO_STACKSIZE=262144)

    # OS specific thingy
    # FIXME: solaris here
    if(APPLE)
        add_definitions(-DEV_CONFIG_H=\"config_darwin.h\" -DEIO_CONFIG_H=\"config_darwin.h\")
        include_directories(src/ares/config_darwin)
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            src/unix/darwin.c
            src/unix/fsevents.c
            src/unix/kqueue.c)
        set(UV_LIBS m)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_definitions(-DEV_CONFIG_H=\"config_linux.h\" -DEIO_CONFIG_H=\"config_linux.h\")
        include_directories(src/ares/config_linux)
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            src/unix/linux-core.c
            src/unix/linux-syscalls.c
            src/unix/linux-inotify.c)
        set(UV_LIBS rt m)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        add_definitions(-DEV_CONFIG_H=\"config_freebsd.h\" -DEIO_CONFIG_H=\"config_freebsd.h\")
        include_directories(src/ares/config_freebsd)
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            src/unix/freebsd.c
            src/unix/kqueue.c)
        set(UV_LIBS m pthread)
    endif()
endif()

add_library(libuv ${UV_SRCS})

target_link_libraries(libuv ${UV_LIBS})
