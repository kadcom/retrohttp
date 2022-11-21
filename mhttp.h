#ifndef MHTTP_H 
#define MHTTP_H 

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#include <strsafe.h>
#endif

#include <string.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

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
  HTTP_PARSER_STATE_METHOD, 
  HTTP_PARSER_STATE_PATH, 
  HTTP_PARSER_STATE_VERSION,
  HTTP_PARSER_STATE_HEADERS,
};

enum {
  HTTP_SUCCESS     = 0,
  HTTP_ERR_GENERIC = -1,
};

struct http_parser_t {
  int state;
};

struct http_buffer_t {
  byte_t *buf;
  size_t len;
  size_t size;
};

#define HTTP_MAX_PATH 2048
struct http_prefix_t {
  char path[HTTP_MAX_PATH];
  int method;
  int version;

  struct http_buffer_t headers;
};

#include "buffer.h"

#endif  /* MHTTP_H */
