#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H
#include <string.h>
#define MALLOC_FAILED EXIT_FAILURE
struct string {
  size_t len;
  char *ptr;
};

const char* easycat(const char *l, const char *h, size_t hlen);
void strinit(struct string *s);
struct string latestjson();

#endif // !JSON_HANDLER_H
