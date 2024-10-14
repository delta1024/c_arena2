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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <sys/mman.h>

[[ noreturn ]] void err_die(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);
	exit(EXIT_FAILURE);
}
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

void *heaparena_alloc(heaparena_t *arena, size_t size) {
	size += -size & (PTRALIGN - 1);
	struct heapchunk_t *prev , *chunk, *next;
	chunk = arena->start;
	prev = next = NULL;
	while (chunk != NULL && size > chunk->size) {
		prev = chunk;
		chunk = chunk->next_free;
	}
	if (chunk == NULL) {
		err_die("reached end of list with no valid chunk\n");
	}

	assert(heapchunk_partition(chunk, size, &next));
	if (prev) {
		prev->next_free = next;
	} else {
		arena->start = next;
	}
	arena->avalible -= size + sizeof(struct heapchunk_t);
	return (void*)get_chunk_ptr(chunk);
}

void heaparena_free(heaparena_t *arena, void *ptr) {
	struct heapchunk_t *chunk, *oldhead, *prev;
	chunk = get_chunk_tag(ptr);
	prev = chunk->meta.prev_in_mem;
	if (prev && !prev->isactive) {
		assert(heapchunk_mergenext(prev));
		arena->avalible += chunk->size + sizeof(struct heapchunk_t);
	} else {
		oldhead = arena->start;
		arena->start = chunk;
		chunk->next_free = oldhead;
		chunk->isactive = false;
		arena->avalible += chunk->size;
	}
}
int main() {
	heaparena_t arena;
	assert(heaparena_init(&arena));

	char *buffer = heaparena_alloc(&arena, 32);
	assert(buffer != NULL);
	uint32_t chunk_size, old_avail;
	old_avail = arena.avalible;
	struct heapchunk_t *chunk = get_chunk_tag(buffer);
	chunk_size = chunk->size;
	assert(chunk->size == 32);
	assert(chunk->meta.next_in_mem == arena.start);
	assert(chunk->meta.prev_in_mem == NULL);
	assert(arena.start->meta.prev_in_mem == chunk);
	assert(arena.start->meta.next_in_mem == NULL);
	assert(chunk->next_free == NULL);
	assert(arena.avalible == (PAGESIZE - 32 - (sizeof(struct heapchunk_t) * 2)));
	heaparena_free(&arena, buffer);
	printf("%lu\n", (unsigned long)arena.start->size);
	assert(arena.avalible == old_avail + chunk_size);
	buffer = heaparena_alloc(&arena, 16);
	printf("%lu\n", (unsigned long)arena.start->next_free->size);
	struct heapchunk_t *chunk2 = get_chunk_tag(buffer);
	assert(chunk == chunk2);
	char *buff2 = heaparena_alloc(&arena, 32);
	printf("%lu\n", (unsigned long)arena.start->next_free->size);
	heaparena_free(&arena, buffer);
	heaparena_free(&arena, buff2);
	assert(arena.start->size == 16 + 32 + sizeof(struct heapchunk_t));

	buffer = heaparena_alloc(&arena, 150);

}
