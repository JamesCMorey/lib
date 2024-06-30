#pragma once

#include <stdio.h>
#include "short_types.h"

/* gets the bits in a byte and stores them in dest */
char *get_bits(char *dest, u8);
/*
 * Will save in dest the bit layout of a passed in variable of at most 64 bits.
 * The size (number of bytes) dictates how many bytes will be read.
 * */
char *get_max_64b(char *dest, u64 value, size_t size);
