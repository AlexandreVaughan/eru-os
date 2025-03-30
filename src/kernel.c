#include "kernel.h"

#include <stdint.h>
#include <stddef.h>
#include "memory/memory.h"
#include "idt/idt.h"

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

void kernel_main()
{
    terminal_initialize();
    print("Welcome to ERU OS\nVersion 0.0.1");

    // initialize interrupts
    idt_init();

    int i = 0;
    int x = 1/i;
    x++;
}