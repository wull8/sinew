#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* commands for the project
   -h : help
   -i <file> : insert file
   -cc : checkout the catalog
   -f <A> = <B> : find <key, value>
 */

const char * command[] = {
  "-h","-i","-cc","-f"
};

void help() {
  printf("usage:\n");
  printf("-h : help\n");
  printf("-i <file> : insert file\n");
  printf("-cc : checkout the catalog\n");
  printf("-f <A> = <B> : find <key,value>\n");
  printf("\n");
}

int main(int argc, char * argv[]) {

  printf("Simple Sinew System\n\n");
  
  if(argc <= 1) {
    printf("parameter -h for usage\n");
    exit(EXIT_FAILURE);
  }

  const char * Argument = argv[1];
  
  if(!strcmp(Argument, command[0])) {
    help();
    exit(EXIT_SUCCESS);
  } else if(!strcmp(Argument, command[1])) {
    const char * file = argv[2];
    if(file == NULL) {
      printf("No input file!\n");
      printf("parameter -h for usage\n");
      exit(EXIT_FAILURE);
    }
    printf("insert file '%s'\n", file);
    //Insert(file);
    exit(EXIT_SUCCESS);
  } else if(!strcmp(Argument, command[2])) {
    printf("checkout catolog\n");
    //CheckoutCatolog();
    exit(EXIT_SUCCESS);
  } else if(!strcmp(Argument, command[3])) {
    const char * A = argv[2];
    // argv[3] is '='
    const char * B = argv[4];
    if(A == NULL || B == NULL) {
      printf("Incomplete expression!\n");
      printf("parameter -h for usage\n");
      exit(EXIT_FAILURE);
    }
    printf("find %s = %s\n", A, B);
    // Find(A,B)
    exit(EXIT_SUCCESS);
  } else {
    printf("No such Command!\n");
    printf("parameter -h for usage\n");
    exit(EXIT_FAILURE);
  }
}
