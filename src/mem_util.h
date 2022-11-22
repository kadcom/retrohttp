#ifndef HTTP_MEM_UTIL_H
#define HTTP_MEM_UTIL_H 1 

#if defined (WIN32) || defined(_WIN32)
#include <windows.h>

static inline void mem_zero(void *p, const size_t len) {
  ZeroMemory(p, len);
}

static inline void mem_copy(void *dest, void *src, const size_t len) {
  CopyMemory(dest, src, len);
}

static inline void *mem_realloc(void *old_addr, const size_t new_len) {
  return VirtualAlloc(old_addr, new_len, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 
}


#else
#include <sys/mman.h>
#include <memory.h>

static inline void mem_zero(void *p, const size_t len) {
  memset(p, 0, len);
}

static inline void mem_copy(void *dest, void *src, const size_t len) {
  memmove(dest, src, len);
}

static inline void *mem_realloc(void *old_addr, const size_t new_len) {
  return mmap(old_addr, new_len, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 
      -1, 0);
}

#endif


#endif // HTTP_MEM_UTIL_H
