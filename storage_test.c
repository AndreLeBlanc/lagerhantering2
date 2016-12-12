#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "storage.h"

////

void test_validName ()
{
  // valid
  CU_ASSERT_TRUE(validName("a"));
  CU_ASSERT_TRUE(validName("abcdefghijklmnopqrstuvwxyz"));
  CU_ASSERT_TRUE(validName("aabcdefghijklmnopqrstuvwxyz0"));
  CU_ASSERT_TRUE(validName("abcdefghijklmnopqrstuvwxyz0123456789"));
  CU_ASSERT_TRUE(validName("0"));
  CU_ASSERT_TRUE(validName("abcdefghijklmnopqrstuvwxyz"));
  CU_ASSERT_TRUE(validName("0aabcdefghijklmnopqrstuvwxyz"));
  CU_ASSERT_TRUE(validName("0123456789abcdefghijklmnopqrstuvwxyz"));
  // invalid
  CU_ASSERT_FALSE(validName(""));
  CU_ASSERT_FALSE(validName(0));
}

void test_validPrice()
{
  // valid
  CU_ASSERT_TRUE(validPrice(0));
  CU_ASSERT_TRUE(validPrice(01));
  CU_ASSERT_TRUE(validPrice(1));
  CU_ASSERT_TRUE(validPrice(1234567890));
  // invalid
  CU_ASSERT_FALSE(validPrice(-01));
  CU_ASSERT_FALSE(validPrice(-1));
  CU_ASSERT_FALSE(validPrice(-1234567890));
}

void test_validAmount()
{
  // valid
  CU_ASSERT_TRUE(validAmount(0));
  CU_ASSERT_TRUE(validAmount(01));
  CU_ASSERT_TRUE(validAmount(1));
  CU_ASSERT_TRUE(validAmount(1234567890));
  // invalid
  CU_ASSERT_FALSE(validAmount(-01));
  CU_ASSERT_FALSE(validAmount(-1));
  CU_ASSERT_FALSE(validAmount(-1234567890));
}

////
void test_createStore_freeStore ()
{
  // declare and initialize store
  StoreT* s = createStore();
  // free store
  freeStore(s);
  // null arg
  freeStore(NULL);
}

void test_createGoods_freeGoods ()
{
  // create and name goods
  GoodsT* g = createGoods("a");
  // check if name as expected
  CU_ASSERT_TRUE(strcmp(getName(g),"a") == 0);
  // free goods
  freeGoods(g);
  // null arg
  freeGoods(NULL);
}

////
void test_store_insert_goods()
{
  StoreT* s = createStore();

  // null arg
  store_insert_goods(NULL, "name1", "desc1", 0, false);
  // testing correct insert
  store_insert_goods(s, "name1", "desc1", 0, false);
  // testing incorrect inserts
  store_insert_goods(s, "", "red", 0, false);
  store_insert_goods(s, "name2", "", 2, false);
  store_insert_goods(s, "name3", "desc3", -1, false);
  // inserting taken goods
  store_insert_goods(s, "name1", "desc1", 2, false);
  // big int
  store_insert_goods(s, "name4", "desc4", 999999999, false);
  freeStore(s);
}

void test_store_remove_goods()
{
  StoreT* s = createStore();

  // null arg
  store_remove_goods(NULL, "name1", false);
  // removing from empty store
  store_remove_goods(s, "name1", false); 
  // populating store
  store_insert_goods(s, "name1", "desc1", 0, false);
  store_insert_goods(s, "name2", "desc1", 0, false);
  // removing non-existing goods
  store_remove_goods(s, "name3", false);
  // removing existing goods
  store_remove_goods(s, "name1", false);
  store_remove_goods(s, "name2", false);
  // removing from empty store
  store_remove_goods(s, "name1", false);

  freeStore(s);
}

////
void test_goods_set_name()
{
  StoreT* s = createStore();

  // null arg
  goods_set_name(NULL, "name1", "new_name1_empty_store");
  // setting in empty store
    goods_set_name(s, "name1", "new_name1_empty_store");  
  // setting incorrectly in empty store
  goods_set_name(s, "name1", "");
  goods_set_name(s, "", "new_name");
  // populating store
  store_insert_goods(s, "name1", "desc1", 0, false);
  store_insert_goods(s, "name2", "desc1", 0, false);
  // setting non-existing goods
  goods_set_name(s, "name3", "new_name3_non_existing");
  // setting incorrectly
  goods_set_name(s, "name1", "");
  goods_set_name(s, "", "new_name");
  // settings existing goods
  goods_set_name(s, "name1", "new_name1_existing"); // todo mem leak
  goods_set_name(s, "name2", "new_name2_existing"); // todo mem leak
  // settings existing goods again
  goods_set_name(s, "name1", "new_name1_existing_again");

  freeStore(s);
}

