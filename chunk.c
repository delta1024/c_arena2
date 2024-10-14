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
#include "chunk.h"
#include <string.h>

heapchunk_e heapchunk_partition(struct heapchunk_t *block, uint32_t partsize,
				struct heapchunk_t **newblock) {
	if (block == NULL) {
		struct heapchunk_t sblock = {0};
		sblock.meta = (struct heap_meta){0};
		sblock.isactive = false;
		sblock.size = partsize - sizeof(struct heapchunk_t);
		**newblock = sblock;
		return HEAPCHUNK_OK;
	}

	struct heapchunk_t *next = (struct heapchunk_t*)((char*)block + partsize + sizeof(struct heapchunk_t));

	// patch heap metadata
	next->meta = block->meta;
	block->meta.next_in_mem = next;
	next->meta.prev_in_mem = block;
	if (next->meta.next_in_mem != NULL) {
		next->meta.next_in_mem->meta.prev_in_mem = next;
	}

	// add chunk metadata
	next->size = block->size - sizeof(struct heapchunk_t) - partsize;
	next->isactive = false;
	next->next_free = block->next_free;
	*newblock = next;

	// setup block metadata
	block->size = partsize;
	block->isactive = true;
	block->next_free = NULL;

	// all good
	return HEAPCHUNK_OK;
}
heapchunk_e heapchunk_mergenext(struct heapchunk_t *previous) {
	struct heapchunk_t *next = previous->meta.next_in_mem;
	if (!next) {
		return HEAPCHUNK_ERR;
	}
	previous->meta.next_in_mem = next->meta.next_in_mem;
	next->meta.next_in_mem->meta.prev_in_mem = previous;
	previous->size += next->size + sizeof(struct heapchunk_t);
	return HEAPCHUNK_OK;
}
