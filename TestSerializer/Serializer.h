
#ifndef SERIALIZER
#define SERIALIZER
#include "Json.h"
#include "Global.h"
#include "Catalog.h"
#include <stdio.h>

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

typedef struct head2 {
  int tuple_id;
  int attr_num;
  data_unit * data_unit_array;
} head2;

bool JsonSerializer(char * json, json_parser * first, int json_len,
		    int * max_id, catalog_record ** CATALOG, FILE * file);
#endif

// the following functions are for tests
#ifndef TEST_SERIALIZER
#define TEST_SERIALIZER
int ArraySerializer(char * json, int start, int end,
		    int * max_id, catalog_record ** CATALOG, FILE * file);
int ObjectSerializer(char * json, int start, int end,
		    int * max_id, catalog_record ** CATALOG, FILE * file);
void write(head2 * Serializer, FILE * file);

void parseString(char * json, int start, int end, char ** destination);
int parseInt(char * json, int start, int end);
bool parseBool(char * json, int start);
int cmp( const void *a ,const void *b);
char** parseStringArray(char * json, int start, int end,
			int * count, int begin[1000], int finish[1000]);

#endif
