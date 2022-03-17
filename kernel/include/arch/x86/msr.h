//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#ifndef GUIDE_OS_MSR_H
#define GUIDE_OS_MSR_H

#define IA32_APIC_BASE_MSR                0x1B
# define IA32_APIC_BASE_MSR_ENABLED       (1UL << 11U)
# define IA32_APIC_BASE_MSR_BSP           (1UL << 8U)
# define IA32_APIC_BASE_MSR_BASE_ADDR_MSK (~0xFFFUL)

#endif //GUIDE_OS_MSR_H
