//#define DEBUG

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "alloc_check.h"
#include "bst.h"

struct element_t {
  void* data;
  void* key;
  struct element_t* left;
  struct element_t* right;
  struct element_t* next;
};

struct set_t {
  struct element_t* root;
  struct element_t* first;
  key_generator_t*  key_generator;
  key_comparator_t* key_comparator;
  key_freer_t*      key_freer;
  data_copier_t*    data_copier;
  data_freer_t*     data_freer;
  int               count;
  bool              needs_reindexing;
};

struct iter_t {
  struct set_t* set;
  struct element_t* current;
  int index;
};

/***************/
/*   PRIVATE   */
/***************/
int compare_keys (set_t* set, void* key1, void* key2) { return set->key_comparator(key1, key2); }

// validators
bool set_is_empty (set_t* s)
{
  return s->root == NULL;
}

bool element_is_empty (element_t* e)
{
  return e == NULL;
}

// getters
key_comparator_t* get_key_comparator  (set_t* s)     { return s->key_comparator;}
key_generator_t*  get_key_generator   (set_t* s)     { return s->key_generator; }
data_copier_t*    get_data_copier     (set_t* s)     { return s->data_copier;   }
void*             get_element_key     (element_t* e) { return e->key;       }

/** Gets element data.
 * Gets data of an element.
 * \params elem The element to retrieve the data from.
 * \returns Null pointer if element empty, otherwise a pointer to element's data.
 */
void* get_element_data (element_t* e)
{
  if(element_is_empty(e)) {
    return NULL;
  }
  return e->data;
}

/**
 * Returns first element of a set.
 * \returns First element of set or null ptr if there is none.
 */
element_t** get_first_element (set_t* set)
{
  if(element_is_empty(set->root)) {
    return &set->first;
  } 
  //return &set->first;

  element_t** current = &set->root;
  element_t** boundary_elem = current;
  while(!element_is_empty(*current)) {
    boundary_elem = current;
    current = (&(*current)->left);
  }
  return boundary_elem;
}

element_t** search_aux (set_t* set, void* target_key, bool bool_get_parent)
{
  if(!target_key) {
    return NULL;
  }
  element_t** current = &set->root;
  element_t** parent  = current;
  int compare_result  = 1;

  while(!element_is_empty(*current) && compare_result != 0) {
    parent = current;
    compare_result = compare_keys(set, target_key, get_element_key(*current));
  
    // result < 0 = less than, result > 0 = greater than, result == 0 = equal
    if(compare_result < 0) {
      current = &(*current)->left;
    } else if(compare_result > 0) {
      current = &(*current)->right;
    }
  }

  if(bool_get_parent) {
    return parent;
  } else {
    return current;
  }
}

// getters
/** Searches for an element.
 * Search for and access element with a key.
 * \params set The set to search in.
 * \params target_key Key of sought element.
 * \returns A ptr to an element ptr if one was matched,
 * otherwise ptr to empty element where the searched data belongs in sorted order.
 * If target_key == null null is returned
 */
element_t** search(set_t* set, void* target_key)
{
  return search_aux(set, target_key, false);
}

element_t** search_parent (set_t* set, void* target_key)
{
  return search_aux(set, target_key, true);
}

element_t* get_predecessor(set_t* set, element_t* element) // todo merge get_pre/succ
{
  if(!set || !element) {
    return NULL;
  }
  void* target_key = get_element_key(element);
  element_t** current  = &set->root;
  void* current_key = get_element_key(*current);
  element_t* least_lower_bound = NULL;
  int comparison_result = set->key_comparator(current_key, target_key);

  while (!element_is_empty(*current)) {
    current_key       = get_element_key(*current);
    comparison_result = set->key_comparator(current_key, target_key);
    if(comparison_result >= 0) {
      current = &(*current)->left;
    } else {
      least_lower_bound = *current;
      current = &(*current)->right;
    }
  }
  return least_lower_bound;
}

