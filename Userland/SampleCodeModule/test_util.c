#include <stdio.h>
#include <stdint.h>

//Random
static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint(){
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max){
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

//Memory
void * memset(void * destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

uint8_t memcheck(void *start, uint8_t value, uint32_t size){
  uint8_t *p = (uint8_t *) start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}

//Dummies
void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop(){
  while(1);
}

// void endless_loop_print(uint64_t wait){
//   int64_t pid = my_getpid();

//   while(1){
//     printf("%d ",pid);
//     bussy_wait(wait);
//   }
// }