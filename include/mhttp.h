#ifndef MHTTP_H 
#define MHTTP_H 

#if defined(_MSC_VER)
#if (_MSC_VER <= 1200)
#define RHTTP_INLINE __inline 
#else 
#define RHTTP_INLINE inline 
#endif
#else
#define RHTTP_INLINE inline
#endif

#if defined(WIN32) || defined(_WIN32)
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stddef.h>

#ifndef byte_t 
typedef unsigned char byte_t; 
#endif

#define HTTP_RET_ERR(x) ((x) < 0)

enum {
  HTTP_VERSION_1_0, 
  HTTP_VERSION_1_1, 
  HTTP_VERSION_0_9,
};

enum {
  HTTP_METHOD_UNKNOWN,
  HTTP_METHOD_GET,
  HTTP_METHOD_POST, 
  HTTP_METHOD_PATCH, 
  HTTP_METHOD_PUT,
  HTTP_METHOD_OPTION,
};

enum {
  HTTP_PARSER_STATE_BEGIN_METHOD = 0,
  HTTP_PARSER_STATE_END_METHOD, 
  HTTP_PARSER_STATE_BEGIN_PATH, 
  HTTP_PARSER_STATE_END_PATH,
  HTTP_PARSER_STATE_BEGIN_VERSION,
  HTTP_PARSER_STATE_END_VERSION,
  HTTP_PARSER_STATE_BEGIN_HEADERS,
  HTTP_PARSER_STATE_END_HEADERS,
};

enum {
  HTTP_SUCCESS     = 0,
  HTTP_ERR_GENERIC = -1,
};

struct http_buffer_t {
  byte_t *buf;
  size_t len;
  size_t size;
};


struct http_parser_t {
  int state;
  size_t token_processed;

  struct http_buffer_t buffer; 
};

#define HTTP_MAX_PATH 2048
struct http_prefix_t {
  char path[HTTP_MAX_PATH];
  int method;
  int version;

  struct http_buffer_t headers;
};

struct http_req_t {
  struct http_prefix_t prefix;
  struct http_buffer_t entity;
};

#include "buffer.h"

#endif  /* MHTTP_H */
