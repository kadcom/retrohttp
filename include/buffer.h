#ifndef MHTTP_BUFFER_H
#define MHTTP_BUFFER_H
#include <stddef.h>

struct http_buffer_t;

int http_alloc_buffer(struct http_buffer_t *, const size_t len, const int is_zero);
int http_grow_buffer(struct http_buffer_t *, const size_t bylen);
int http_free_buffer(struct http_buffer_t *);

#endif //MHTTP_BUFFER_H 
