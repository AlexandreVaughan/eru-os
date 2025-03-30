#include "kernel.h"

#include <stdint.h>
#include <stddef.h>
typedef char Byte;
typedef Byte* ByteArray;

typedef uint16_t Word;
typedef Word* WordArray;

typedef int Int32;



WordArray video_mem = 0;
Int32 terminal_current_row = 0;
Int32 terminal_current_col = 0;

Word terminal_make_char(Byte asciiChar, Byte colour) 
{
    return (colour << 8) | asciiChar;
}

void terminal_putchar(Int32 x, Int32 y, Byte asciiChar, Byte colour)
{
    video_mem[(y*VGA_WIDTH) + x] = terminal_make_char(asciiChar, colour);
}

void terminal_initialize() 
{
    video_mem = (WordArray)(0xb8000);
    terminal_current_row = 0;
    terminal_current_col = 0;
    for (Int32 y = 0; y < VGA_HEIGHT; y++) 
    {
        for (Int32 x = 0; x < VGA_WIDTH; x++) 
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

void terminal_writechar(Byte asciiChar, Byte colour)
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
}