cmake_minimum_required(VERSION 3.0)
project(guide_os C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -Wall -ggdb -Werror -fno-omit-frame-pointer -fno-stack-protector")

set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb3")

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

add_subdirectory(stage1_bootloader)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/guideos.img
        COMMAND dd if=/dev/zero of=${CMAKE_CURRENT_BINARY_DIR}/guideos.img bs=1M count=1 status=none
        COMMAND dd if=${CMAKE_CURRENT_BINARY_DIR}/stage1_bootloader/stage1.bin of=${CMAKE_CURRENT_BINARY_DIR}/guideos.img bs=512 count=1 conv=notrunc
        DEPENDS stage1Binary ${CMAKE_CURRENT_BINARY_DIR}/stage1_bootloader/stage1.bin
)

add_custom_target(
        disk_image ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/guideos.img
)