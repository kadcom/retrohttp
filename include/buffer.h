#ifndef MHTTP_BUFFER_H
#define MHTTP_BUFFER_H

int http_alloc_buffer(struct http_buffer_t *, const size_t len, const bool is_zero);
int http_free_buffer(struct http_buffer_t *);

#endif //MHTTP_BUFFER_H 
