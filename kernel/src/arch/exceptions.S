.globl enable_debug_exceptions
enable_debug_exceptions:
    msr     daifclr, #8
    ret

.globl disable_debug_exceptions
disable_debug_exceptions:
    msr     daifset, #8
    ret

.globl enable_serror
enable_serror:
    msr     daifclr, #4
    ret

.globl disable_serror
disable_serror:
    msr     daifset, #4
    ret

.globl enable_irq
enable_irq:
    msr     daifclr, #2
    ret

.globl disable_irq
disable_irq:
    msr     daifset, #2
    ret

.globl enable_fiq
enable_fiq:
    msr     daifclr, #1
    ret

.globl disable_fiq
disable_fiq:
    msr     daifset, #1
    ret
