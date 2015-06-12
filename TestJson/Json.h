
#ifndef JSONPARSER
#define JSONPARSER
typedef enum {atr = 0, value = 1} json_type;
typedef struct json_parser {
  unsigned int start;
  unsigned int end;
  json_type type;
  struct json_parser * next;
} json_parser;
extern json_parser * ParseJson(char * JsonStr, int * len);
extern void DeleteJson(json_parser * head);
#endif
