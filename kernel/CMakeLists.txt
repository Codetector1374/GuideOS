project(kernel ASM C)

add_library(kernel OBJECT
        src/arch/x86/asm/bsp_init.s
        src/arch/x86/asm/multiboot.s
        src/arch/x86/asm/trap.s
        src/arch/x86/asm/vector.s

        src/arch/x86/console.c
        src/arch/x86/interrupt.c
        src/arch/x86/system.c
        src/arch/x86/mem/vm.c
        src/arch/x86/lapic.c
        src/arch/x86/x86_pic.c
        src/arch/x86/ioapic.c

        src/mem/boot_alloc.c
        src/mem/page_alloc.c 
        src/mem/memlayout.c 
        src/mem/kmalloc.c
        
        src/time/systick.c
        
        src/proc/proc.c
        
        src/device/uart/uart.c 
        
        src/cpu.c
        src/spinlock.c 

        src/kmain.c
        
        src/util/kprintf.c 
        src/util/string.c 
        src/util/helpers.c 
)

include_directories(include)

set(CMAKE_C_FLAGS "-static -fno-pic -ffreestanding -fno-stack-protector -mcmodel=large -mgeneral-regs-only -m64 -Wall -Werror-implicit-function-declaration")
set(CMAKE_ASM_FLAGS "-m64 -x assembler-with-cpp")

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_DEBUG "-O2 -ggdb")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj
        COMMAND ${CMAKE_LINKER} -m elf_x86_64 -T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj $<TARGET_OBJECTS:kernel>
        COMMAND ${CMAKE_OBJDUMP} -M intel -S ${CMAKE_CURRENT_BINARY_DIR}/kernel.obj > ${CMAKE_CURRENT_BINARY_DIR}/kernel.asm
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