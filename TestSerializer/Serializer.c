#include <stdio.h>
#include <malloc.h>
#include "Json.h"
#include "Global.h"
#include "Catalog.h"

// assume that all jsons have only one nested object or arrays of one degree
// assume that all arrays are string arrays

int ArraySerializer(char * json, int start, int end, int * max_id, catalog_record * CATALOG);
int ObjectSerializer(char * json, int start, int end, int * max_id, catalog_record * CATALOG);

const char * data_type = {
  "int32",
  "bool",
  "string",
  "array",
  "object",
  "unknown"
}


void parseString(char * json, int start, int end, char * destination) {
  destination = (char *)malloc(sizeof(char) * (end - start - 1));
  int i, j;
  for(i = start+1, j = 0; i <end; i++, j++) {
    destination[j] = json_parser[i];
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

bool Serializer(char * json, json_parser * first, int json_len, int * max_id, catalog_record ** CATALOG) {
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
    char * Key_Type = NULL;
    parseString(json, json_parser_positioner->start, json_parser_positioner->end, Key_name);
    Key_Type = data_type[json_parser_positioner->dtype];
    data_units[i].attr_id = catalog_update(catalog_record ** CATALOG, char * Key_name, char * Key_Type);
    free(Key_name);
    Key_name = NULL;
    Key_Type - NULL;
    json_parser_positioner = json_parser_positioner->next;
    data_units[i].data_type = json_parser_positioner->dtype;
    switch(json_parser_positioner->dtype) {
    case 0 :
      data_units[i].data.INT_DATA = parseInt(json, json_parser_positioner->start, json_parser_positioner->end);
      break;
    case 1 :
      data_units[i].data.BOOL_DATA = parseBool(json, json_parser_positioner->start);
      break;
    case 2 :
      parseString(json, json_parser_positioner->start, json_parser_positioner->end,
		  data_units[i].data.STRING_DATA);
      break;
    case 3 :
      data_unit[i].data.ARRAY_DATA = ArraySerializer(json, json_parser_positioner->start,
						     json_parser_positioner->end, max_id, CATALOG);
      break;
    case 4 :
      data_unit[i].data.OBJECT_DAT = ObjectSerializer(json, json_parser_positioner->start,
						       json_parser_positioner->end, max_id, CATALOG);
      break;
    default:
      printf("Error @ Serializer:Serialize Unknown Type!\n");
      break;
    }
    json_parser_positioner = json_parser_positioner->next;
  }


  write(&head2_data);
}


