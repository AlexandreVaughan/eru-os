#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"

struct idt_desc idt_descriptors[ERUOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interrupt();

void int21h_handler()
{
    print("keyboard pressed");
    outb(0x20,0x20);
}

void no_interrupt_handler()
{
    outb(0x20,0x20);
}

void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, mem_ptr address)
{
    struct idt_desc* desc = idt_descriptors+interrupt_no;
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xee;
    desc->offset_2 = (uint32_t) address >> 16;
}

void idt_init()
{
    memset(idt_descriptors,0,sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors)-1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    // set no interrupt handler by default to avoid processor
    // failures
    for (int i = 0; i < ERUOS_TOTAL_INTERRUPTS; ++i )
    {
        idt_set(i,no_interrupt);
    }
    idt_set(0,idt_zero);
    idt_set(0x21, int21h); // keyboard pressed
    idt_load(&idtr_descriptor);
}