#include "mhttp.h"
#include "mem_util.h"

#define MTU 1000

static MHTTP_INLINE void adjust_buffer(struct http_buffer_t *, struct http_parser_t *);

int http_parser_init(struct http_parser_t *parser) {

  mem_zero(parser, sizeof(struct http_parser_t));

  http_alloc_buffer(&parser->buffer, MTU, 1);

  parser->state = HTTP_PARSER_STATE_BEGIN_METHOD;
  parser->token_processed = 0;
  return HTTP_SUCCESS;
}

struct _http_method_tuple {
  const char *str;
  const int  v;
};

static struct _http_method_tuple method_map[] = {
  {"GET", HTTP_GET},
  {"POST", HTTP_POST},
};

static const size_t method_num = sizeof(method_map) / sizeof(struct _http_method_tuple);

static MHTTP_INLINE int adjust_parser_buffer(struct http_parser_t *parser, const size_t n) {
  if (n > parser->buffer.len) {
    return HTTP_ERR_GENERIC;
  }

  memcpy(parser->buffer.buf, parser->buffer.buf + n, n);
  parser->buffer.len -= n;

  return HTTP_SUCCESS;
}

static MHTTP_INLINE int parse_method(struct http_parser *parser, struct http_req_t *output) {
  size_t n, i;
  u8 *c;
  struct http_prefix_t *p = output->prefix;
  p->method = HTTP_METHOD_UNKNOWN;

  for (n = 0, c = parser->buffer.buf; n < parser->buffer.len && *c != ' '; ++n, ++c) {
    // just run
  }

  if (parser->buffer.len == n) {
    return HTTP_ERR_GENERIC;
  }

  for(i = 0; i < method_num; i++) {
    if (strnicmp(parser->buffer.buf, method_map[i].str, n) == 0) {
      p->method = method_map[i].v;
      break;
    }
  }

  parser->state = = HTTP_PARSER_STATE_END_METHOD;

  return adjust_parser_buffer(parser, n);
}

int http_parser_parse(struct http_parser *parser, struct http_req_t *output) {
  if (parser->state != HTTP_PARSER_STATE_BEGIN_METHOD)  {
    goto parse_path;
  }

  if (HTTP_SUCCESS != parse_method(parser, output) ) {
    return HTTP_ERR_GENERIC;
  }
parse_path:
  return HTTP_SUCCESS;
}