void test_goods_set_price()
{
  StoreT* s = createStore();

  // null arg
  goods_set_price(NULL, "name1", 10); 
  // setting in empty store
  goods_set_price(s, "name1", 10);  
  // setting incorrectly in empty store
  goods_set_price(s, "name1", 11);  
  goods_set_price(s, "", 12);  
  // populating store
  store_insert_goods(s, "name1", "desc1", 0, false);
  store_insert_goods(s, "name2", "desc1", 0, false);
  // setting non-existing goods
  goods_set_price(s, "name3", 21);
  // setting incorrectly
  goods_set_price(s, "name1", -1);  
  goods_set_price(s, "", 14);
  // settings existing goods
  goods_set_price(s, "name1", 15);
  goods_set_price(s, "name2", 16);
  // settings existing goods again
  goods_set_price(s, "name1", 17);

  freeStore(s);
}

void test_goods_set_description()
{
  StoreT* s = createStore();

  // null arg
  goods_set_description(NULL, "name1", "new_description1_empty_store");  
  // setting in empty store
  goods_set_description(s, "name1", "new_description1_empty_store");  
  // setting incorrectly in empty store
  goods_set_description(s, "name1", "");  
  goods_set_description(s, "", "new_description");  
  // populating store
  store_insert_goods(s, "name1", "desc1", 0, false);
  store_insert_goods(s, "name2", "desc1", 0, false);
  // setting non-existing goods
  goods_set_description(s, "name3", "new_description3_non_existing");
  // setting incorrectly
  goods_set_description(s, "name1", "");  
  goods_set_description(s, "", "new_description");  
  // settings existing goods
  goods_set_description(s, "name1", "new_description1_existing");
  goods_set_description(s, "name2", "new_description2_existing");
  // settings existing goods again
  goods_set_description(s, "name1", "new_description1_existing_again");

  freeStore(s);
}

////
void test_store_search_goods()
{
  StoreT* s = createStore();

  // null arg
  CU_ASSERT_TRUE(store_search_goods(NULL, "name1") == 0);
  // searching empty story
  CU_ASSERT_TRUE(store_search_goods(s, "name1") == 0);
  CU_ASSERT_TRUE(store_search_goods(s, "") == 0);
  // inserting goods and searching for them
  store_insert_goods(s, "name1", "desc1", 0, false);
  CU_ASSERT_TRUE(store_search_goods(s, "name1") != 0);
  store_insert_goods(s, "name2", "desc1", 0, false);
  CU_ASSERT_TRUE(store_search_goods(s, "name2") != 0);
  store_insert_goods(s, "name3", "desc1", 0, false);
  CU_ASSERT_TRUE(store_search_goods(s, "name3") != 0);
  // searching for non-existing goods
  CU_ASSERT_TRUE(store_search_goods(s, "desc1") == 0);
  CU_ASSERT_TRUE(store_search_goods(s, "name4") == 0);
  CU_ASSERT_TRUE(store_search_goods(s, "") == 0);
  // removing goods and searching for them
  store_remove_goods(s, "name1", false);
  CU_ASSERT_TRUE(store_search_goods(s, "name1") == 0);
  CU_ASSERT_TRUE(store_search_goods(s, "name2") != 0);
  CU_ASSERT_TRUE(store_search_goods(s, "name3") != 0);
  store_remove_goods(s, "name2", false);
  CU_ASSERT_TRUE(store_search_goods(s, "name2") == 0);
  CU_ASSERT_TRUE(store_search_goods(s, "name3") != 0);
  store_remove_goods(s, "name3", false);
  CU_ASSERT_TRUE(store_search_goods(s, "name3") == 0);

  freeStore(s);
}

