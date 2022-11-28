#ifndef MHTTP_BUFFER_H
#define MHTTP_BUFFER_H
#include <stddef.h>

struct http_buffer_t;

int http_alloc_buffer(struct http_buffer_t *, const size_t len);
int http_grow_buffer(struct http_buffer_t *, const size_t newlen);
int http_attach_buffer(struct http_buffer_t *, void *src, const size_t len);
int http_detach_buffer(struct http_buffer_t *buf, void **dest, size_t *length, size_t *size);
int http_free_buffer(struct http_buffer_t *);
int http_append_buffer(struct http_buffer_t *buf, void *src, const size_t len);
int http_append_buffer_with_buffer(struct http_buffer_t *dst, struct http_buffer_t *src);
size_t http_slack_buffer(struct http_buffer_t *);
int http_is_full_buffer(struct http_buffer_t *);

#endif //MHTTP_BUFFER_H 
