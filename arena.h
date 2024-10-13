/* arena.h
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
#ifndef _ARENA_HEAP_H__
#define _ARENA_HEAP_H__
#include "chunk.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#define PAGESIZE sysconf(_SC_PAGESIZE)

typedef struct heaparena_t {
	struct heapchunk_t *start;
	uint32_t avalible;
} heaparena_t;

typedef enum heaparena_e {
	HEAPARENA_OK = true,
	HEAPARENA_ERR = false,
} heaparena_e;

heaparena_e heaparena_init(heaparena_t *arena);
void *heaparena_alloc(heaparena_t *arena, size_t size);
void heaparena_free(heaparena_t *arena, void *ptr);
#endif // _ARENA_HEAP_H__
// vim:set ft=c
