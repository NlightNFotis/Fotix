cmake_minimum_required(VERSION 3.8)
project(Fotix)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES
        docs/descriptor_tables.md
        docs/framebuffer.md
        docs/interrupts.md
        docs/pit.md
        docs/ports.md
        include/common.h
        include/descriptor_tables.h
        include/elf.h
        include/fs.h
        include/initrd.h
        include/isr.h
        include/keyboard.h
        include/kheap.h
        include/monitor.h
        include/multiboot.h
        include/ordered_array.h
        include/paging.h
        include/syscall.h
        include/task.h
        include/timer.h
        sys/boot.s
        sys/common.c
        sys/descriptor_tables.c
        sys/dt_asm.s
        sys/elf.c
        sys/fs.c
        sys/initrd.c
        sys/interrupt.s
        sys/isr.c
        sys/keyboard.c
        sys/kheap.c
        sys/main.c
        sys/monitor.c
        sys/ordered_array.c
        sys/paging.c
        sys/process.s
        sys/syscall.c
        sys/task.c
        sys/timer.c
        utils/grub.cfg
        utils/mkinit.c
        utils/prepare_toolchain.sh
        link.ld
        )

add_executable(Fotix ${SOURCE_FILES})