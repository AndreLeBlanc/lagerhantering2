#define DEF5INT                    \
    int n1 = 0;                    \
    int n2 = -1;                   \
    int n3 = 1;                    \
    int n4 = 5;                    \
    int n5 = 7;

#define ADD5INTSET                 \
    insert_element(s,&n1);         \
    insert_element(s,&n2);         \
    insert_element(s,&n3);         \
    insert_element(s,&n4);         \
    insert_element(s,&n5);
                      

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bst.h"

void* keygen (void* data)
{
  int* key = calloc(1,sizeof(int));
  *key = *(int*)data;
  return key;
}

int keytoi (void* k)
{
  return *((int*)k);
}

int keycomp (void* key1, void* key2)
{
  int k1 = keytoi(key1);
  int k2 = keytoi(key2);
  if(k1>k2) { return 1;  }
  if(k1<k2) { return -1; }
  return 0;
}

void keyfreer (void* key)
{
  assert(key);
  free(key);
}

void* datacopier (void* data)
{
  assert(data);
  int* copy = calloc(1,sizeof(int));
  *copy = *(int*)data;
  return copy;
}

void datafreer (void* data)
{
  assert(data);
  free(data);
}

void dataprinter(void* data)
{
  printf("Name: %d\n", *((int*)data));
}

set_t* createbst() {
  return new_set(keygen, keycomp, keyfreer, datacopier, datafreer);
}

/** A helper to get the key that a data generates
 * \param data The data
 * \returns Int representing the key
 */
int datatointkey (void* data)
{
  void* key = keygen(data);
  int key_int = keytoi(key);
  keyfreer(key);
  return key_int;
}

void test_bst_new_set()
{
  set_t* s = createbst();
  CU_ASSERT_FALSE(s == NULL);

  free_set(s);
}

void test_bst_copy_set()
{
  set_t* s = createbst();

  // todo

  free_set(s);
}


void test_bst_empty()
{
  set_t* s = createbst();
  CU_ASSERT_TRUE(set_is_empty(s));

  free_set(s);
}

void test_bst_insert()
{
  set_t* s = createbst();
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;

  // check if empty
  CU_ASSERT_TRUE(set_is_empty(s));
  // insert 1 and check if not empty
  insert_element(s,&n1);
  CU_ASSERT_FALSE(set_is_empty(s));
  // test more insertions
  insert_element(s,&n2);
  insert_element(s,&n4);
  insert_element(s,&n3);
  insert_element(s,&n5);

  free_set(s);
}

void test_bst_searchdata()
{
  set_t* s = createbst();
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;
  int bad1 = 9;
  int bad2 = -9;

  // fill set with test data
  insert_element(s,&n1);
  insert_element(s,&n2);
  insert_element(s,&n3);
  insert_element(s,&n4);
  insert_element(s,&n5);
  // check if inserted data exists
  CU_ASSERT_TRUE(datatointkey(searchdata (s, &n1)) == n1);
  CU_ASSERT_TRUE(datatointkey(searchdata (s, &n2)) == n2);
  CU_ASSERT_TRUE(datatointkey(searchdata (s, &n3)) == n3);
  CU_ASSERT_TRUE(datatointkey(searchdata (s, &n4)) == n4);
  CU_ASSERT_TRUE(datatointkey(searchdata (s, &n5)) == n5);
  // check that bad data doesn't exist
  CU_ASSERT_PTR_NULL(searchdata(s, &bad1));
  CU_ASSERT_PTR_NULL(searchdata(s, &bad2));

  free_set(s);
}

void test_bst_delete()
{
  set_t* s = createbst();
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;

  // test deleting non-existent data
  delete_element(s,&n1);
  // fill set with test data
  insert_element(s,&n1);
  insert_element(s,&n2);
  insert_element(s,&n3);
  insert_element(s,&n4);
  insert_element(s,&n5);
  // delete all known data
  delete_element(s,&n1);
  delete_element(s,&n2);
  delete_element(s,&n3);
  delete_element(s,&n4);
  delete_element(s,&n5);
  // check if empty after delete
  CU_ASSERT_TRUE(set_is_empty(s));
  // insert one element
  insert_element(s,&n2);
  // check if not empty after insert
  CU_ASSERT_FALSE(set_is_empty(s));
  // do some more inserts and deletes
  insert_element(s,&n1);
  insert_element(s,&n4);
  insert_element(s,&n3);
  insert_element(s,&n5);
  delete_element(s,&n2);
  delete_element(s,&n1);
  delete_element(s,&n4);
  delete_element(s,&n3);
  delete_element(s,&n5);
  // check if empty
  CU_ASSERT_TRUE(set_is_empty(s));

  free_set(s);
}

