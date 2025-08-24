#pragma once
#include <stddef.h>

#define da_reserve(da, expected_capacity)                                                  \
	do {                                                                                   \
		if ((expected_capacity) > (da)->capacity) {                                        \
			if ((da)->capacity == 0) {                                                     \
				(da)->capacity = 256;                                                      \
			}                                                                              \
			while ((expected_capacity) > (da)->capacity) {                                 \
				(da)->capacity *= 2;                                                       \
			}                                                                              \
			(da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));     \
			assert((da)->items != NULL);                                                   \
		}                                                                                  \
	} while (0)

#define da_append(da, item)                    \
	do {                                       \
		da_reserve((da), (da)->count + 1);     \
		(da)->items[(da)->count++] = (item);   \
	} while (0)

#define da_append_many(da, new_items, new_items_count)                                          \
	do {                                                                                        \
		da_reserve((da), (da)->count + (new_items_count));                                      \
		memcpy((da)->items + (da)->count, (new_items), (new_items_count)*sizeof(*(da)->items)); \
		(da)->count += (new_items_count);                                                       \
	} while (0)

#define da_append_cstr(da, cstr) da_append_many((da), (cstr), strlen((cstr)))
#define da_append_sv(da, sv)     da_append_many((da), (sv)->items, (sv)->count)
#define da_append_token(da, tok) da_append_sv  ((da), &(tok).text)

typedef struct {
	char *items;
	size_t count;
	size_t capacity;
} StringBuilder;

typedef struct {
	char *items;
	size_t count;
} StringView;


