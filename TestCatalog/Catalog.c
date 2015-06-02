#include "Catalog.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "Global.h"

bool catalog_inc(catalog_record ** head, char * Key_name, char * Key_Type) {
  typedef catalog_record node;
  node * positioner = *head;
  printf("%s %s\n", positioner->Key_name, positioner->Key_Type);
  
  while(positioner != NULL) {
    if(strcmp(positioner->Key_name, Key_name) == 0 && strcmp(positioner->Key_Type, Key_Type) == 0)
      break;
    positioner = positioner->next;
  }

  if(positioner == NULL) {
    return false;
  }

  positioner->count++;
  positioner->dirty = true;
  return true;
}

void catalog_append(catalog_record ** head, int _id, char * Key_name,
		    char * Key_Type, int count, bool dirty) {
  typedef catalog_record node;		   
  
  if((*head) == NULL) {
    (*head) = (node *)malloc(sizeof(node));
    (*head)->_id = (_id == -1 ? 1 : _id);
    strcpy((*head)->Key_name, Key_name);
    strcpy((*head)->Key_Type, Key_Type);
    (*head)->count = count;
    (*head)->dirty = dirty;
    (*head)->next = NULL;
    return;
  }

  if(catalog_inc(head, Key_name, Key_Type)) {
    return;
  }
  
  node * positioner = *head;
  while(positioner->next != NULL) {
    positioner = positioner->next;
  }
  positioner->next = (node *)malloc(sizeof(node));
  positioner->next->_id = (_id == -1 ? positioner->_id+1 : _id);
  strcpy(positioner->next->Key_name, Key_name);
  strcpy(positioner->next->Key_Type, Key_Type);
  positioner->next->count = count;
  positioner->next->dirty = dirty;
  positioner->next->next = NULL;
  return;
}

void catalog_traversal(catalog_record * head) {
  typedef catalog_record node;
  node * positioner = head;
  if(head == NULL) {
    return;
  }

  printf("_id\tKey_name\tKey_type\tcount\n");

  while(positioner != NULL) {
    printf("%d\t%s\t\t%s\t\t%d\n", positioner->_id, positioner->Key_name,
	   positioner->Key_Type, positioner->count);
    positioner = positioner->next;
  }
  
}

bool catalog_update(catalog_record ** CATALOG, char * Key_name, char * Key_Type) {
  if(Key_name == NULL || Key_Type == NULL) {
    return false;
  }

  catalog_append(CATALOG, -1, Key_name, Key_Type, 1, true);

  if(*CATALOG == NULL) {
    return false;
  }

  return true;
}

bool catalog_build(catalog_record ** CATALOG) {
  FILE * F_Catalog = fopen(CATALOG_FILE, "r");
  catalog_record temp;
  
  if(F_Catalog == NULL) {
    return false;
  }

  while(fscanf(F_Catalog, "%d%s%s%d", &temp._id, temp.Key_name, temp.Key_Type, &temp.count) != EOF) {
    catalog_append(CATALOG, temp._id, temp.Key_name, temp.Key_Type, temp.count, false);
  }
  
  fclose(F_Catalog);
}

bool catalog_save(catalog_record * CATALOG) {
  FILE * F_Catalog = fopen(CATALOG_FILE, "w+");

  if(F_Catalog == NULL) {
    return false;
  }

  catalog_record * positioner = CATALOG;
  
  while(positioner != NULL) {
    fprintf(F_Catalog, "%d %s %s %d\n",positioner->_id, positioner->Key_name,
	    positioner->Key_Type, positioner->count);
    positioner = positioner->next;
  }

  return true;
}

