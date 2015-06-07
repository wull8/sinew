#include <stdio.h>
#include "Json.h"

void print(unsigned int start, unsigned int end, char * s) {
  int i;
  for(i = start; i <= end; i++) {
    printf("%c", s[i]);
  }
  printf("\n");
}

int main() {
  char Json[10000];
  fgets(Json, 10000, stdin);
  //puts(Json);

  json_parser * Parser;
  Parser = ParseJson(Json);
  
  json_parser * positioner = Parser;
  while(positioner!=NULL) {
    print(positioner->start,positioner->end, Json);
    positioner = positioner->next;
  }
  
  return 0;
}
