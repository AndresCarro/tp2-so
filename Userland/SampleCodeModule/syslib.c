// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syslib.h>
#include <syscalls.h>
#include <types.h>
#include <stdarg.h>

static void reverse(char s[]);
void strcpy(char * dest, char * src);

char ** strtok(char * str, char delim, int * qty) {
    (*qty) = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == delim) {
            (*qty)++;
        }
    }
    char ** parts = malloc(sizeof(char *) * *qty);
    if (parts == NULL) {
        return NULL;
    }
    char buffer[MAX_BUFFER];
    int i, j, k;
    for (i = 0, j = 0, k = 0; str[j] != '\0'; j++) {
        if (str[j] == delim) {
            buffer[i] = '\0';
            char * new_part = malloc(i + 1);
            if (new_part == NULL) {
                while (k > 0) {
                    free((uint64_t) parts[--k]);
                }
                free((uint64_t) parts);
                return NULL;
            }
            strcpy(new_part, buffer);
            parts[k++] = new_part;
            i = 0;
        } else {
            buffer[i] = str[j];
            i++;
        }
    }
    buffer[i] = '\0';
    char * new_part = malloc(i + 1);
    if (new_part == NULL) {
        while (k > 0) {
            free((uint64_t) parts[--k]);
        }
        free((uint64_t) parts);
        return NULL;
    }
    strcpy(new_part, buffer);
    parts[k++] = new_part;
    return parts;
}

unsigned int strlen(char * str) {
    unsigned int len = 0;
    while (str[len]!='\0') {
        len++;
    }
    return len;
}

//https://code.woboq.org/userspace/glibc/string/strcmp.c.html
int strcmp(char * p1, char * p2) {
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    unsigned char c1, c2;
    do {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}

int atoi(char * str) {
    uint64_t i = 0;
    int64_t res = 0;
    int8_t sign = 1;

    if (!str) return 0;

    if (str[i] == '-'){
        i++;
        sign = -1;
    }

    for ( ; str[i] != '\0'; ++i){
        if(str[i] < '0' || str[i] > '9'){
            return 0;
        }
        res = res * 10 + str[i] - '0';
    }

    return res * sign;
}

void itoa(int n, char s[]) {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}

// Retreived from init
uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int is_num(char c) {
    return c >= '0' && c <= '9';
}

void fprintf(int fd, char * str, ...) {
    va_list vl;
	int i = 0, j=0;
    int spaces = 0;
    char buff[MAX_BUFFER]={0}, tmp[100], fmt[MAX_BUFFER];
    va_start( vl, str ); 
    strcpy(fmt, str);

    while (fmt[i]) {
        if(fmt[i] == '%') {
            i++;
            switch (fmt[i]) {
                case 'c': 
                    buff[j] = (char)va_arg( vl, int );
                    j++;
                    if (spaces > 0) {
                        spaces--;
                    }
                    while (spaces > 0) {
                        buff[j] = ' ';
                        j++;
                        spaces--;
                    }
                    break;
                case 'd': 
                    itoa(va_arg( vl, int ), tmp);
                    strcpy(&buff[j], tmp);
                    int len = strlen(tmp);
                    j += len;
                    if (spaces > 0) {
                        spaces = spaces - len > 0 ? spaces - len : 0;
                    }
                    while (spaces > 0) {
                        buff[j] = ' ';
                        j++;
                        spaces--;
                    }
                    break;
                case 's': {
                    char * s = va_arg( vl, char * );
                    strcpy(&buff[j], s);
                    int len = strlen(s);
                    j += len;
                    if (spaces > 0) {
                        spaces = spaces - len > 0 ? spaces - len : 0;
                    }
                    while (spaces > 0) {
                        buff[j] = ' ';
                        j++;
                        spaces--;
                    }
                    break;
                }
                case 'x': {
                    uint_to_base(va_arg( vl, int ), tmp, 16);
                    strcpy(&buff[j], tmp);
                    int len = strlen(tmp);
                    j += len;
                    if (spaces > 0) {
                        spaces = spaces - len > 0 ? spaces - len : 0;
                    }
                    while (spaces > 0) {
                        buff[j] = ' ';
                        j++;
                        spaces--;
                    }
                    break;
                }
                case '-': {
                    i++;
                    int k = 0;
                    while (is_num(fmt[i])) {
                        tmp[k++] = fmt[i++];
                    }
                    i--;
                    tmp[k] = 0;
                    fmt[i--] = '%';
                    spaces = atoi(tmp);
                    break;
                }
            }
        } else {
            buff[j] = fmt[i];
            j++;
            if (j == MAX_BUFFER) {
                write(fd, buff, j);
                j = 0;
            }
        }
        i++;
    }
    va_end(vl);
    write(fd, buff, j);
 }

int put_char(char c) {
    write(STDOUT, &c, 1);
    return 1;
}

char get_char() {
    char c;
    read(STDIN, &c, 1);
    return c;
}

int gets(char * s) {
    int i = 0, c = get_char();
    while (c != '\n' && c != EOF) {
        if (i > 0 && s[i] == '\b') {
            put_char(c);
        } else if(s[i] == '\b' && i == 0 ) {
            
        } else {
            s[i] = c;
            i++;
            put_char(c);
        }
        c = get_char();
    }
    s[i] = '\0';
    return i;
}

unsigned int char_belongs(char * s, char c) {
    while (*s != '\0') {
        if (*s == c) {
            return 1;
        }
        s++;
    }
    return 0;
}

int contains_string(char * p1, char * p2) {
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    if (s2 == NULL) //if s2 is empty
        return -1;
    unsigned int i=0;
    unsigned char flag=0;
    while(*s1 != '\0' && !flag){
        if(*s1 == *s2){
            flag =1;
        }else{
            i++;s1++;
        }
    }
    if(!flag){
        return -1;
    }
    while(*s1 != '\0' && *s2 != '\0'){
        if(*s1 != *s2){
            return -1;
        }
        s1++;s2++;
    }
    if(*s2 == '\0'){
        return i;
    }
    return -1;
}

void strcpy(char * dest, char * src) {
	unsigned int len = strlen(src);
    int i;
	for (i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    dest[i] = 0;
}

static void reverse(char s[]){
     int i, j;
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         char c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}