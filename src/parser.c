#include "mhttp.h"
#include "mem_util.h"

#define MTU 1000

static MHTTP_INLINE void adjust_buffer(struct http_buffer_t *, struct http_parser_t *);

int http_parser_init(struct http_parser_t *parser) {

  mem_zero(parser, sizeof(struct http_parser_t));

  http_alloc_buffer(&parser->current, MTU, 1);
  http_alloc_buffer(&parser->left, MTU, 1);

  parser->state = HTTP_PARSER_STATE_BEGIN_METHOD;
  parser->token_processed = 0;
  return HTTP_SUCCESS;
}

int fill_parser_buffer(struct http_parser_t *parser, void *buf, const size_t len) {
  void *ptr = &parser->current.buf + (parser->current.len - parser->leftovers);
  mem_copy(parser->left.buf, ptr, parser->leftovers);
  parser->left.len = parser->leftovers;
  mem_copy(parser->current.buf, buf, len);
  return HTTP_SUCCESS;
}