element_t* get_successor(set_t* set, element_t* element)
{
  if(!set || !element || set->count < 1) {
    return NULL;
  }

  element_t** current  = &set->root;
  void* target_key = get_element_key(element);
  void* current_key;
  element_t* least_upper_bound = NULL;
  int comparison_result;

  while (!element_is_empty(*current)) {
    current_key = get_element_key(*current);
    comparison_result = set->key_comparator(current_key, target_key);
    if(comparison_result <= 0) {
      current = &(*current)->right;
    } else {
      least_upper_bound = *current;
      current = &(*current)->left;
    }
  }
  return least_upper_bound;
}

// constructors

// Achievement O44: Optimized insert_element (doesn't copy data)
// optimized version
element_t* new_element(set_t* set, void* data)
{
  element_t* element = calloc (1, sizeof(element_t));
  check_allocation(element);
  element->data = data;
  element->key  = set->key_generator(element->data);
  return element;
}
// original version
/* element_t* new_element(set_t* set, void* data) */
/* { */
/*   element_t* element = calloc (1, sizeof(element_t)); */
/*   check_allocation(element); */
/*   element->data = set->data_copier(data); */
/*   element->key  = set->key_generator(element->data); */
/*   return element; */
/* } */

// destructors
void free_key (set_t* set, element_t* element)
{
  void* key = get_element_key(element);
  assert(key && "free key failed!" ); // todo what if key == 0?
  set->key_freer(key);
}

void free_element_data (set_t* set, element_t* element)
{
  void* data = get_element_data(element);
  assert(data && "free element data failed!");
  set->data_freer(data);
}

void free_element (set_t* set, element_t* element)
{
  free_key(set, element);
  free_element_data(set, element);
  assert(element && "free element failed!");
  free(element);
}

// mappers
void map_on_data (set_t* set, void* data, data_map_fun_t function)
{
  if(data) {
    function(data);
  }
}

void set_foreachelement_aux(set_t* set, set_t* recipient, element_t* parent, void function (set_t*, element_t*))
{
  if(!element_is_empty(parent)) {
    element_t* left = parent->left;
    element_t* right = parent->right;
    set_foreachelement_aux(set, recipient, left, function);
    function(recipient, parent);
    set_foreachelement_aux(set, recipient, right, function);
  }
}

// first set is the one to find elements in, second set is the one to do the function on
void set_foreachelement_to_new(set_t* set, set_t* new, void function (set_t*, element_t*))
{
  if (!set_is_empty(set)) {
    set_foreachelement_aux(set, new, set->root, function); 
  }
}

// todo merge with set_foreachelement_aux and change all calls to that function
void set_foreachdata_aux(set_t* set, element_t* parent, void function (void*))
{
  if(!element_is_empty(parent)) {
    set_foreachdata_aux(set, parent->left, function);      
    map_on_data(set, get_element_data(parent), function);
    set_foreachdata_aux(set, parent->right, function);
  }
}

/**
 * Advance the iterator by one step.
 * \warning Must only be called if \ref iter_end returns `false`.
 * \sa iter_end
 */
void iter_next(iter_t* iter)
{
  if(!iter_end(iter) && iter->current) {
    iter->current = get_successor(iter->set, iter->current);
    //iter->current = iter->current->next;
    iter->index++;
  }
}

/** Access the current element.
 * Access the current element of an iterator.
 * \param The iterator.
 * \returns The current element.
 * \invariant !set_is_empty(iter_t->set)
 * \sa set_is_empty
 */
void* iter_get(iter_t* iter)
{
  if(element_is_empty(iter->current) || iter->set->needs_reindexing) {
    iter_reset(iter);
  }
  return iter->current;
}

/**************/
/*   PUBLIC   */
/**************/

// constructors
set_t* new_set (key_generator_t* key_generator, key_comparator_t* key_comparator, key_freer_t* key_freer, data_copier_t* data_copier, data_freer_t* data_freer)
{
  set_t* set            = calloc(1, sizeof(set_t));
  check_allocation(set);
  set->key_generator    = key_generator;
  set->key_comparator   = key_comparator;
  set->data_copier      = data_copier;
  set->key_freer        = key_freer;
  set->data_freer       = data_freer;
  return set;
}

void copy_set_aux(set_t* set, element_t* element)
{
  insert_element(set, get_element_data(element));
}

