cmake_minimum_required(VERSION 3.0)

project(guide_os C)

message("USING objdump ${CMAKE_OBJDUMP}")

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-fno-strict-aliasing -Wall -ggdb -Werror -fno-omit-frame-pointer -fno-stack-protector -m64")

set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb3")

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

add_subdirectory(stage1_bootloader)
add_subdirectory(stage2_bootloader)
add_subdirectory(kernel)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/guideos.img
        COMMAND dd if=/dev/zero of=${CMAKE_CURRENT_BINARY_DIR}/guideos.img bs=1048576 count=10
        COMMAND dd if=${CMAKE_CURRENT_BINARY_DIR}/stage1_bootloader/stage1.bin of=${CMAKE_CURRENT_BINARY_DIR}/guideos.img bs=512 count=1 conv=notrunc
        COMMAND dd if=${CMAKE_CURRENT_BINARY_DIR}/stage2_bootloader/stage2.bin of=${CMAKE_CURRENT_BINARY_DIR}/guideos.img bs=512 seek=1 count=64 conv=notrunc
        COMMAND dd if=${CMAKE_CURRENT_BINARY_DIR}/kernel/kernel.bin of=${CMAKE_CURRENT_BINARY_DIR}/guideos.img bs=512 seek=65 conv=notrunc
        DEPENDS
          stage1Binary ${CMAKE_CURRENT_BINARY_DIR}/stage1_bootloader/stage1.bin
          stage2Binary ${CMAKE_CURRENT_BINARY_DIR}/stage2_bootloader/stage2.bin
          kernelBinary ${CMAKE_CURRENT_BINARY_DIR}/kernel/kernel.bin
        )

add_custom_target(
        disk_image ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/guideos.img
)