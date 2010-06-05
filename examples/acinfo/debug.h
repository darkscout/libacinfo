#ifndef DEBUG_H
#define DEBUG_H 1

#include <stdio.h>

/* prints out every printable byte from a buffer.
 * unprintable bytes are replaced with '*' 
 */
void debug_buffer(char *buffer, int len);

/* prtints out a buffer as hex */
void debug_buffer_hex(unsigned char *buffer, int len);

#endif
