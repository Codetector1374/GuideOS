project(stage2_bootloader ASM C)

add_library(stage2Bootloader OBJECT
        src/asm/stage2.s
        src/asm/protected_mode.s

        src/stage2_loader.c
)

set(CMAKE_C_FLAGS "-static -fno-builtin -fno-pic -nostdinc -m32")
set(CMAKE_ASM_FLAGS "-m32")

set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_DEBUG "-Os -ggdb3")


add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj
        COMMAND ${CMAKE_LINKER} -m elf_i386 -T ${CMAKE_CURRENT_SOURCE_DIR}/stage2.ld -o ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj $<TARGET_OBJECTS:stage2Bootloader>
        COMMAND ${CMAKE_OBJDUMP} -M intel -D ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj > ${CMAKE_CURRENT_BINARY_DIR}/stage2.asm
        COMMAND ${CMAKE_OBJDUMP} -x ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj > ${CMAKE_CURRENT_BINARY_DIR}/stage2.objdump
        COMMAND_EXPAND_LISTS
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/stage2.asm
        DEPENDS $<TARGET_OBJECTS:stage2Bootloader> stage2Bootloader ${CMAKE_CURRENT_SOURCE_DIR}/stage2.ld
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stage2.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj ${CMAKE_CURRENT_BINARY_DIR}/stage2.bin
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj
)

add_custom_target(stage2Binary
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stage2.bin
        )