/* chunk.h
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
#ifndef _ARENA_CHUNK_H__
#define _ARENA_CHUNK_H__
#include <stdint.h>
#include <stdbool.h>
typedef enum heapchunk_e {
	HEAPCHUNK_OK = true,
	HEAPCHUNK_ERR = false,
} heapchunk_e;
struct heapchunk_t;
struct heap_meta {
	struct heapchunk_t *prev_in_mem;
	struct heapchunk_t *next_in_mem;
};
struct heapchunk_t {
	struct heap_meta meta;
	struct heapchunk_t *next_free;
	uint32_t size;
	bool isactive;
};
#define get_chunk_tag(ptr) &((struct heapchunk_t*)(ptr))[-1]
#define get_chunk_ptr(chunk) &((struct heapchunk_t*)chunk)[1]

heapchunk_e heapchunk_partition(struct heapchunk_t *block, uint32_t partsize, 
				struct heapchunk_t **newblock);
heapchunk_e heapchunk_mergenext(struct heapchunk_t *previous);

#endif // _ARENA_CHUNK_H__
// vim:set ft=c
