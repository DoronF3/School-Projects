//
// Created by USER on 19/04/2020.
//

#include "Thread.h"

#ifdef __x86_64__
/* code for 64 bit Intel arch */

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr) {
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
                 "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}

#else
/* code for 32 bit Intel arch */

typedef unsigned int address_t;
#define JB_SP 4
#define JB_PC 5

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
        "rol    $0x9,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}

#endif

Thread::Thread(int id, int priority, State state, void (*f)()) : _id(id),
                                                    _priority(priority),
                                                    quantum_time(0),
                                                    _state(state),_f(f)
{
    address_t sp, pc;

    sp = (address_t)_stack + STACK_SIZE - sizeof(address_t);
    pc = (address_t)f;
    sigsetjmp(_buf, 1);
    (_buf->__jmpbuf)[JB_SP] = translate_address(sp);
    (_buf->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&_buf->__saved_mask);
}

Thread::Thread(int id, int priority, State state) : _id(id),_priority(priority),
                                                    quantum_time(0),
                                                    _state(state){}


