cmake_minimum_required(VERSION 3.13)
project(ccze-cute)

set(SOURCES
    src/cute.c
)

add_library(cute SHARED ${SOURCES})
target_compile_options(cute PUBLIC -Wall -Werror -Wpedantic)
target_link_libraries(cute PUBLIC pcre)

SET_TARGET_PROPERTIES(cute PROPERTIES PREFIX "")

install(TARGETS cute DESTINATION /usr/lib/x86_64-linux-gnu/ccze/)