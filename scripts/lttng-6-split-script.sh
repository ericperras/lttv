NAME=-2.6.19-lttng-$1
#for hotplug
FILE=../${PRENAME}${COUNT}${NAME}-relay.diff
VALUE=$(( ${VALUE} + 1 ))
printf -v COUNT "%02d" ${VALUE}

IN="?_include_linux_relay.h
?_kernel_relay.c
?_block_blktrace.c"

for a in $IN; do wr $a $FILE; done

FILE=../${PRENAME}${COUNT}${NAME}-atomic.diff
IN="
?_include_asm-alpha_atomic.h
?_include_asm-alpha_system.h
?_include_asm-arm_atomic.h
?_include_asm-generic_atomic.h
?_include_asm-i386_atomic.h
?_include_asm-i386_system.h
?_include_asm-ia64_atomic.h
?_include_asm-mips_atomic.h
?_include_asm-parisc_atomic.h
?_include_asm-powerpc_atomic.h
?_include_asm-powerpc_system.h
?_include_asm-sparc64_atomic.h
?_include_asm-x86_64_atomic.h
?_include_asm-x86_64_system.h"
FILE=../${PRENAME}${COUNT}${NAME}-local.diff
IN="
?_include_asm-alpha_local.h
?_include_asm-generic_local.h
?_include_asm-i386_local.h
?_include_asm-ia64_local.h
?_include_asm-mips_local.h
?_include_asm-parisc_local.h
?_include_asm-powerpc_local.h
?_include_asm-s390_local.h
?_include_asm-sparc64_local.h
?_include_asm-x86_64_local.h"
FILE=../${PRENAME}${COUNT}${NAME}-facility-core-headers.diff
VALUE=$(( ${VALUE} + 1 ))
printf -v COUNT "%02d" ${VALUE}

IN="?_include_ltt_ltt-facility-core.h
?_include_ltt_ltt-facility-id-core.h
?_include_ltt_ltt-facility-select-core.h"

for a in $IN; do wr $a $FILE; done


FILE=../${PRENAME}${COUNT}${NAME}-facility-loader-core.diff
VALUE=$(( ${VALUE} + 1 ))
printf -v COUNT "%02d" ${VALUE}

IN="?_ltt_facilities_ltt-facility-loader-core.c
?_ltt_facilities_ltt-facility-loader-core.h
?_ltt_facilities_Makefile"

for a in $IN; do wr $a $FILE; done

?_include_asm-arm_unistd.h
?_include_asm-mips_unistd.h
?_include_asm-powerpc_systbl.h
?_arch_i386_kernel_traps.c
?_arch_i386_mm_fault.c"
?_include_asm-mips_mipsregs.h
?_arch_mips_kernel_scall32-o32.S
?_arch_mips_kernel_scall64-64.S
?_arch_mips_kernel_scall64-n32.S
?_arch_mips_kernel_scall64-o32.S
?_arch_mips_mm_fault.c"
?_arch_powerpc_kernel_traps.c
?_arch_powerpc_mm_fault.c"
IN="?_arch_ppc_kernel_misc.S
?_arch_ppc_kernel_traps.c
?_arch_ppc_mm_fault.c"
?_arch_x86_64_kernel_traps.c
?_arch_x86_64_mm_fault.c"