void test_store_has_goods ()
{
  StoreT* s = createStore();

  // null arg
  CU_ASSERT_TRUE(store_has_goods(NULL, "name1") == 0);
  // searching empty story
  CU_ASSERT_TRUE(store_has_goods(s, "name1") == 0);
  CU_ASSERT_TRUE(store_has_goods(s, "") == 0);
  // inserting goods and searching for them
  store_insert_goods(s, "name1", "desc1", 0, false);
  CU_ASSERT_TRUE(store_has_goods(s, "name1") != 0);
  store_insert_goods(s, "name2", "desc1", 0, false);
  CU_ASSERT_TRUE(store_has_goods(s, "name2") != 0);
  store_insert_goods(s, "name3", "desc1", 0, false);
  CU_ASSERT_TRUE(store_has_goods(s, "name3") != 0);
  // searching for non-existing goods
  CU_ASSERT_TRUE(store_has_goods(s, "desc1") == 0);
  CU_ASSERT_TRUE(store_has_goods(s, "name4") == 0);
  CU_ASSERT_TRUE(store_has_goods(s, "") == 0);
  // removing goods and searching for them
  store_remove_goods(s, "name1", false);
  CU_ASSERT_TRUE(store_has_goods(s, "name1") == 0);
  CU_ASSERT_TRUE(store_has_goods(s, "name2") != 0);
  CU_ASSERT_TRUE(store_has_goods(s, "name3") != 0);
  store_remove_goods(s, "name2", false);
  CU_ASSERT_TRUE(store_has_goods(s, "name2") == 0);
  CU_ASSERT_TRUE(store_has_goods(s, "name3") != 0);
  store_remove_goods(s, "name3", false);
  CU_ASSERT_TRUE(store_has_goods(s, "name3") == 0);

  freeStore(s);
}

void test_getName ()
{
  StoreT* s = createStore();
  char* name1 = "name1";
  char* name2 = "name2";
  char* name1new = "newname1";
  char* name2new = "newname2";
  // null arg
  CU_ASSERT_TRUE(getName(NULL) == NULL);
  // populate store, check name of goods
  store_insert_goods(s, name1, "desc1", 0, false);
  store_insert_goods(s, name2, "desc1", 0, false);
  GoodsT* g1 = store_search_goods(s, name1);
  GoodsT* g2 = store_search_goods(s, name2);
  CU_ASSERT_TRUE(strcmp(getName(g1), name1) == 0);
  CU_ASSERT_TRUE(strcmp(getName(g2), name2) == 0);
  // settings existing goods
  goods_set_name(s, name1, name1new);
  goods_set_name(s, name2, name2new);
  CU_ASSERT_TRUE(strcmp(getName(g1), name1new) == 0);
  CU_ASSERT_TRUE(strcmp(getName(g2), name2new) == 0);

  freeStore(s);
}

void test_getDescription()
{
  StoreT* s = createStore();

  char* name1 = "name1";
  char* name2 = "name2";
  char* desc1 = "desc1";
  char* desc2 = "desc2";
  char* desc1new = "desc1new";
  char* desc2new = "desc2new";
  // null arg
  CU_ASSERT_TRUE(getDescription(NULL) == NULL);
  // populate store, check description of goods
  store_insert_goods(s, name1, desc1, 0, false);
  store_insert_goods(s, name2, desc2, 0, false);
  GoodsT* g1 = store_search_goods(s, name1);
  GoodsT* g2 = store_search_goods(s, name2);
  CU_ASSERT_TRUE(strcmp(getDescription(g1), desc1) == 0);
  CU_ASSERT_TRUE(strcmp(getDescription(g2), desc2) == 0);
  // settings existing goods
  goods_set_description(s, name1, desc1new);
  goods_set_description(s, name2, desc2new);
  CU_ASSERT_TRUE(strcmp(getDescription(g1), desc1new) == 0);
  CU_ASSERT_TRUE(strcmp(getDescription(g2), desc2new) == 0);

  freeStore(s);
}

void test_getPrice()
{
  StoreT* s = createStore();

  char* name1 = "name1";
  char* name2 = "name2";
  char* desc1 = "desc1";
  char* desc2 = "desc2";
  int price1 = 1;
  int price2 = 2;
  int price1new = 10;
  int price2new = 20;
  // null arg
  CU_ASSERT_TRUE(getPrice(NULL) == 0);
  // populate store, check prices of goods
  store_insert_goods(s, name1, desc1, price1, false);
  store_insert_goods(s, name2, desc2, price2, false);
  GoodsT* g1 = store_search_goods(s, name1);
  GoodsT* g2 = store_search_goods(s, name2);
  CU_ASSERT_TRUE(getPrice(g1) == price1);
  CU_ASSERT_TRUE(getPrice(g2) == price2);
  // settings existing goods
  goods_set_price(s, name1, price1new);
  goods_set_price(s, name2, price2new);
  CU_ASSERT_TRUE(getPrice(g1) == price1new);
  CU_ASSERT_TRUE(getPrice(g2) == price2new);

  freeStore(s);
}

