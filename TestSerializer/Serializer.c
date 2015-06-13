#include <stdio.h>
#include <malloc.h>
#include "Serializer.h"
#include "Json.h"
#include "Global.h"
#include "Catalog.h"

// assume that all jsons have only one array of one degree
// assume that all arrays are string arrays

/*
int ArraySerializer(char * json, int start, int end, int * max_id, catalog_record ** CATALOG, FILE * file);
int ObjectSerializer(char * json, int start, int end, int * max_id, catalog_record ** CATALOG, FILE * file);
void write(head2 * Serializer, FILE * file);
*/

const char * data_types[] = {
  "int32",
  "bool",
  "string",
  "array",
  "object",
  "unknown"
};


void parseString(char * json, int start, int end, char ** destination) {
  (*destination) = (char *)malloc(sizeof(char) * (end - start - 1));
  int i, j;
  for(i = start+1, j = 0; i <end; i++, j++) {
    (*destination)[j] = json[i];
  }
}

int parseInt(char * json, int start, int end) {
  int ret = 0;
  int i;
  for(i = start; i <= end; i++) {
    ret *= 10;
    ret += json[i] - '0';
  }
  return ret;
}

bool parseBool(char * json, int start) {
  return json[start] == 't' || json[start] == 'T' ? 1 : 0;
}

int cmp( const void *a ,const void *b){
  return (*(data_unit *)a).attr_id > (*(data_unit *)b).attr_id ? 1 : -1;
}

bool JsonSerializer(char * json, json_parser * first, int json_len,
		    int * max_id, catalog_record ** CATALOG, FILE * file) {
  if(json == NULL || first == NULL) {
    return false;
  }

  head2 head2_data;
  (*max_id)++;
  head2_data.tuple_id = *max_id;
  head2_data.attr_num = json_len / 2;

  head2_data.data_unit_array = (data_unit *)malloc(sizeof(data_unit) * (int)(json_len / 2));
  data_unit * data_units = head2_data.data_unit_array;
  
  int i;
  json_parser * json_parser_positioner = first;
  for(i = 0; i < json_len /2 ; i++) {
    char * Key_name = NULL;
    const char * Key_Type = NULL;
    parseString(json, json_parser_positioner->start, json_parser_positioner->end, &Key_name);
    Key_Type = data_types[(int)(json_parser_positioner->dtype)];
    data_units[i].attr_id = catalog_update(CATALOG,Key_name,Key_Type);
    free(Key_name);
    Key_name = NULL;
    Key_Type = NULL;
    json_parser_positioner = json_parser_positioner->next;
    data_units[i].dtype = json_parser_positioner->dtype;
    switch(json_parser_positioner->dtype) {
    case 0 :
      data_units[i].data.INT_DATA = parseInt(json, json_parser_positioner->start, json_parser_positioner->end);
      break;
    case 1 :
      data_units[i].data.BOOL_DATA = parseBool(json, json_parser_positioner->start);
      break;
    case 2 :
      parseString(json, json_parser_positioner->start, json_parser_positioner->end,
		  &data_units[i].data.STRING_DATA);
      break;
    case 3 :
      data_units[i].data.ARRAY_DATA = ArraySerializer(json, json_parser_positioner->start,
						      json_parser_positioner->end, max_id, CATALOG, file);
      break;
    case 4 :
      data_units[i].data.OBJECT_DATA = ObjectSerializer(json, json_parser_positioner->start,
							json_parser_positioner->end, max_id, CATALOG, file);
      break;
    default:
      printf("Error @ Serializer:Serialize Unknown Type!\n");
      break;
    }
    json_parser_positioner = json_parser_positioner->next;
  }

  qsort(data_units, json_len / 2, sizeof(data_unit), cmp);
  
  head2_data.data_unit_array = data_units;

  write(&head2_data, file);
}

char** parseStringArray(char * json, int start, int end, int * count, int begin[1000], int finish[1000]) {
  
  int positioner = start;
  *count = 0;
  
  for(;positioner <= end; positioner++) {
    if(json[positioner] == '"') {
      begin[*count] = positioner+1;
      while(json[++positioner] != '"')
	;
      finish[*count] = positioner-1;
      positioner++;
      *count++;
    }
  }

  char ** strings = (char **)malloc(sizeof(char *) * (*count));

  int i, j;
  for(i = 0; i < *count; i++) {
    if(begin[i] == finish[i]) {
      strings[i] = NULL;
      continue;
    }
    strings[i] = malloc(sizeof(char) * (finish[i] - begin[i] + 1));
    for(j = begin[i]; j <= finish[i]; j++) {
      strings[i][j] = json[j];
    }
  }

  return strings;
}

int ArraySerializer(char * json, int start, int end, int * max_id, catalog_record ** CATALOG, FILE * file) {
  char ** strings;
  int string_num;
  int begin[1000], finish[1000];
  strings = parseStringArray(json, start, end, &string_num, begin, finish);
  head2 head2_data;
  (*max_id)++;
  head2_data.tuple_id = *max_id;
  head2_data.attr_num = string_num;

  head2_data.data_unit_array = (data_unit *)malloc(sizeof(data_unit) * (string_num + 1));
  data_unit * data_units = head2_data.data_unit_array;

  int i;
  int offset = 0;
  for(i = 0; i < string_num; i++) {
    data_units[i].dtype = 2;
    data_units[i].attr_id = -1;
    data_units[i].data.STRING_DATA = strings[i];
    data_units[i].offset = offset;
    offset += finish[i] -begin[i];
  }

  data_units[string_num].offset = offset;

  write(&head2_data, file);
  
  return *max_id;
}

int ObjectSerializer(char * json, int start, int end, int * max_id, catalog_record ** CATALOG, FILE * file) {
  char * nested_json;
  nested_json = (char *)malloc(sizeof(char) * (end - start + 1));
  int i, j;
  for(i = start, j = 0; i <= end; i++, j++) {
    nested_json[j] = json[i];
  }
  json_parser * nested_json_parser;
  int nested_json_len;
  nested_json_parser = ParseJson(nested_json, &nested_json_len);
  JsonSerializer(nested_json, nested_json_parser, nested_json_len, max_id,CATALOG, file);
}

void write(head2 * Serializer, FILE * file) {
  /*
  static char IOBUFFER[PAGE_SIZE];

  // 8k buffer
  if (setvbuf(file, IOBUFFER, _IOFBF, PAGE_SIZE) != 0) {
    printf("failed to set up buffer for input file\n");
  } else {
    printf("buffer set up for input file\n"); 
  }
  */

  fwrite(&(Serializer->tuple_id), sizeof(int), 1, file);
  fwrite(&(Serializer->attr_num), sizeof(int), 1, file);

  int i;
  int len = Serializer->attr_num + 1;
  for(i = 0; i < len; i++) {
    fwrite(&(Serializer->data_unit_array[i].attr_id), sizeof(int), 1, file);
  }

  for(i = 0; i < len + 1; i++) {
    fwrite(&(Serializer->data_unit_array[i].offset), sizeof(int), 1, file);
  }

  for(i = 0; i < len; i++) {
    switch(Serializer->data_unit_array[i].dtype) {
    case 0 : case 3 : case 4 :
      fwrite(&(Serializer->data_unit_array[i].data), sizeof(int), 1, file);
      break;
    case 1 :
      fwrite(&(Serializer->data_unit_array[i].data), sizeof(bool), 1, file);
      break;
    case 2 :
      fwrite(&(Serializer->data_unit_array[i].data), sizeof(char), sizeof(Serializer->data_unit_array[i].data.STRING_DATA), file);
      break;
    }
  }
}
