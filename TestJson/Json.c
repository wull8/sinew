#include <stdio.h>
#include <string.h>
#include "Json.h"
#include <malloc.h>

json_parser * ParseJson(char * JsonStr) {

  if(JsonStr == NULL)
    return NULL;

  // check_json();
  
  int i = 2;
  int length = strlen(JsonStr);

  json_parser * head;

  if(JsonStr[0] == '{' && JsonStr[1] == '"') {
    head = (json_parser *)malloc(sizeof(json_parser));
    head->start = 1;
    while(JsonStr[i] != '"')
      i++;
    head->end = i;
    head->next = NULL;
    head->type = 0;
  } else {
    return NULL;
  }

  json_parser * positioner = head;
  json_parser * temp = NULL;
  
  for(i; i < length; i++) {
    if(JsonStr[i] == ':') {
      i++;
      while(JsonStr[i] == ' ') {
	i++;
      }
      
      temp = (json_parser *)malloc(sizeof(json_parser));
      temp->start = i;
      
      if(JsonStr[i] == '{') {
	while(JsonStr[i] != '}')
	  i++;
	i++;
      } else if (JsonStr[i] == '[') {
	while(JsonStr[i] != ']')
	  i++;
	i++;
      } else {
	while(JsonStr[i] != ',' && JsonStr[i] != '}')
	  i++;
      }
      
      temp->end = i - 1;
      temp->type = 1;
      temp->next = NULL;
      positioner->next = temp;
      positioner = temp;
      i--;
      continue;
    } else if(JsonStr[i] == ',') {
      i++;
      while(JsonStr[i] == ' ') {
	i++;
      }
      
      temp = (json_parser *)malloc(sizeof(json_parser));
      temp->start = i;
      while(JsonStr[i] != ':')
	i++;
      temp->end = i - 1;
      temp->type = 0;
      temp->next = NULL;
      positioner->next = temp;
      positioner = temp;
      i--;
      continue;
      
    }
  }

  return head;
}

char * RestoreJson(char * datas) {

}
