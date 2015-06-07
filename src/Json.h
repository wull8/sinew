
#ifndef JSONPARSER
#def JSONPARSER
typedef struct json_parser {
  char * position;
  int offset;
  struct json_parser * next;
} json_parser;
#include "Catalog.h"
json_parser * ParseJson(char * JsonStr);
char * RestoreJson(char * datas);
#endif
