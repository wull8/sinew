#include <stdio.h>
#include "Catalog.h"
#include "Global.h"

int main() {

  char Key_name[100];
  char Key_Type[100];

  int n;

  catalog_record * CATALOG = NULL;
  
  catalog_build(&CATALOG);
  catalog_traversal(CATALOG);

  scanf("%s %s", Key_name, Key_Type);
  catalog_update(&CATALOG, Key_name, Key_Type);
  catalog_traversal(CATALOG);

  catalog_save(CATALOG);
  catalog_traversal(CATALOG);
    
  return 0;
}
