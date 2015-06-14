#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "Global.h"
#include "Serializer.h"
#include <time.h>

void test_parseString() {
  char * json = "\"hello world!\"";
  int start = 0, end = strlen(json) - 1;
  char * destination = NULL;
  parseString(json, start, end, &destination);
  printf("test_parseString: %s %s\n", json, destination);
  if(destination != NULL) {
    free(destination);
  }
}

void test_parseInt() {
  char * json = "12345";
  int start = 0, end = strlen(json) - 1;
  int res = parseInt(json, start, end);
  printf("test_parseInt: %s %d\n", json, res);
}

void test_parseBool() {
  char * json1 = "true";
  char * json2 = "false";
  bool x, y;
  x = parseBool(json1, 0);
  y = parseBool(json2, 0);
  printf("test_parseBool_true: %s %d\n", json1, x);
  printf("test_parseBool_false: %s %d\n", json2, y);
}

void test_parseArray() {
  char * json= "[\"abcdefgh\", \"1234567\", \"womens\"]";
  int start = 0, end = strlen(json) - 1;
  int count = 0;
  int begin[1000];
  int finish[1000];
  int i, j;
  
  char ** res;
  res = parseStringArray(json, start, end, &count, begin, finish);

  printf("test_parseArray: %s, string_num : %d\n", json, count);
  for(i = 0; i < count; i++) {
    printf("\t%s\n", res[i]);
  }

  for(i = 0; i < count; i++) {
    free(res[i]);
  }
  free(res);
}

void test_write() {
  FILE * file = fopen("test_write.bin", "ab+");
  head2 Serializer;
  Serializer.tuple_id = 1;
  Serializer.attr_num = 3;
  Serializer.data_unit_array = (data_unit *)malloc(sizeof(data_unit) * 4);
  int data1 = 132;
  bool data2 = true;
  char * data3 = "i love sysu";
  int offset;
  
  Serializer.offset = 9 * sizeof(int);
  offset = Serializer.offset;
  
  Serializer.data_unit_array[0].attr_id = 1;
  Serializer.data_unit_array[0].offset = offset + sizeof(data1);
  offset += sizeof(int);
  Serializer.data_unit_array[0].data.INT_DATA = data1;
  Serializer.data_unit_array[0].dtype = 0;

  Serializer.data_unit_array[1].attr_id = 2;
  Serializer.data_unit_array[1].offset = offset + sizeof(data2);
  offset += sizeof(bool);
  Serializer.data_unit_array[1].data.BOOL_DATA = data2;
  Serializer.data_unit_array[1].dtype = 1;

  Serializer.data_unit_array[2].attr_id = 3;
  Serializer.data_unit_array[2].offset = offset + sizeof(data3);
  offset += sizeof(data3);
  Serializer.data_unit_array[2].data.STRING_DATA = data3;
  Serializer.data_unit_array[2].dtype = 2;

  printf("test_write: \n");
  write(&Serializer, file);

  fclose(file);
}

void test_ArraySerializer() {
  char * json = "[\"abcdefgh\", \"1234567\", \"womens\"]";
  int start = 0, end = strlen(json)-1;
  int max_id = 0;
  catalog_record * CATALOG = NULL;
  FILE * file = fopen("test_ArraySerializer.bin", "ab+");
  printf("test_ArraySerializer: \n");
  ArraySerializer(json, start, end, &max_id, &CATALOG, file);
}

void test_JsonSerializer(char * json,  catalog_record ** CATALOG, FILE * file) {
  // char json[1000];
  // fgets(json, 1000, stdin);  
  json_parser * parser;
  int len = 0;
  parser = ParseJson(json, &len);
  static int max_id = 0;
  JsonSerializer(json, parser, len, &max_id, CATALOG, file);
}

void test_10w_data() {
  time_t clock_start, clock_end;
  char json[10000];
  catalog_record * CATALOG = NULL;
  int counter = 1;
  catalog_build(&CATALOG);
  FILE * file = fopen("test_JsonSerializer.txt", "w+");

  /*
  static char IOBUFFER[PAGE_SIZE];
  
  if (setvbuf(file, IOBUFFER, _IOFBF, PAGE_SIZE) != 0) {
    printf("failed to set up buffer for input file\n");
    } else {
    printf("buffer(8KB) set up for input file\n"); 
  }
  */

  clock_start = clock();
  while(fgets(json, 10000, stdin) != NULL) {
    test_JsonSerializer(json, &CATALOG, file);
    if (counter % 1000 == 0)
      printf("process at %d\n", counter);
    counter++;
  }
  clock_end = clock();

  printf("done!\n");
  printf("time waste: %.2fs\n", (double)(clock_end-clock_start)/CLOCKS_PER_SEC);
  
  catalog_save(CATALOG);
  fclose(file);
}

int main() {
  test_parseString();
  test_parseInt();
  test_parseBool();
  test_parseArray();
  test_write();
  test_ArraySerializer();
  //test_JsonSerializer();
  test_10w_data();
  return 0;
}

