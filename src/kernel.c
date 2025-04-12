#include "kernel.h"

#include <stdint.h>
#include <stddef.h>
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "types.h"
#include "idt/idt.h"
#include "io/io.h"


typedef uint16_t* uint16_array;

uint16_array video_mem = 0;
int32_t terminal_current_row = 0;
int32_t terminal_current_col = 0;

uint16_t terminal_make_char(char asciiChar, byte colour) 
{
    return (colour << 8) | asciiChar;
}

void terminal_putchar(int32_t x, int32_t y, char asciiChar, byte colour)
{
    video_mem[(y*VGA_WIDTH) + x] = terminal_make_char(asciiChar, colour);
}

void terminal_initialize() 
{
    video_mem = (uint16_array)(0xb8000);
    terminal_current_row = 0;
    terminal_current_col = 0;
    for (int32_t y = 0; y < VGA_HEIGHT; y++) 
    {
        for (int32_t x = 0; x < VGA_WIDTH; x++) 
        {
            terminal_putchar(x,y,' ', 0);
        }
    }
}

void terminal_newline()
{
    terminal_current_col = 0;
    terminal_current_row ++;
    if (terminal_current_row >= VGA_HEIGHT)
    {
        terminal_current_row = 0;
    }
}

void terminal_writechar(char asciiChar, byte colour)
{
    if (asciiChar == '\n')
    {
        terminal_newline();
        return;
    }

    terminal_putchar(terminal_current_col,terminal_current_row, asciiChar, colour); 
    terminal_current_col++;
    if (terminal_current_col >= VGA_WIDTH) 
    {
        terminal_newline();
    }
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (*str != 0)
    {
        len++;
        str++;
    }
    return len;
}

void print(const char* str) 
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i)
    {
        terminal_writechar(str[i],15);
    }
}


struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main()
{
    terminal_initialize();
    print("Welcome to ERU OS\nVersion 0.0.1");

    // initialize the heap
    kheap_init();

    // initialize interrupts
    idt_init();

    // setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
    
    mem_ptr ptr =  kzalloc(4096);
    paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (mem_ptr)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITABLE); 
    enable_paging();


    char *ptr2 = (char*)0x1000;
    ptr2[0] = 'A';
    ptr2[1] = 'B';
    print(ptr2);
    print(ptr);

    enable_interrupts();

  

}