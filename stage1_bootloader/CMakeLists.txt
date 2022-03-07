
project(stage1_bootloader ASM)

add_library(stage1Bootloader OBJECT
        src/asm/init.s)

set(CMAKE_C_FLAGS "-static -fno-builtin -fno-pic -nostdinc -m16")
set(CMAKE_ASM_FLAGS "-m32")

set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_DEBUG "-Os -ggdb3")


add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stage1.obj
        COMMAND ${CMAKE_LINKER} -m elf_i386 -N -T ${CMAKE_CURRENT_SOURCE_DIR}/stage1.ld -o ${CMAKE_CURRENT_BINARY_DIR}/stage1.obj $<TARGET_OBJECTS:stage1Bootloader>
        COMMAND ${CMAKE_OBJDUMP} -M intel -D ${CMAKE_CURRENT_BINARY_DIR}/stage1.obj > ${CMAKE_CURRENT_BINARY_DIR}/stage1.asm
        COMMAND ${CMAKE_OBJDUMP} -x ${CMAKE_CURRENT_BINARY_DIR}/stage1.obj > ${CMAKE_CURRENT_BINARY_DIR}/stage1.objdump
        COMMAND_EXPAND_LISTS
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/stage1.asm
        DEPENDS $<TARGET_OBJECTS:stage1Bootloader> stage1Bootloader ${CMAKE_CURRENT_SOURCE_DIR}/stage1.ld)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stage1.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_CURRENT_BINARY_DIR}/stage1.obj ${CMAKE_CURRENT_BINARY_DIR}/stage1.bin
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stage1.obj
)

add_custom_target(stage1Binary
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stage1.bin
        )