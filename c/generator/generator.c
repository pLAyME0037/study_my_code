#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#include <sys/mman.h>
#include <unistd.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

#define GENERATOR_STACK_CAPACITY (1024*getpagesize())
#define da_lasted(da) (da)->items[(da)->count - 1]

typedef struct {
    void *rsp;
    void *stack_base;
    bool is_dead;
    bool is_fresh;
} Generator;

typedef struct {
    Generator **items;
    int count;
    int capacity;
} Generator_Stack;

thread_local Generator_Stack generator_stack = {0};

void generator_init(void) {
    Generator *g = malloc(sizeof(*g));
    assert(g != NULL && "Buy More Ram");
    memset(g, 0, sizeof(*g));

    da_append(&generator_stack, g);
}

void __attribute__((naked)) generator_restore_context(void *rsp) {
    // @arch
    (void)rsp;
    asm(
    "    movq %rdi, %rsp\n"
    "    popq %r15\n"
    "    popq %r14\n"
    "    popq %r13\n"
    "    popq %r12\n"
    "    popq %rbx\n"
    "    popq %rbp\n"
    "    popq %rdi\n"
    "    ret\n");
}

void __attribute__((naked)) generator_restore_context_with_return(void *rsp, void *arg) {
    // @arch
    UNUSED(rsp);
    UNUSED(arg);
    asm(
    "    movq %rdi, %rsp\n"
    "    movq %rsi, %rax\n"
    "    popq %r15\n"
    "    popq %r14\n"
    "    popq %r13\n"
    "    popq %r12\n"
    "    popq %rbx\n"
    "    popq %rbp\n"
    "    popq %rdi\n"
    "    ret\n");
}

// Linux x86_64 call convention
// %rdi, %rsi, %rdx, %rcx, %r8, and %9

void* __attribute__((naked)) generator_next(Generator *g, void* arg) {
    UNUSED(g);
    UNUSED(arg);
    // @arch
    asm(
    "    pushq %rdi\n"
    "    pushq %rbp\n"
    "    pushq %rbx\n"
    "    pushq %r12\n"
    "    pushq %r13\n"
    "    pushq %r14\n"
    "    pushq %r15\n"
    "    movq %rsp, %rdx\n"     // rsp
    "    jmp generator_switch_context\n");
}

void generator_switch_context(Generator *g, void *arg, void *rsp) {
    da_lasted(&generator_stack)->rsp = rsp;
    da_append(&generator_stack, g);
    if (g->is_fresh) {
        g->is_fresh = false;
        void **rsp = (void**)((char*)g->stack_base + GENERATOR_STACK_CAPACITY);
        *(rsp-3) = arg;
        generator_restore_context(g->rsp);
    } else {
        generator_restore_context_with_return(g->rsp, arg);
    }
}

void* __attribute__((naked)) generator_yield(void *arg) {
    UNUSED(arg);
    // @arch
    asm(
    "    pushq %rdi\n"
    "    pushq %rbp\n"
    "    pushq %rbx\n"
    "    pushq %r12\n"
    "    pushq %r13\n"
    "    pushq %r14\n"
    "    pushq %r15\n"
    "    movq %rsp, %rsi\n"     // rsp
    "    jmp generator_return\n");
}

void generator_return(void *arg, void *rsp) {
    da_lasted(&generator_stack)->rsp = rsp;
    generator_stack.count -= 1;
    generator_restore_context_with_return(da_lasted(&generator_stack)->rsp, arg);
}

void generator__finish_current(void) {
    da_lasted(&generator_stack)->is_dead = true;
    generator_stack.count -= 1;
    generator_restore_context_with_return(da_lasted(&generator_stack)->rsp, NULL);
}

Generator *generator_create(void (*f)(void*)) {
    Generator *g = malloc(sizeof(*g));
    assert(g != NULL && "Buy More Ram");
    memset(g, 0, sizeof(*g));

    g->stack_base = mmap(NULL, GENERATOR_STACK_CAPACITY, PROT_WRITE|PROT_READ, MAP_PRIVATE|MAP_STACK|MAP_ANONYMOUS|MAP_GROWSDOWN, -1, 0);
    assert(g->stack_base != MAP_FAILED);
    void **rsp = (void**)((char*)g->stack_base + GENERATOR_STACK_CAPACITY);

    *(--rsp) = generator__finish_current;
    *(--rsp) = f;
    *(--rsp) = 0;   // push rdi
    *(--rsp) = 0;   // push rbx
    *(--rsp) = 0;   // push rbp
    *(--rsp) = 0;   // push r12
    *(--rsp) = 0;   // push r13
    *(--rsp) = 0;   // push r14
    *(--rsp) = 0;   // push r15
    g->rsp = rsp;
    g->is_fresh = true;

    return g;
}

void generator_destory(Generator *g) {
    munmap(g->stack_base, GENERATOR_STACK_CAPACITY);
    free(g);
}

#define foreach(it, g, arg) for (void* it = generator_next(g, arg); !g->is_dead; it = generator_next(g, arg))

void forever(void *arg) {
    while (true) generator_yield(arg);
}

void fibonacci(void *arg) {
    long max = (long)arg;
    long a = 0;
    long b = 1;
    while (a < max) {
        long result = a;

        // Generator *g = generator_create(forever, (void*)a);
        //
        // result += (long)generator_next(g);
        // result += (long)generator_next(g);
        // result += (long)generator_next(g);

        generator_yield((void*)result);

        long c = a + b;
        a = b;
        b = c;
    }
}

void square(void *arg) {
    while (true) {
        long x = (long)arg;
        arg = generator_yield((void*)(x*x));
    }
}

int main() {
    generator_init();

    
    // for (long it = (long)generator_next(g, (void*)(1000*1000)); !g->is_dead; it = (long)generator_next(g, NULL)) {
    //     printf("%ld\n", it);
    // }

    // Generator *g = generator_create(fibonacci);
    // foreach (value, g, (void*)(1000*1000)) {
    //     printf("%ld\n", (long)value);
    // }

    Generator *g = generator_create(square);
    for (long x = 1; x < 100; ++x) {
        long xx = (long)generator_next(g, (void*)x);
        printf("%ld\n", xx);
    }

    generator_destory(g);

    return 0;
}