void test_bst_delete_search()
{
  set_t* s = createbst();
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;

  // check if search for non-existent elem returns negative
  CU_ASSERT_PTR_NULL(searchdata (s, &n1));
  // delete non-existent and check if that changes the above search
  delete_element(s,&n1);
  CU_ASSERT_PTR_NULL(searchdata (s, &n1));
  // insert and check if element is found
  insert_element(s,&n1);
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n1));
  // delete after insert and check if search returns negative
  delete_element(s,&n1);
  CU_ASSERT_PTR_NULL(searchdata (s, &n1));
  // insert same element after deletion and verify that it exists
  insert_element(s,&n1);
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n1));
  // fill with data
  insert_element(s,&n2);
  insert_element(s,&n3);
  insert_element(s,&n4);
  insert_element(s,&n5);
  //delete one and check if all is as expected
  delete_element(s,&n2);
  CU_ASSERT_PTR_NULL(searchdata (s, &n2));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n1));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n3));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n4));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n5));
  // delete bad and check if anything changes
  delete_element(s,&n2);
  CU_ASSERT_PTR_NULL(searchdata (s, &n2));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n1));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n3));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n4));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n5));
  //delete one and check if all is as expected
  delete_element(s,&n5);
  CU_ASSERT_PTR_NULL(searchdata (s, &n5));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n1));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n3));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n4));
  //delete one and check if all is as expected
  delete_element(s,&n1);
  CU_ASSERT_PTR_NULL(searchdata (s, &n1));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n3));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n4));
  //delete one and check if all is as expected
  delete_element(s,&n3);
  CU_ASSERT_PTR_NULL(searchdata (s, &n3));
  CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n4));
  //delete one and check if all is as expected
  delete_element(s,&n4);
  CU_ASSERT_PTR_NULL(searchdata (s, &n4));
  CU_ASSERT_TRUE(set_is_empty(s));

  free_set(s);
}

void test_bst_delete_getcount ()
{
  set_t* s = createbst();
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;

  // check if count of empty set 0
  CU_ASSERT_EQUAL(set_getcount(s), 0);
  // fill set data and check if count as expected
  insert_element(s,&n1);
  CU_ASSERT_EQUAL(set_getcount(s), 1);
  insert_element(s,&n2);
  CU_ASSERT_EQUAL(set_getcount(s), 2);
  insert_element(s,&n3);
  CU_ASSERT_EQUAL(set_getcount(s), 3);
  // delete data and check if count as expected
  delete_element(s,&n3);
  CU_ASSERT_EQUAL(set_getcount(s), 2);
  delete_element(s,&n2);
  CU_ASSERT_EQUAL(set_getcount(s), 1);
  delete_element(s,&n1);
  CU_ASSERT_EQUAL(set_getcount(s), 0);
  // delete bad and check if count as expected
  delete_element(s,&n1);
  CU_ASSERT_EQUAL(set_getcount(s), 0);

  free_set(s);
}

void test_bst_set_foreachdata_fun(void* a)
{
  *(int*)a += 1;
}

void test_bst_set_foreachdata_get_element_data()
{
  set_t* s = createbst();
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;
  int m1 = n1;
  int m2 = n2;
  int m3 = n3;
  int m4 = n4;
  int m5 = n5;
  // run function on "after" data
  test_bst_set_foreachdata_fun(&m1);
  test_bst_set_foreachdata_fun(&m2);
  test_bst_set_foreachdata_fun(&m3);
  test_bst_set_foreachdata_fun(&m4);
  test_bst_set_foreachdata_fun(&m5);
  // fill set with "before" data
  insert_element(s,&n1);
  insert_element(s,&n2);
  insert_element(s,&n3);
  insert_element(s,&n4);
  insert_element(s,&n5);
  // run function on all elements of set
  set_foreachdata(s, &test_bst_set_foreachdata_fun);
  
  // check that all elements have the expected values of the "after" data
  CU_ASSERT_EQUAL(*((int*)searchdata(s, &n1)), m1);
  CU_ASSERT_EQUAL(*((int*)searchdata(s, &n2)), m2);
  CU_ASSERT_EQUAL(*((int*)searchdata(s, &n3)), m3);
  CU_ASSERT_EQUAL(*((int*)searchdata(s, &n4)), m4);
  CU_ASSERT_EQUAL(*((int*)searchdata(s, &n5)), m5);

  free_set(s);
}

void test_bst_iter_new ()
{
  set_t* s   = createbst();
  iter_t* it = iter_new(s);
  DEF5INT
  ADD5INTSET
  iter_free(it);
  free_set(s);

  s  = createbst();
  it = iter_new(s);
  ADD5INTSET
  free_set(s);
  iter_free(it);
}