////
void test_validShelfName()
{
  // valid
  CU_ASSERT_TRUE(validShelfName("a0"));
  CU_ASSERT_TRUE(validShelfName("a0123456789"));
  CU_ASSERT_TRUE(validShelfName("z0"));
  // invalid
  CU_ASSERT_FALSE(validShelfName(0));
  CU_ASSERT_FALSE(validShelfName(""));
  CU_ASSERT_FALSE(validShelfName("0"));
  CU_ASSERT_FALSE(validShelfName("0a"));
  CU_ASSERT_FALSE(validShelfName("0a0"));
  CU_ASSERT_FALSE(validShelfName("01"));
  CU_ASSERT_FALSE(validShelfName("01a"));
  CU_ASSERT_FALSE(validShelfName("01a2"));
  CU_ASSERT_FALSE(validShelfName("a"));
  CU_ASSERT_FALSE(validShelfName("ab"));
  CU_ASSERT_FALSE(validShelfName("ab1"));
  CU_ASSERT_FALSE(validShelfName("a0a"));
  CU_ASSERT_FALSE(validShelfName("a01a"));
  CU_ASSERT_FALSE(validShelfName("a0a1"));
}

void test_goods_search_shelf()
{
  // todo
}

void test_shelf_get_name()
{
  //todo
}

void test_shelf_get_amount()
{
  //todo
}

void test_store_search_shelf()
{
  //todo
}

void test_store_has_shelf()
{
  //todo
}

////
void test_getCount()
{
  StoreT* s = createStore();
  char* name1 = "name1";
  char* name2 = "name2";
  
  // empty store
  CU_ASSERT_TRUE(getCount(s) == 0);
  // populate store, check counts
  store_insert_goods(s, name1, "desc1", 0, false);
  CU_ASSERT_TRUE(getCount(s) == 1);
  store_insert_goods(s, name2, "desc2", 0, false);
  CU_ASSERT_TRUE(getCount(s) == 2);
  // remove goods, check counts
  store_remove_goods(s, name1, false);
  CU_ASSERT_TRUE(getCount(s) == 1);
  store_remove_goods(s, name2, false);
  CU_ASSERT_TRUE(getCount(s) == 0);

  freeStore(s);
}

void test_validIndex()
{
  StoreT* s = createStore();
  char* name1 = "name1";
  char* name2 = "name2";
  
  // null arg
  CU_ASSERT_FALSE(validIndex(NULL,0));
  CU_ASSERT_FALSE(validIndex(NULL,-1));
  CU_ASSERT_FALSE(validIndex(NULL,1));
  // empty store
  CU_ASSERT_FALSE(validIndex(s,0));
  CU_ASSERT_FALSE(validIndex(s,-1));
  CU_ASSERT_FALSE(validIndex(s,1));
  // populate store, check indexes
  store_insert_goods(s, name1, "desc1", 0, false);
  CU_ASSERT_TRUE(validIndex(s,0));
  CU_ASSERT_FALSE(validIndex(s,1));
  store_insert_goods(s, name2, "desc2", 0, false);
  CU_ASSERT_TRUE(validIndex(s,0));
  CU_ASSERT_TRUE(validIndex(s,1));
  CU_ASSERT_FALSE(validIndex(s,2));
  // remove goods, check indexes
  store_remove_goods(s, name1, false);
  CU_ASSERT_TRUE(validIndex(s,0));
  CU_ASSERT_FALSE(validIndex(s,1));
  store_remove_goods(s, name2, false);
  CU_ASSERT_FALSE(validIndex(s,0));
  CU_ASSERT_FALSE(validIndex(s,1));

  freeStore(s);
}

