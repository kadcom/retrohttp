#if defined (WIN32) || defined(_WIN32)
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

#include "mhttp.h"
#include "mem_util.h"
#include "buffer.h"


#define DEFAULT_PAGE_SIZE 4096

#if defined(_WIN32) || defined(WIN32)
static size_t RHTTP_INLINE getpagesize() {
  SYSTEM_INFO si;
  GetSystemInfo(&si);

  return si.dwPageSize;
}
#endif

static size_t RHTTP_INLINE aligned_size(const size_t len) {
  size_t pg_size = getpagesize();
  if (0 == len) {
    return 0;
  }

  /* align len to nearest page size */
  return (len + (pg_size - 1)) & ~(pg_size - 1);
}


int http_alloc_buffer(struct http_buffer_t *buf, const size_t len) {
  const size_t aligned = aligned_size(len);
  void *arena;
#if defined(WIN32) || defined(_WIN32)
  arena = VirtualAlloc(NULL, aligned, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 
#else
  arena = mmap(NULL, aligned, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (MAP_FAILED == arena) {
    return HTTP_ERR_GENERIC;
  }

  mem_zero(arena, aligned);
#endif

  buf->buf = arena; 
  buf->size = aligned; 
  buf->len = len;

  return HTTP_SUCCESS;
}

int http_grow_buffer(struct http_buffer_t *buf, const size_t new_len) {
  const size_t aligned = aligned_size(new_len);
  void *arena;

  if (aligned <= buf->size) {
    buf->len = new_len;
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
  buf->len = new_len;

  return HTTP_SUCCESS;
}

int http_attach_buffer(struct http_buffer_t *buf, void *src, const size_t len) {
  buf->buf = src; 
  buf->len = len; 

  /* Check if the attached buffer length is aligned */
  if (len % getpagesize() == 0) {
    buf->size = len;
    return HTTP_SUCCESS; 
  }

  buf->size = aligned_size(len);
  return http_grow_buffer(buf, len);
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

int http_detach_buffer(struct http_buffer_t *buf, void **dest, size_t *length, size_t *size) {
  if (0 == buf) {
    return HTTP_ERR_GENERIC;
  }

  if (0 != dest) {
    *dest = buf->buf;
  }

  if (0 != length) {
    *length = buf->len;
  }

  if (0 != size) {
    *size = buf->size;
  }

  mem_zero(buf, sizeof(struct http_buffer_t));

  return HTTP_SUCCESS;
}

int http_append_buffer(struct http_buffer_t *buf, void *src, const size_t len) {
  size_t total_size = buf->len + len;
  size_t aligned_total_size = aligned_size(total_size);

  if (aligned_total_size <= buf->size) {
    goto append_copy_data;
  }

  if (http_grow_buffer(buf, total_size) != HTTP_SUCCESS) {
    return HTTP_ERR_GENERIC;
  }
append_copy_data:
  mem_copy(buf->buf + buf->len, src, len);
  return HTTP_SUCCESS;
}

int http_append_buffer_with_buffer(struct http_buffer_t *dst, struct http_buffer_t *src) {
  return http_append_buffer(dst, src->buf, src->len);
}


size_t http_slack_buffer(struct http_buffer_t *buf) {
  return buf->size - buf->len;
}

int http_is_full_buffer(struct http_buffer_t *buf) {
  return http_slack_buffer(buf) == 0;
}
