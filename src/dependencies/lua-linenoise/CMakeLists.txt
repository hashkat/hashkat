cmake_minimum_required( VERSION 2.6 )
PROJECT(project_lualinenoise)

set( LIBS ../)

include_directories(
	${LIBS}/lua
)

set( lualinenoise_src
    linenoise.c  linenoiselib.c  win32fixes.c
)

add_library( lualinenoise STATIC
             ${lualinenoise_src} )