void test_searchName()
{
  StoreT* s = createStore();

  char* name1 = "name1";
  char* name2 = "name2";
  char* name3 = "name3";
  
  // null arg
  CU_ASSERT_TRUE(searchName(NULL,name1) == -1);
  // empty store
  CU_ASSERT_TRUE(searchName(s,name1) == -1);
  // populate store, check names
  store_insert_goods(s, name1, "desc1", 0, false);
  CU_ASSERT_TRUE(searchName(s,name1) == 0);
  CU_ASSERT_TRUE(searchName(s,name2) == -1);
  CU_ASSERT_TRUE(searchName(s,name3) == -1);
  store_insert_goods(s, name3, "desc3", 0, false);
  CU_ASSERT_TRUE(searchName(s,name1) == 0);
  CU_ASSERT_TRUE(searchName(s,name2) == -1);
  CU_ASSERT_TRUE(searchName(s,name3) == 1);
  store_insert_goods(s, name2, "desc2", 0, false);
  CU_ASSERT_TRUE(searchName(s,name1) == 0);
  CU_ASSERT_TRUE(searchName(s,name2) == 1);
  CU_ASSERT_TRUE(searchName(s,name3) == 2);
  // remove goods, check names
  store_remove_goods(s, name1, false);
  CU_ASSERT_TRUE(searchName(s,name1) == -1);
  CU_ASSERT_TRUE(searchName(s,name2) == 0);
  CU_ASSERT_TRUE(searchName(s,name3) == 1);
  store_remove_goods(s, name3, false);
  CU_ASSERT_TRUE(searchName(s,name1) == -1);
  CU_ASSERT_TRUE(searchName(s,name2) == 0);
  CU_ASSERT_TRUE(searchName(s,name3) == -1);
  store_remove_goods(s, name2, false);
  CU_ASSERT_TRUE(searchName(s,name1) == -1);
  CU_ASSERT_TRUE(searchName(s,name2) == -1);
  CU_ASSERT_TRUE(searchName(s,name3) == -1);

  freeStore(s);
}

void test_getGoods()
{
  StoreT* s = createStore();

  char* name1 = "name1";
  char* name2 = "name2";
  
  // null arg
  CU_ASSERT_TRUE(getGoods(NULL,-1) == NULL);
  CU_ASSERT_TRUE(getGoods(NULL,0) == NULL);
  CU_ASSERT_TRUE(getGoods(NULL,1) == NULL);
  // empty store
  CU_ASSERT_TRUE(getGoods(s,-1) == NULL);
  CU_ASSERT_TRUE(getGoods(s,0) == NULL);
  CU_ASSERT_TRUE(getGoods(s,1) == NULL);
  // populate store, check goods
  store_insert_goods(s, name1, "desc1", 0, false);
  CU_ASSERT_TRUE(strcmp(getName(getGoods(s,0)), name1) == 0);
  CU_ASSERT_TRUE(getGoods(s,1) == NULL);
  store_insert_goods(s, name2, "desc2", 0, false);
  CU_ASSERT_TRUE(strcmp(getName(getGoods(s,0)), name1) == 0);
  CU_ASSERT_TRUE(strcmp(getName(getGoods(s,1)), name2) == 0);
  // remove goods, check goods
  store_remove_goods(s, name1, false);
  CU_ASSERT_TRUE(strcmp(getName(getGoods(s,0)), name2) == 0);
  CU_ASSERT_TRUE(getGoods(s,1) == NULL);
  store_remove_goods(s, name2, false);
  CU_ASSERT_TRUE(getGoods(s,0) == NULL);
  CU_ASSERT_TRUE(getGoods(s,1) == NULL);

  freeStore(s);
}

////
void test_goods_get_amount()
{
  StoreT* s = createStore();
  char* name1 = "name1";
  char* shelf1a = "A1";
  char* shelf1b = "A2";

  // populate store and get goods
  store_insert_goods(s, name1, "desc1", 0, false);
  GoodsT* g1 = store_search_goods(s, name1);

  // null arg
  CU_ASSERT_TRUE(goods_get_amount(NULL) == 0);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);

  // add to goods
  goods_add_to_shelf(s,    name1, shelf1a, 10);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 10);

  // add more to goods
  goods_add_to_shelf(s,    name1, shelf1a, 1);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 11);

  // add to second shelf, get amount from two shelves
  goods_add_to_shelf(s,    name1, shelf1b, 2);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 13);
  
  freeStore(s);  
}

