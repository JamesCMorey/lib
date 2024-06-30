#include <stdio.h>
#include <short_types.h>

char *get_bits(char *dest, u8);
char *get_max_64b(char *dest, u64 value, size_t size);

/*
 * Will save in dest the bit layout of a passed in variable of at most 64 bits.
 * The size (number of bytes) dictates how many bytes will be read.
 * */
char *get_max_64b(char *dest, u64 value, size_t size) {
	u8 *value_ptr = (u8 *)&value;
	char tmp[9];

	for(int i = size - 1; i >= 0; i--) {
		/* get each index: 0, 8, 16, etc */ 
		int insert_index = (size- i - 1) * 8;		
		/* paste the bits in the byte starting at insert_index */
		sprintf(&dest[insert_index], "%s", get_bits(tmp, value_ptr[i]));	
	}

	dest[size * 8] = '\0';
	return dest;
}

char *get_bits(char *dest, u8 value) {
	for (u8 i = 0; i < 8; i++) {
		if (value & 128) {
			dest[i] = '1';
		}
		else {
			dest[i] = '0';
		}
		value = value<<1;
	}

	return dest;
}
