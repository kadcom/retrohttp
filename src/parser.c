#include "mhttp.h"
#include "mem_util.h"

#define MTU 1000

static inline void adjust_buffer(struct http_buffer_t *, struct http_parser_t *);

int http_parser_init(struct http_parser_t *parser) {

  mem_zero(parser, sizeof(struct http_parser_t));

  http_alloc_buffer(&parser->current, MTU, true);
  http_alloc_buffer(&parser->left, MTU, true);

  parser->state = HTTP_PARSER_STATE_BEGIN_METHOD;
  parser->token_processed = 0;
  return HTTP_SUCCESS;
}

int fill_parser_buffer(struct http_parser_t *parser, void *buf, const size_t len) {
  void *ptr = &parser->current.buf + (parser->current.len - parser->leftovers);
  mem_copy(parser->left.buf, ptr, parser->leftovers);
  parser->left.len = parser->leftovers;
  mem_copy(parser->buffer.buf, buf, len);
  return HTTP_SUCCESS;
}


