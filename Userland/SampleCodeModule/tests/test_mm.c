// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syslib.h>
#include <test_util.h>
#include <syscalls.h>
#include <types.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void * address;
  uint32_t size;
} mm_rq;

void test_mm(int argc, char * argv[]) {
	mm_rq mm_rqs[MAX_BLOCKS];
	uint8_t rq;
	uint32_t total;
	uint64_t max_memory;

	if (argc != 2) {
        fprintf(STDOUT, "ERROR: Invalid amount of arguments provided.");
        return;
    }
	if ((max_memory = atoi(argv[1])) <= 0) {
        fprintf(STDOUT, "ERROR: Invalid argument.");
        return;
    }

	int j = 0;
	while (j < 10) {
        rq = 0;
        total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory) {
            mm_rqs[rq].size = get_uniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = malloc(mm_rqs[rq].size);

            if (mm_rqs[rq].address == NULL) {
                fprintf(STDOUT, "No more memory to allocate.\n");
                break;
            }

            if (mm_rqs[rq].address) {
                total += mm_rqs[rq].size;
                rq++;
            }
        }

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++) {
            if (mm_rqs[i].address) {
                memset(mm_rqs[i].address, i, mm_rqs[i].size);
            }
        }
            
        // Check
        for (i = 0; i < rq; i++) {
            if (mm_rqs[i].address) {
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
                    fprintf(STDOUT, "Error in test_mm.\n");
                    return;
                }
            }
        }
            
        // Free
        for (i = 0; i < rq; i++) {
            if (mm_rqs[i].address) {
                free((uint64_t) mm_rqs[i].address);
            }
        }
        j++;            
    } 
    fprintf(STDOUT, "All good with test_mm. ;)\n");
}