void test_goods_get_shelves()
{
  StoreT* s = createStore();
  char* name1 = "name1";
  char* shelf1a = "A1";
  char* shelf1b = "A2";
  shelf_t** temp;
  char* temp_name;

  // populate store
  store_insert_goods(s, name1, "desc1", 0, false);
  GoodsT* g1 = store_search_goods(s, name1);

  // null arg
  CU_ASSERT_TRUE(goods_get_shelves(NULL) == NULL);

  // new goods
  CU_ASSERT_TRUE(goods_get_shelves(g1) == 0);

  // adding 50 to shelf1a
  goods_add_to_shelf(s, name1, shelf1a, 50);
  temp = goods_get_shelves(g1);
  
  temp_name = shelf_get_name(temp[0]);
  CU_ASSERT_TRUE(strcmp(temp_name, shelf1a) == 0);
  free(temp_name);

  CU_ASSERT_TRUE(temp[1] == NULL);
  free(temp);

  // adding 20 to (shelf1b)
  goods_add_to_shelf(s, name1, shelf1b, 20);
  temp = goods_get_shelves(g1);
  
  temp_name = shelf_get_name(temp[0]);
  CU_ASSERT_TRUE(strcmp(temp_name, shelf1a) == 0);
  free(temp_name);

  temp_name = shelf_get_name(temp[1]);
  CU_ASSERT_TRUE(strcmp(temp_name, shelf1b) == 0);
  free(temp_name);

  CU_ASSERT_TRUE(temp[2] == NULL);
  free(temp);

  freeStore(s);
}

void test_goods_add_to_shelf()
{
  StoreT* s = createStore();
  char* name1 = "name1";
  char* name2 = "name2";
  char* shelf1a = "A1";
  char* shelf1b = "A2";
  char* shelf2a = "B1";
  char* shelf2b = "B2";

  // populate store
  store_insert_goods(s, name1, "desc1", 0, false);
  store_insert_goods(s, name2, "desc2", 0, false);
  GoodsT* g1 = store_search_goods(s, name1);
  GoodsT* g2 = store_search_goods(s, name2);

  // null arg
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);
  goods_add_to_shelf(NULL, name1, shelf1a, 9);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);
  goods_add_to_shelf(s,    NULL,  shelf1a, 9);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);
  goods_add_to_shelf(s,    name1, NULL,    9);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);

  // 0 arg
  goods_add_to_shelf(s,    name1, shelf1a, 0);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 0);
  
  // add valid
  goods_add_to_shelf(s,    name1, shelf1a, 10);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 10);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 0);
  goods_add_to_shelf(s,    name2, shelf2a, 20);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 10);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 20);

  // add more to shelf
  goods_add_to_shelf(s,    name1, shelf1a, 1);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 11);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 20);

  goods_add_to_shelf(s,    name2, shelf2a, 1);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 11);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 21);

  // add to second shelf
  goods_add_to_shelf(s,    name1, shelf1b, 2);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 13);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 21);

  goods_add_to_shelf(s,    name2, shelf2b, 2);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 13);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 23);

  // add to shelf taken by different goods
  goods_add_to_shelf(s,    name1, shelf2a, 9);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 13);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 23);

  goods_add_to_shelf(s,    name2, shelf1a, 9);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 13);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 23);

  freeStore(s);
}