void test_bst_iter_nsteps_start_end ()
{
  set_t* s   = createbst();
  iter_t* it = iter_new(s);
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;
  
  insert_element(s, &n1);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  iter_nsteps(it, 0);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  iter_nsteps(it, 1);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  // invalid, stepping backwards
  iter_nsteps(it, -1);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  insert_element(s, &n2);
  insert_element(s, &n3);
  insert_element(s, &n4);
  insert_element(s, &n5);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_nsteps(it, 0);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  // invalid, stepping backwards
  iter_nsteps(it, -1);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_nsteps(it, 1);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_nsteps(it, 2);
  CU_ASSERT_FALSE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_nsteps(it, 1);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));
  iter_nsteps(it, 1);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));
  // invalid, stepping backwards
  iter_nsteps(it, -4);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));
  iter_nsteps(it, 4);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));

  iter_free(it);
  free_set(s); 
}

void test_bst_iter_toindex_start_end ()
{
  set_t* s   = createbst();
  iter_t* it = iter_new(s);
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;
  
  insert_element(s, &n1);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  iter_toindex(it, 0);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  iter_toindex(it, 1);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  iter_toindex(it, -1);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  insert_element(s, &n2);
  insert_element(s, &n3);
  insert_element(s, &n4);
  insert_element(s, &n5);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_toindex(it, 0);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_toindex(it, -1);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_toindex(it, 1);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_toindex(it, 3);
  CU_ASSERT_FALSE (iter_start(it));
  CU_ASSERT_FALSE (iter_end  (it));
  iter_toindex(it, 4);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));
  iter_toindex(it, 0);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_toindex(it, 4);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));
  iter_toindex(it, -8);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_toindex(it, 8);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));

  iter_free(it);
  free_set(s); 
}

void test_bst_iter_reset_nsteps ()
{
  set_t* s   = createbst();
  iter_t* it = iter_new(s);
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;
  
  insert_element(s, &n1);
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  iter_reset(it);  
  CU_ASSERT_TRUE(iter_start(it));
  CU_ASSERT_TRUE(iter_end  (it));
  insert_element(s, &n2);
  insert_element(s, &n3);
  insert_element(s, &n4);
  insert_element(s, &n5);
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_nsteps(it, 4);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_TRUE (iter_end  (it));
  iter_reset(it);  
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_nsteps(it, 1);
  CU_ASSERT_FALSE(iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));
  iter_reset(it);  
  CU_ASSERT_TRUE (iter_start(it));
  CU_ASSERT_FALSE(iter_end  (it));

  iter_free(it);
  free_set(s); 
}

void test_bst_iter_index ()
{
  set_t* s   = createbst();
  iter_t* it = iter_new(s);
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;

  insert_element(s, &n1);
  CU_ASSERT_EQUAL(iter_index(it),0);
  insert_element(s, &n2);
  CU_ASSERT_EQUAL(iter_index(it),0);
  iter_nsteps(it, 1);
  CU_ASSERT_EQUAL(iter_index(it),1);
  insert_element(s, &n3);
  insert_element(s, &n4);
  insert_element(s, &n5);
  CU_ASSERT_EQUAL(iter_index(it),0);
  iter_nsteps(it, 4);
  CU_ASSERT_EQUAL(iter_index(it),4);
  delete_element(s,&n2);
  CU_ASSERT_EQUAL(iter_index(it),0);
  iter_nsteps(it, 8);
  CU_ASSERT_EQUAL(iter_index(it),3);
  // invalid, stepping backwards
  iter_nsteps(it, -8);
  CU_ASSERT_EQUAL(iter_index(it),3);
  iter_reset(it);
  CU_ASSERT_EQUAL(iter_index(it),0);

  iter_free(it);
  free_set(s); 
}

void test_bst_iter_get ()
{
  set_t* s   = createbst();
  iter_t* it = iter_new(s);
  int n1 = 0;
  int n2 = -1;
  int n3 = 1;
  int n4 = 5;
  int n5 = 7;

  insert_element(s, &n1);
  insert_element(s, &n2);
  insert_element(s, &n3);
  insert_element(s, &n4);
  insert_element(s, &n5);
                  
  CU_ASSERT_EQUAL(*(int*)iter_getdata(it), -1);
  iter_nsteps(it, 4);
  CU_ASSERT_EQUAL(*(int*)iter_getdata(it), 7);
  delete_element(s,&n5);
  CU_ASSERT_EQUAL(*(int*)iter_getdata(it), -1);
  iter_nsteps(it, 2);
  CU_ASSERT_EQUAL(*(int*)iter_getdata(it), 1);
  // invalid, stepping backwards
  iter_nsteps(it, -1);
  CU_ASSERT_EQUAL(*(int*)iter_getdata(it), 1);
  iter_reset(it);
  CU_ASSERT_EQUAL(*(int*)iter_getdata(it), -1);

  iter_free(it);
  free_set(s); 
}

