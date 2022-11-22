#include "mhttp.h"

#if defined (WIN32) || defined(_WIN32)
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

#define DEFAULT_PAGE_SIZE 4096

#if defined(_WIN32) || defined(WIN32)
static inline size_t getpagesize() {
  SYSTEM_INFO si;
  GetSystemInfo(&si);

  return si.dwPageSize;
}
#endif

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
#if defined(WIN32) || defined(_WIN32)
  arena = VirtualAlloc(NULL, aligned, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 
#else
  arena = mmap(NULL, aligned, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (MAP_FAILED == arena) {
    return HTTP_ERR_GENERIC;
  }

  memset(arena, 0, aligned);
#endif
  buf->buf = arena; 
  buf->size = aligned; 
  buf->len = len;

  return HTTP_SUCCESS;
}

int http_grow_buffer(struct http_buffer_t *buf, const size_t bylen) {
  const size_t aligned = aligned_size(buf->len + bylen);
  void *arena;

  if (aligned < buf->size) {
    /* do nothing, buffer still has some size */ 
    return HTTP_SUCCESS;
  }

#if defined(WIN32) || defined(_WIN32)
  arena = VirtualAlloc(buf->buf, aligned, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  
  if (NULL == arena) {
    return HTTP_ERR_GENERIC;
  }

#else
  arena = mmap(buf->buf, aligned, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (MAP_FAILED == arena) {
    return HTTP_ERR_GENERIC;
  }
#endif

  buf->buf = arena; 
  buf->size = aligned; 
  buf->len += bylen;

  return HTTP_SUCCESS;
}

int http_free_buffer(struct http_buffer_t *buf) {
  int ret;
#if defined(WIN32) || defined(_WIN32)
  VirtualFree(buf->buf, 0, MEM_RELEASE);
#else
  ret = munmap(buf->buf, buf->size);
#endif

  if (0 != ret) {
    return HTTP_ERR_GENERIC;
  }

  memset(buf, 0, sizeof(struct http_buffer_t));

  return HTTP_SUCCESS;
}
