#include <stdio.h>
#include "Json.h"
#include "Global.h"

void print(unsigned int start, unsigned int end, char * s) {
  int i;
  for(i = start; i <= end; i++) {
    printf("%c", s[i]);
  }
}

int main() {
  char Json[10000];
  fgets(Json, 10000, stdin);
  int len;
  //puts(Json);

  json_parser * Parser;
  Parser = ParseJson(Json, &len);
  
  json_parser * positioner = Parser;
  while(positioner!=NULL) {
    print(positioner->start,positioner->end, Json);
    printf(" %d", positioner->dtype);
    positioner = positioner->next;
    printf("\n");
  }
  
  return 0;
}
