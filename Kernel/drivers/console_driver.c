// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <console_driver.h>
#include <lib.h>

#define WIDTH 80
#define HEIGHT 25

static uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base);
static char buffer[64] = {};
static uint8_t * const video = (uint8_t *) 0xB8000;
static uint8_t * current_video = (uint8_t *) 0xB8000;

void delete_char() {
	if (current_video - 2 >= video) {
		current_video -= 2;
	    *current_video = ' ';
		*(current_video+1) = DEFAULT_FORMAT;
	}
}

void print_format(const char * string, uint8_t format) {
	for (int i = 0; string[i] != '\0'; i++) {
		print_char_format(string[i], format);
	}
}

void print_char_format(char character, uint8_t format) {
	switch (character) {
	    case '\n':
		    new_line();
		    break;
	    case '\b':
		    delete_char();
		    break;
	    default:
		    *(current_video++) = character;
		    *(current_video++) = format;
		    if ((uint64_t)(current_video - video) / (2 * WIDTH) >= HEIGHT) {
			    scroll_up();
		    }
		    break;
	}
}

void print(const char * string) {
	for (int i = 0; string[i] != 0; i++) {
		print_char(string[i]);
    }
}

void print_char(char character) {
	print_char_format(character, DEFAULT_FORMAT);
}

void new_line() {
	do {
		print_char_format(' ', DEFAULT_FORMAT);
	} while ((uint64_t)(current_video - video) % (WIDTH * 2) != 0);
}

void print_dec(uint64_t value) {
	print_base(value, 10);
}

void print_hex(uint64_t value) {
	print_base(value, 16);
}

void print_bin(uint64_t value) {
	print_base(value, 2);
}

void print_base(uint64_t value, uint32_t base) {
	uint_to_base(value, buffer, base);
	print(buffer);
}

void clear_console() {
	for (int i = 0; i < HEIGHT * WIDTH; i++){
		video[i * 2] = ' ';
		video[i * 2 + 1] = DEFAULT_FORMAT;
	}
	current_video = video;
}

// Retreived from init
static uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base) {
	char * p = buffer;
	char * p1, * p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void scroll_up(){
    memmove(video, video + WIDTH * 2, WIDTH * (HEIGHT - 1) * 2);
	current_video -= WIDTH * 2;
	for (int i = 0; i < WIDTH * 2; i += 2) {
		current_video[i] = ' ';
		current_video[i+1] = DEFAULT_FORMAT;
	}
}
