#include "mhttp.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

#define DEFAULT_PAGE_SIZE 4096

static inline size_t aligned_size(const size_t len) {
  size_t pg_size = getpagesize();
  if (0 == len) {
    return 0;
  }

  /* align len to nearest page size */
  return (len + (pg_size - 1)) & ~(pg_size - 1);
}

int http_alloc_buffer(struct http_buffer_t *buf, const size_t len, const bool is_zero) {
  const size_t aligned = aligned_size(len);
  void *arena;
  arena = mmap(NULL, aligned, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (MAP_FAILED == arena) {
    return HTTP_ERR_GENERIC;
  }

  memset(arena, 0, aligned);

  buf->buf = arena; 
  buf->size = aligned; 
  buf->len = len;

  return HTTP_SUCCESS;
}

int http_grow_buffer(struct http_buffer_t *buf, const size_t bylen) {
  const size_t aligned = aligned_size(buf->len + bylen);
  if (aligned < buf->size) {
    /* do nothing, buffer still has some size */ 
    return HTTP_SUCCESS;
  }

  void *arena;
  arena = mmap(buf->buf, aligned, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (MAP_FAILED == arena) {
    return HTTP_ERR_GENERIC;
  }

  buf->buf = arena; 
  buf->size = aligned; 
  buf->len += bylen;

  return HTTP_SUCCESS;
}

int http_free_buffer(struct http_buffer_t *buf) {
  int ret;

  ret = munmap(buf->buf, buf->size);

  if (0 != ret) {
    return HTTP_ERR_GENERIC;
  }

  memset(buf, 0, sizeof(struct http_buffer_t));

  return HTTP_SUCCESS;
}
