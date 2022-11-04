#include <syslib.h>
#include <stdarg.h>

unsigned int strlen(const char *str){
    unsigned int len = 0;
    while(str[len]!='\0')
    {
        len++;
    }
    return len;
}

int puts(const char * str){
    int len = strlen(str);
    return sys_write(STDOUT, str, len);
}

int put_char(char c){
    return sys_write(STDOUT, &c, 1);
}

int gets(char * s){
    int i = 0, c = get_char();
    while (c != '\n' && c != EOF)
    {
        s[i] = c;
        if(s[i] == '\b' && i > 0){
            i--;
            put_char(c);
        }else if(s[i] == '\b' && i == 0 ){
            //no hago nada
        }else{
            i++;
            put_char(c);
        }
        c = get_char();
    }
    s[i] = '\0';
    return i;
}

char get_char(){
    char c;
    sys_read(STDIN, &c, 1);
    if(c == 0){
        return EOF;
    }
    return c;
}

//Retorna 1 si lo encuentra, 0 sino
unsigned int char_belongs(char *s,char c){
    while(*s != '\0'){
        if(*s == c){
            return 1;
        }
        s++;
    }
    return 0;
}

//https://code.woboq.org/userspace/glibc/string/strcmp.c.html
int strcmp (const char *p1, const char *p2){
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

static void reverse(char s[]);

//Turn an integer number to a char array
void itoa(int n, char s[]){
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

static void reverse(char s[]){
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

char valueToHexChar(unsigned char value) {
    return value >= 10 ? (value - 10 + 'A') : (value + '0');
} 

//Checks the first occurence of p2 inside p1, return the index where p2 appears inside p1 or -1
//if p2 isnt found inside p1
int contains_string(const char *p1,const char *p2){
    const unsigned char *s1 = (const unsigned char *) p1;
    const unsigned char *s2 = (const unsigned char *) p2;
    if(s2 == NULL)//if s2 is empty
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

//Tomado de x86-Barebones
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
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

void strcpy(char * dest, char * src) {
	unsigned int len = strlen(src);
    int i;
	for (i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    dest[i] = 0;
}

void fprintf(int fd, char * str, ...) {
    va_list vl;
	int i = 0, j=0;
    char buff[100]={0}, tmp[20];
    va_start( vl, str ); 

    while (str && str[i]) {
        if(str[i] == '%') {
            i++;
            switch (str[i]) {
                case 'c': 
                    buff[j] = (char)va_arg( vl, int );
                    j++;
                    break;
                case 'd': 
                    itoa(va_arg( vl, int ), tmp);
                    strcpy(&buff[j], tmp);
                    j += strlen(tmp);
                    break;
                case 's': {
                    char * str = va_arg( vl, char * );
                    strcpy(&buff[j], str);
                    j += strlen(str);
                    break;
                }
                case 'x': {
                    uintToBase(va_arg( vl, int ), tmp, 16);
                    strcpy(&buff[j], tmp);
                    j += strlen(tmp);
                    break;
                }
            }
        } else {
            buff[j] = str[i];
            j++;
        }
        i++;
    }
    va_end(vl);
    sys_write(fd, buff, j);
 }