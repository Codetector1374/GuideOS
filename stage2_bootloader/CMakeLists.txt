
project(stage2_bootloader ASM)

add_library(stage2Bootloader OBJECT
        src/asm/stage2.s)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -fno-builtin -fno-pic -nostdinc -m64")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_DEBUG "-Os -ggdb3")


add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj
        COMMAND ${CMAKE_LINKER} -m elf_x86_64 -T ${CMAKE_CURRENT_SOURCE_DIR}/stage2.ld -o ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj $<TARGET_OBJECTS:stage2Bootloader>
        COMMAND objdump -D ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj > ${CMAKE_CURRENT_BINARY_DIR}/stage2.asm
        COMMAND objdump -M intel -D ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj > ${CMAKE_CURRENT_BINARY_DIR}/stage2.asm
        COMMAND objdump -x ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj > ${CMAKE_CURRENT_BINARY_DIR}/stage2.objdump
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/stage2.asm
        DEPENDS $<TARGET_OBJECTS:stage2Bootloader> stage2Bootloader ${CMAKE_CURRENT_SOURCE_DIR}/stage2.ld)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stage2.bin
        COMMAND objcopy -O binary ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj ${CMAKE_CURRENT_BINARY_DIR}/stage2.bin
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stage2.obj
)

add_custom_target(stage2Binary
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stage2.bin
        )