set_t* copy_set (set_t* set)
{
  if(!set) {
    return NULL;
  }
  set_t* copy = new_set(set->key_generator, set->key_comparator, set->key_freer, set->data_copier, set->data_freer);
  set_foreachelement_to_new(set, copy, copy_set_aux);
  return copy;
}

// destructors
void free_set (set_t* set)
{
  assert(set && "free set failed!");
  set_foreachelement_to_new(set, set, free_element);
  free(set);
}

void* searchdata(set_t* set, void* target_key)
{
  return get_element_data(*search(set, target_key));
}

int set_getcount (set_t* set)
{
  return set->count;
}


// mappers
void set_foreachdata(set_t* set, data_map_fun_t function) // todo non-recursive
{
  set_foreachdata_aux(set, set->root, function);
}

// other
void insert_element(set_t* set, void* data)
{
  element_t* new = new_element(set, data);
  if (set->count < 1) {
    set->root = new;
    set->first = new;
    set->count = 1;
    return;
  }

  void* key = get_element_key(new);
  element_t** parent = search_parent(set, key);
  element_t** insert_location;
  
  if(set->key_comparator(key, get_element_key(*parent)) > 0) {
    insert_location = &(*parent)->right;
  } else {
    insert_location = &(*parent)->left;
  }
  
  if(element_is_empty(*insert_location)) {
    *insert_location = new;
    set->needs_reindexing = true;
    set->count++;

    /* if(set->key_comparator(key, get_element_key(set->first)) < 0) { */
    /*   set->first = new; */
    /* } */ // todo use something like this instead of the below
    set->first = *get_first_element(set);
    
    new->next = get_successor(set, new);
    (*parent)->next = get_successor(set, *parent);
  } else { // element already exists
    free_element(set, new);
  }
}

void delete_element(set_t* set, void* target_key)
{
  if(set_is_empty(set)) {
    return;
  }

  element_t** new = search(set, target_key);
  if(element_is_empty(*new)) {
    return;
  }
  element_t* old = *new;
  element_t* old_pred = get_predecessor(set, old);
  if (element_is_empty(old->left)) { // < two children  
    *new = old->right;
  } else if (element_is_empty(old->right)) { // < two children
    *new = old->left;
  } else { // two children
    *new = old->left;
    old_pred->right = old->right;
  }
  set->count--;
  set->needs_reindexing = true;
  free_element(set, old);
  if(old_pred) {
    old_pred->next = get_successor(set, old_pred);
  }
}

// iterator
iter_t* iter_new(set_t* set)
{
  iter_t* iter = calloc(1, sizeof(iter_t));
  check_allocation(iter);
  iter->set = set;
  iter_reset(iter);
  return iter;
}

void iter_free(iter_t* iter)
{
  assert(iter);
  free(iter);
}

void iter_nsteps(iter_t* iter, int n)
{
  assert(iter);
  if (n > 0) {
    while(n > 0) {
      iter_next (iter);
      n--;
    }
  }
}

void iter_toindex(iter_t* iter, int i)
{
  int n = i - iter_index(iter); // steps from current index
  if(n > 0) { // target is ahead
    iter_nsteps(iter, n);
  } else { // target is behind
    iter_reset (iter);
    iter_nsteps(iter, i);
  }
}

bool iter_end(iter_t* iter)
{
  if(iter->set->needs_reindexing) {
    iter_reset(iter);
  }
  return iter->index >= set_getcount(iter->set)-1;
}

bool iter_start(iter_t* iter)
{
  if(iter->set->needs_reindexing) { iter_reset(iter); }
  return iter->index <= 0;
}

void iter_reset(iter_t* iter)
{
  iter->current = *get_first_element(iter->set);
  // iter->current = iter->set->first;
  iter->index = 0;
  iter->set->needs_reindexing = false;
};

void* iter_getdata(iter_t* iter)
{
  if(element_is_empty(iter->current) || iter->set->needs_reindexing) {
    iter_reset(iter);
  }
  return get_element_data(iter->current);
}

int iter_index(iter_t* iter)
{
  if(set_is_empty(iter->set)) {
    return -1;
  }
  if(element_is_empty(iter_get(iter)) || iter->set->needs_reindexing) {
    iter_reset(iter);
  }
  return iter->index;
}

