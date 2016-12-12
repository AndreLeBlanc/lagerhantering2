#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "alloc_check.h"
#include "bst.h"
#include "pallet.h"
#include "storage.h"

void* pallet_key_generator (void* data)
{
  char* name = getName((GoodsT*)data);
  char* key  = calloc(strlen(name)+1, sizeof(char));
  check_allocation(key);
  strcpy(key, name);
  return key;
}

int pallet_key_comparator (void* key1_in, void* key2_in)
{
  int result = 0;
  char* key1 = (char*) key1_in;
  char* key2 = (char*) key2_in;
  result = strcmp (key1, key2);
  
  #ifdef DEBUGHIGH
  printf("strcmp: %s is %d than %s\n", key1, result, key2);
  #endif

  return result;
}

void pallet_key_freer (void* key)
{
  assert(key);
  free(key);
}

void* pallet_data_copier(void* data) { return (void*)getGoodsCpyPtr((GoodsT*)data); }

void pallet_data_freer (void* data) { freeGoods((GoodsT*)data); }

set_t * create_pallet(){
  return new_set(&pallet_key_generator, &pallet_key_comparator, &pallet_key_freer, &pallet_data_copier, &pallet_data_freer);
}

