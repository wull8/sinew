#include <stdio.h>
#include "Json.h"
#include "Global.h"

bool Serializer(char * json, json_parser * first, int json_len) {
  if(json == NULL || first == NULL) {
    return false;
  }

  json_parser * positioner = first;
  Serialize_Int32(json_len);

  while(positioner != NULL) {
    if()
  }
  
}