void test_bst_insert_delete_searchdata_iter_many_fun_add (void* a) { *(int*)a += 1; }
void test_bst_insert_delete_searchdata_iter_many_fun_sub (void* a) { *(int*)a -= 1; }

void test_bst_insert_delete_searchdata_iter_many ()
{
  set_t* s   = createbst();
  iter_t* it = iter_new(s);
  int amount = 1000;
  int n = 0;
  int m = 0;
  int i = 0;
  // insert all elements
  for(i = 0; i < amount; i++) {
    if(i%2) { n = (-i); }
    else    { n = i;    }
    insert_element(s, &n);
  }

  // search check if exist
  for(i = 0; i < amount; i++) {
    if(i%2) { n = (-i); }
    else    { n = i;    }
    searchdata (s, &n);
    CU_ASSERT_PTR_NOT_NULL(searchdata (s, &n));
    CU_ASSERT_EQUAL(*((int*)searchdata(s, &n)), n);
  }
  
  // add 1 to each elem
  set_foreachdata(s, test_bst_insert_delete_searchdata_iter_many_fun_add);
  for(i = 0; i < amount; i++) {
    if(i%2) { n = (-i); }
    else    { n = i;    }
    m = n;
    test_bst_insert_delete_searchdata_iter_many_fun_add(&m);
    CU_ASSERT_EQUAL(*((int*)searchdata(s, &n)), m);
  }

  // sub 1 to each elem
  set_foreachdata(s, test_bst_insert_delete_searchdata_iter_many_fun_sub);
  for(i = 0; i < amount; i++) {
    if(i%2) { n = (-i); }
    else    { n = i;    }
    test_bst_insert_delete_searchdata_iter_many_fun_sub(&m);
    CU_ASSERT_EQUAL(*((int*)searchdata(s, &n)), n);
  }

  // del neg keys
  iter_reset(it);
  int new_amount = amount;
  for(i = 0; i < amount; i++) {
    if(i%2) { n = (-i); }
    else    { n = i;    }
    if(n < 0) {
      delete_element(s,&n);
      new_amount--;
      CU_ASSERT_PTR_NULL(searchdata (s, &n));
    } else {
      delete_element(s,&n);
      insert_element(s,&n);
      CU_ASSERT_EQUAL(*((int*)searchdata(s, &n)), n); 
    }
  }
  
  iter_reset(it);
  iter_nsteps(it, amount);
  CU_ASSERT_EQUAL(iter_index(it), new_amount-1); 

  iter_free(it);
  free_set(s); 
}

int main(int argc, char *argv[])
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite creation = CU_add_suite("Test creation and empty", NULL, NULL);
  CU_add_test(creation, "Creation", test_bst_new_set);
  CU_add_test(creation, "Empty", test_bst_empty);
  
  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite insdelsearch = CU_add_suite("Test insert, delete and search", NULL, NULL);
  CU_add_test(insdelsearch, "Insert", test_bst_insert);
  CU_add_test(insdelsearch, "Search", test_bst_searchdata);
  CU_add_test(insdelsearch, "Delete", test_bst_delete);
  CU_add_test(insdelsearch, "Delete and search", test_bst_delete_search);
  CU_add_test(insdelsearch, "Delete and get count", test_bst_delete_getcount);
  CU_add_test(insdelsearch, "For each data and get element", test_bst_set_foreachdata_get_element_data);

  //////////////////////////////////////////////////////////////////////////////
  CU_pSuite iterator = CU_add_suite("Test iterator", NULL, NULL);
  CU_add_test(iterator, "Creation", test_bst_iter_new);
  CU_add_test(iterator, "Step and boundary check.", test_bst_iter_nsteps_start_end);
  CU_add_test(iterator, "Step by 'index' and boundary check.", test_bst_iter_toindex_start_end);
  CU_add_test(iterator, "Reset and step.", test_bst_iter_reset_nsteps);
  CU_add_test(iterator, "Index.", test_bst_iter_index);
  CU_add_test(iterator, "Get.", test_bst_iter_get);

  //////////////////////////////////////////////////////////////////////////////
  // integration tests
  CU_pSuite largeset = CU_add_suite("Test manipulating larger sets", NULL, NULL);
  CU_add_test(largeset, "Insert, delete, search data, iterate", test_bst_insert_delete_searchdata_iter_many);

  // Actually run tests
  //////////////////////////////////////////////////////////////////////////////
  CU_basic_run_tests();

  // Tear down
  //////////////////////////////////////////////////////////////////////////////
  CU_cleanup_registry();

  return 0;
}
