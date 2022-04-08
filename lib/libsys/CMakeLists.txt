project(libsys C ASM)

add_library(libsys STATIC
  src/syscall.c
)

target_include_directories(libsys PUBLIC include)
