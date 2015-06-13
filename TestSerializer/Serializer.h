
#ifndef SERIALIZER
#define SERIALIZER
#include "Json.h"
#include "Global.h"

typedef struct head2 {
  int tuple_id;
  int attr_num;
  data_unit * data_unit_array;
} head2;

typedef struct data_unit {
  int attr_id;
  int offset;
  union {
    int INT_DATA;
    bool BOOL_DATA;
    char * STRING_DATA;
    int ARRAY_DATA;
    int OBJECT_DATA;
  } data;
  data_type dtype;
} data_unit;

bool JsonSerializer(char * json, json_parser * first, int json_len,
		    int * max_id, catalog_record ** CATALOG, FILE * file)
#endif
