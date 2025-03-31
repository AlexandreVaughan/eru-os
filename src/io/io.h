#ifndef IO_H
#define IO_H

#include "types.h"

ubyte insb(uword port);
uword insw(uword port);

void outb(uword port, ubyte val);
void outw(uword port, uword val);

#endif