void test_goods_take()
{
  StoreT* s = createStore();
  char* name1 = "name1";
  char* name2 = "name2";
  char* shelf1a = "A1";
  char* shelf1b = "A2";
  char* shelf2a = "B1";
  char* shelf2b = "B2";
  shelf_t* temp;
  char* temp_name;
  
  // populate store
  store_insert_goods(s, name1, "desc1", 0, false);
  store_insert_goods(s, name2, "desc2", 0, false);
  GoodsT* g1 = store_search_goods(s, name1);
  GoodsT* g2 = store_search_goods(s, name2);
  
  // new goods
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);

  // null arg
  CU_ASSERT_TRUE(goods_take(NULL, 9) == NULL);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);

  // take invalid amounts
  CU_ASSERT_TRUE(goods_take(g1,0) == NULL);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);
  CU_ASSERT_TRUE(goods_take(g1,-1) == NULL);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);
  CU_ASSERT_TRUE(goods_take(g1,-20) == NULL);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);

  // adding: name1: 50  (shelf1a) + 20  (shelf1b)
  //         name2: 120 (shelf2a) + 150 (shelf2b)
  goods_add_to_shelf(s, name1, shelf1a, 50);
  goods_add_to_shelf(s, name1, shelf1b, 20);
  goods_add_to_shelf(s, name2, shelf2a, 120);
  goods_add_to_shelf(s, name2, shelf2b, 150);

  // take, expect: name1: 45  (shelf1a) + 20  (shelf1b)
  //               name2: 120 (shelf2a) + 150 (shelf2b)
  temp = goods_take(g1,5);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 65);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 270);

  temp_name = shelf_get_name(temp);
  CU_ASSERT_TRUE(strcmp(temp_name, shelf1a) == 0);
  free(temp_name);
  
  temp_name = shelf_get_name(temp+1);
  CU_ASSERT_TRUE(strcmp(temp_name,"") == 0);
  free(temp_name);


  free(temp);

  // take, expect: name1: 45  (shelf1a) + 20  (shelf1b)
  //               name2: 120 (shelf2a) + 145 (shelf2b)
  temp = goods_take(g2,5);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 65);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 265);

  temp_name = shelf_get_name(temp);
  CU_ASSERT_TRUE(strcmp(temp_name, shelf2b) == 0);
  free(temp_name);
  
  temp_name = shelf_get_name(temp+1);
  CU_ASSERT_TRUE(strcmp(temp_name,"") == 0);
  free(temp_name);

  
  free(temp);
  
  // take, expect: name1: 20  (shelf1a) + 20  (shelf1b)
  //               name2: 120 (shelf2a) + 145 (shelf2b)
  temp = goods_take(g1,25);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 40);

  temp_name = shelf_get_name(temp);
  CU_ASSERT_TRUE(strcmp(temp_name, shelf1a) == 0);
  free(temp_name);
  
  temp_name = shelf_get_name(temp+1);
  CU_ASSERT_TRUE(strcmp(temp_name,"") == 0);
  free(temp_name);

  free(temp);
  
  // take, expect: name1: 0   (shelf1a) + 15  (shelf1b)
  //               name2: 120 (shelf2a) + 145 (shelf2b)
  temp = goods_take(g1,25);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 15);
  
  temp_name = shelf_get_name(temp);
  CU_ASSERT_TRUE(strcmp(temp_name,shelf1a) == 0);
  free(temp_name);

  temp_name = shelf_get_name(temp+1);
  CU_ASSERT_TRUE(strcmp(temp_name,shelf1b) == 0);
  free(temp_name);

  temp_name = shelf_get_name(temp+2);
  CU_ASSERT_TRUE(strcmp(temp_name,"") == 0);
  free(temp_name);

  CU_ASSERT_TRUE(goods_search_shelf(g1,shelf1a) == 0);
  CU_ASSERT_TRUE(goods_search_shelf(g1,shelf1b) != 0);
  free(temp);

  // taking more than available
  // expect: name1: 0   (shelf1a) + 15  (shelf1b)
  //         name2: 120 (shelf2a) + 145 (shelf2b)
  temp = goods_take(g1,25);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 15);
  CU_ASSERT_TRUE(temp == 0);
  free(temp);

  // expect: name1: 0  (shelf1b)
  //         name2: 120 (shelf2a) + 145 (shelf2b)
  temp = goods_take(g1,15);
  CU_ASSERT_TRUE(goods_get_amount(g1) == 0);
  
  temp_name = shelf_get_name(temp);
  CU_ASSERT_TRUE(strcmp(temp_name,shelf1b) == 0);
  free(temp_name);

  temp_name = shelf_get_name(temp+1);
  CU_ASSERT_TRUE(strcmp(temp_name,"") == 0);
  free(temp_name);

  CU_ASSERT_TRUE(goods_search_shelf(g1,shelf1a) == 0);
  CU_ASSERT_TRUE(goods_search_shelf(g1,shelf1b) == 0);
  free(temp);

  CU_ASSERT_TRUE(goods_search_shelf(g2,shelf2a) != 0);
  CU_ASSERT_TRUE(goods_search_shelf(g2,shelf2b) != 0);
  // expect: name2: 0 (shelf2a) + 0 (shelf2b)  temp = goods_take(g1,15);
  temp = goods_take(g2,145+120);
  CU_ASSERT_TRUE(goods_get_amount(g2) == 0);
  
  temp_name = shelf_get_name(temp);
  CU_ASSERT_TRUE(strcmp(temp_name,shelf2b) == 0);
  free(temp_name);
  
  temp_name = shelf_get_name(temp+1);
  CU_ASSERT_TRUE(strcmp(temp_name,shelf2a) == 0);
  free(temp_name);

  temp_name = shelf_get_name(temp+2);
  CU_ASSERT_TRUE(strcmp(temp_name,"") == 0);
  free(temp_name);

  CU_ASSERT_TRUE(goods_search_shelf(g2,shelf2a) == 0);
  CU_ASSERT_TRUE(goods_search_shelf(g2,shelf2b) == 0);

  free(temp);

  freeStore(s);
}

