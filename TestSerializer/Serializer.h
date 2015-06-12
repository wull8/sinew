
#ifndef SERIALIZER
#define SERIALIZER
#include "Json.h"
#include "Global.h"

typedef struct head2 {
  int_32 tuple_id;
  int_32 attr_num;
  data_unit * data_unit_array;
} head2;

typedef struct data_unit {
  int_32 attr_id;
  int_32 offset;
  union {
    int_32 INT_DATA;
    bool BOOL_DATA;
    char * STRING_DATA;
    int_32 ARRAY_DATA;
    int_32 OBJECT_DATA;
  } data;
  data_type dtype;
} data_unit;

bool JsonSerializer(char * json, json_parser * first, int json_len);
#endif
