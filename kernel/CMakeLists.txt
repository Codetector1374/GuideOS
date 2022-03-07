project(kernel ASM C)

add_library(kernel OBJECT
        src/arch/x86_64/asm/bsp_init.s
        src/arch/x86_64/asm/multiboot.s
        src/arch/x86_64/console.c
        src/arch/x86_64/interrupt.c
        src/arch/x86_64/system.c

        src/kmain.c
        src/util/kprintf.c src/util/string.c src/mem/page_alloc.c include/mem/boot_alloc.h src/mem/boot_alloc.c
  )

set(CMAKE_C_FLAGS "-static -fno-pic -ffreestanding -mcmodel=large -mgeneral-regs-only -m64")
set(CMAKE_ASM_FLAGS "-m64")

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_DEBUG "-O2 -ggdb3")

include_directories(include)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj
        COMMAND ${CMAKE_LINKER} -m elf_x86_64 -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj $<TARGET_OBJECTS:kernel>
        COMMAND ${CMAKE_OBJDUMP} -M intel -D ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj > ${CMAKE_CURRENT_BINARY_DIR}/kernel.asm
        COMMAND ${CMAKE_OBJDUMP} -x ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj > ${CMAKE_CURRENT_BINARY_DIR}/kernel.objdump
        COMMAND_EXPAND_LISTS
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/kernel.asm
        DEPENDS $<TARGET_OBJECTS:kernel> kernel ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj
)

add_custom_target(kernelBinary
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
        )