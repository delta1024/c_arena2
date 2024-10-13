/* chunk.c
 * Copyright 2024 Jacob Stannix
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in 
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "arena.h"
#include "chunk.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>

heaparena_e heaparena_init(heaparena_t *arena) {
	void *ptr = mmap(
		NULL, PAGESIZE,
		PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE,
		-1, 0
	);
	if (ptr == NULL) {
		perror("mmap");
		return HEAPARENA_ERR;
	}
	struct heapchunk_t *first = (struct heapchunk_t*)ptr; 
	printf("asserted heapchunk_partition\n");
	assert(heapchunk_partition(NULL, PAGESIZE, &first));
	assert(first->size == PAGESIZE - sizeof(struct heapchunk_t));
	printf("asserted pagesize\n");
        assert(first->meta.next_in_mem == NULL);
	printf("asserted next_in_mem\n");
        assert(first->meta.prev_in_mem == NULL);
	printf("asserted prev_in_mem\n");

	arena->start = first;
	arena->avalible = first->size;
	return HEAPARENA_OK;
}

int main() {
	heaparena_t arena;
	assert(heaparena_init(&arena));
}