////
void test_getGoodsCpyPtr()
{
  // todo
}

void test_replaceGoods()
{
  //todo
}

////
void test_undoLastAction()
{
  StoreT* s = createStore();

  // todo

  freeStore(s);
}

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite goods_valid_params = CU_add_suite("Goods validation functions", NULL, NULL);
  CU_add_test(goods_valid_params, "Valid name", test_validName);  
  CU_add_test(goods_valid_params, "Valid price", test_validPrice);  
  CU_add_test(goods_valid_params, "Valid amount", test_validAmount);  

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite creation = CU_add_suite("Test creation and empty", NULL, NULL);
  CU_add_test(creation, "Store creation", test_createStore_freeStore);
  CU_add_test(creation, "Goods creation", test_createGoods_freeGoods);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite insert_remove = CU_add_suite("Insertion and removal", NULL, NULL);
  CU_add_test(insert_remove, "Insert goods", test_store_insert_goods);
  CU_add_test(insert_remove, "Remove goods", test_store_remove_goods);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite goods_set = CU_add_suite("Goods setters", NULL, NULL);
  CU_add_test(goods_set, "Set description", test_goods_set_description);
  CU_add_test(goods_set, "Set name", test_goods_set_name);
  CU_add_test(goods_set, "Set price", test_goods_set_price);
  
  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite goods_search_get = CU_add_suite("Searching for goods and goods getters", NULL, NULL);
  CU_add_test(goods_search_get, "Search store for goods", test_store_search_goods);
  CU_add_test(goods_search_get, "Check if store contains goods", test_store_has_goods);
  CU_add_test(goods_search_get, "Get goods name", test_getName);
  CU_add_test(goods_search_get, "Get goods description", test_getDescription);
  CU_add_test(goods_search_get, "Get goods price", test_getPrice);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite goods_by_index = CU_add_suite("Searching for and getting goods by index", NULL, NULL);
  CU_add_test(goods_by_index, "Get store count", test_getCount);
  CU_add_test(goods_by_index, "Check if index is valid", test_validIndex);
  CU_add_test(goods_by_index, "Search name and get index", test_searchName);
  CU_add_test(goods_by_index, "Get goods by index", test_getGoods);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite shelf1 = CU_add_suite("Shelf related tests 1", NULL, NULL);
  CU_add_test(shelf1, "Validate shelf name", test_validShelfName);
  CU_add_test(shelf1, "Search for shelf of goods", test_goods_search_shelf);
  CU_add_test(shelf1, "Get shelf name", test_shelf_get_name);
  CU_add_test(shelf1, "Get shelf amount", test_shelf_get_amount);
  CU_add_test(shelf1, "Search for shelf in store", test_store_search_shelf);
  CU_add_test(shelf1, "Check if store has shelf", test_store_has_shelf);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite shelf2 = CU_add_suite("Shelf related tests 2", NULL, NULL);
  CU_add_test(shelf2, "Get amount from goods", test_goods_get_amount);
  CU_add_test(shelf2, "Get goods' shelves", test_goods_get_shelves);
  CU_add_test(shelf2, "Add goods to shelf", test_goods_add_to_shelf);
  CU_add_test(shelf2, "Take an amount from goods", test_goods_take);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite copy_replace = CU_add_suite("Copy and replace goods", NULL, NULL);
  CU_add_test(copy_replace, "Copy goods", test_getGoodsCpyPtr);
  CU_add_test(copy_replace, "Replace goods", test_replaceGoods);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite undo = CU_add_suite("Undo actions", NULL, NULL);
  CU_add_test(undo, "Undo actions", test_undoLastAction);

  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();

  // Tear down
  //////////////////////////////////////////////////////////////////////////////
  CU_cleanup_registry();

  return 0;
}
