//#define DEBUG
//#define DEBUGHIGH
#define PRINTSET
  //  set_foreachdata(s->goods, set_printer); 

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "storage.h"
#include "bst.h"
#include "helpers.h"

enum UndoType { INSERT, REMOVE, SET };

struct Goods {
  char* name;
  char* description;
  int   price;          // in Swedish öre
  set_t* shelves;
};

struct UndoStep {
  enum UndoType type;
  struct Goods* goods;
  int index; // todo remove index from undo
};

struct UndoDB {
  int count;
  int max;
  struct UndoStep* undoSteps;
};

/**
 * \invariant No two goods can share the same shelf.
 */
struct Store {
  set_t* goods;
  iter_t* iterator;
  struct UndoDB undoDB;
};

/*************** undo functions *****************/
struct UndoDB createUndoDB (int MAX) {
  struct UndoDB db;
  db.max = MAX;
  db.count = 0;
  db.undoSteps = calloc(db.max, sizeof(struct UndoStep));
  return db;
}

void freeUndoStep (struct UndoStep u) {
  freeGoods(u.goods);
}

void freeUndoDB(struct UndoDB* db)
{
  if(db) {
    for(int i = 0; i < (db->count); i++) {
      freeUndoStep(db->undoSteps[i]);
    }
    free(db->undoSteps);
  }
}

void popUndoStep(StoreT* s) 
{
  if(s->undoDB.count < 1) {
    return;
  }
  freeUndoStep(s->undoDB.undoSteps[0]); // TODO more than one undo
  s->undoDB.count--;
}

void pushUndoStep(StoreT* s, enum UndoType type, GoodsT* pg) // todo
{
  /*popUndoStep(s);
  struct UndoStep newUndoStep = {.type = type, .goods = getGoodsCpyPtr(g)};
  s->undoDB.undoSteps[0] = newUndoStep;
  s->undoDB.count = 1;*/
}

// other
void setString(char** s, char* n)
{
  if(*s) { // todo error
    free(*s);
  }
  *s = calloc(strlen(n)+1, sizeof(char));
  strcpy(*s, n);
}

void setEmpty(GoodsT* g)
{
  g->name = 0;
  g->description = 0;
  g->price = 0; // todo shelves
}

bool isEmpty (GoodsT* g)
{
  bool result = \
    getName (g) == 0 &&
    getDescription(g) == 0 &&
    getPrice (g) == 0 &&
    goods_get_amount(g) == 0 &&
    set_is_empty(g->shelves);
  return result;
}

shelf_t* store_search_shelf(StoreT* s, char* shelf_name)
{
  if(set_is_empty(s->goods) || !validShelfName(shelf_name)) {
    return NULL;
  }
  iter_t* iter = iter_new(s->goods);
  shelf_t* found;
  while (!iter_end(iter)) {
    found = goods_search_shelf(iter_getdata(iter), shelf_name);
    if (found) {
      iter_free(iter);
      return found;
    }
    iter_nsteps(iter,1);
  }
  found = goods_search_shelf(iter_getdata(iter), shelf_name);
  if (found) {
      iter_free(iter);
    return found;
  }
  iter_free(iter);
  return NULL;
}

shelf_t* goods_search_shelf(GoodsT* g, char* shelf_name)
{
  if(!validShelfName(shelf_name)) {
    return NULL;
  }
  return searchdata(g->shelves, shelf_name);
}

/** Sets name of a shelf
 * Sets name of a shelf. If shelf name is invalid nothing is set.
 * \param shelf The shelf.
 * \param n New shelf name.
 * \invariant \ref validShelfName
 * \sa validShelfName
 */
void shelf_set_name (shelf_t* shelf, char* name)
{
  if(!validShelfName(name)) {
    return;
  }
  
  char  letter = toupper (name[0]);
  char* number = name+1;
  shelf->letter = letter;
  shelf->number = atoi(number);
}

/** Copy a shelf.
 * Gets newly allocated copy of shelf.
 * \param shelf Shelf to copy.
 * \returns Pointer to copy.
 * \sa free
 */
shelf_t* shelf_get_copy(shelf_t* shelf)
{
  shelf_t* copy = calloc(1, sizeof(shelf_t));
  *copy = *shelf;
  return copy;
}

/** Get the shelf with highest amount of a goods in it
 * \param g The goods
 * \returns Shelf with the highest amount of goods if goods is available, otherwise a null ptr.
 */
shelf_t* goods_get_largest_shelf (GoodsT* g)
{
  if(goods_get_amount(g) <= 0) {
    return NULL;
  }

  iter_t* iter = iter_new(g->shelves);
  shelf_t* largest = iter_getdata(iter);
  shelf_t* temp;
  while(!iter_end(iter)) {
    iter_nsteps(iter,1);
    temp = iter_getdata(iter);
    if(shelf_get_amount(temp) > shelf_get_amount(largest)) {
      largest = temp;
    }
  }  
  iter_free(iter);
  return largest;
}

void* shelf_key_generator (void* data)
{
  char* name = shelf_get_name((shelf_t*)data);
  char* key  = calloc(strlen(name)+1, sizeof(char));
  strcpy(key, name);
  free(name);
  return key;
}


/** Insert new shelf to goods
 * \param g The goods.
 * \param shelf_name Name of new shelf.
 * \param amount Amount in new shelf.
 * \invariant amount has to be > 0.
 */
void goods_insert_shelf (GoodsT* g, char* shelf_name, int amount)
{
  if (amount <= 0) {
    return;
  }
  // Achievement O44: Optimized insert_element (doesn't copy data)
  shelf_t* shelf = calloc(1, sizeof(shelf_t));
  shelf_set_name(shelf, shelf_name);
  shelf->amount = amount;
  insert_element(g->shelves, shelf);
  // before optimization
  /* shelf_t shelf; */
  /* shelf_set_name(&shelf, shelf_name); */
  /* shelf.amount = amount; */
  /* insert_element(g->shelves, &shelf); */
}

// returns index of goods, -1 if not found
int searchString (StoreT* s, char* string, char* (*getStringFunction) (GoodsT*))
{
  GoodsT* g;
  for (int i = 0; i < getCount(s); i++) {
      g = getGoods(s, i);
      char* inString = (*getStringFunction) (g);
      if ( strcmp( inString, string) == 0 ) {
        return i;
      }
      if(inString) {free(inString); } // todo ugly workaround in case getStringFunction is getShelfName
    }
  return -1;
}

/**************** bst functions *****************/

void* goods_key_generator (void* data)
{
  char* name = getName((GoodsT*)data);
  char* key  = calloc(strlen(name)+1, sizeof(char));
  strcpy(key, name);
  return key;
}

int string_comparator (void* key1_in, void* key2_in)
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

void set_key_freer  (void* key)  { assert(key); free(key);   }

void* set_data_copier(void* data) { return getGoodsCpyPtr(data); }

void set_data_freer (void* data) { freeGoods(data); }

void* shelf_data_copier (void* data)
{
  shelf_t* copy = calloc(1, sizeof(shelf_t));
  *copy = *((shelf_t*)data);
  return copy;
}

GoodsT getGoodsCpy (GoodsT* original)
{
  GoodsT copy = *original;  

  char* t = getName(&copy);
  copy.name = calloc(strlen(t)+1, sizeof(char));
  if(t) {
    strcpy (copy.name, t);
  }
  
  t = getDescription(&copy);
  copy.description = calloc(strlen(t)+1, sizeof(char));
  if(t) {
    strcpy (copy.description, t);
  }

  copy.shelves = copy_set(original->shelves);

  return copy;
}

GoodsT* getGoodsCpyPtr (GoodsT* original)
{
  GoodsT* g = calloc (1, sizeof(GoodsT)); 
  *g = getGoodsCpy(original);
  return g;
}


//////////////////////////////////////////////////
//                    PUBLIC                    //
//////////////////////////////////////////////////

// constructors
StoreT* createStore()
{
  StoreT *s = calloc(1, sizeof(StoreT));
  s->goods = new_set(&goods_key_generator, &string_comparator, &set_key_freer, &set_data_copier, &set_data_freer);
  s->iterator = iter_new(s->goods);
  s->undoDB = createUndoDB(100);
  return s;
}

GoodsT* createGoods(char* name)
{
  GoodsT* g = calloc(1, sizeof(GoodsT));
  setString(&g->name, name);
  g->shelves = new_set(&shelf_key_generator, &string_comparator, &set_key_freer, &shelf_data_copier, &free);
  return g; // TODO if alloc returns null
}

// destructors
void freeGoods (GoodsT* g)
{
  if (g) {
    if(g->name) {
      free(g->name);
    }
    if(g->description) {
      free(g->description);
    }
    if(g->shelves) {
      free_set(g->shelves);
    }
    free(g);
  }
}

void freeStore (StoreT* s)
{
  if(!s) {
    return;
  }
  free_set(s->goods);
  iter_free(s->iterator);
  freeUndoDB(&s->undoDB);
  free(s);
}

/****************** insertion *******************/

void store_insert_goods (StoreT* s, char* name, char* description, int price, bool saveUndo)
{
  if(!s || !validName(name) || !validPrice(price) || store_has_goods(s, name)) {
    return;
  }
  GoodsT* g = createGoods(name);
  g->description = calloc(strlen(description)+1, sizeof(char));
  strcpy(g->description, description);

  g->price = price;
  
  if(saveUndo) {
    pushUndoStep(s, INSERT, g);
  }
  
  insert_element(s->goods, g);
  // Achievement O44: Optimized insert_element (doesn't copy data)
  /* freeGoods(g); */
}

// todo Bad logic allover, figure out how to do properly
void replaceGoods (StoreT* s, GoodsT* old, GoodsT* new, bool saveUndo)
{
  bool validShelves = true;
  //!validShelfName(getShelfName(new))
  if(!validName(getName(new)) || isEmpty(old) || isEmpty(new) || !validShelves) {
    return;
  }
  if (strcmp(getName(old), getName(new)) != 0) {
    assert(!store_has_goods (s, getName (new)));
  } else {
    /*char* oldShelfName = getShelfName(old);
    char* newShelfName = getShelfName(new);
    if(strcmp(oldShelfName, newShelfName) != 0) {
      assert(!store_has_shelf(s, newShelfName));
    }
    free(oldShelfName);
    free(newShelfName);*/
  }

  if(saveUndo) {/* todo
    pushUndoStep(s, SET, old);
    pushUndoStep(s, SET, new);*/
  }
  store_remove_goods(s, getName(old), false);
  store_insert_goods(s, getName(new), getDescription(new), getPrice(new), false);
}

void store_remove_goods (StoreT* s, char* goods_name, bool saveUndo)
{
  GoodsT* goods = store_search_goods(s, goods_name);
  if(!goods) {
    return;
  }
  if (saveUndo) {
    pushUndoStep(s, REMOVE, goods);
  }
  char* key = goods_key_generator(goods);
  delete_element(s->goods, key);
  free(key);
}

void addNTestGoods (StoreT* s, int numberOfGoods)
{
  char name[128];
  char description[128];
  char shelfName[128];
  for(int i = 1; i <= numberOfGoods; i++) {
    sprintf (name, "name%d",i);
    sprintf (description, "description%d",i);
    store_insert_goods(s, name, description, i*100, false);
    sprintf (shelfName, "G%d", i);
    goods_add_to_shelf(s, name, shelfName, i);
    sprintf (shelfName, "H%d", i);
    goods_add_to_shelf(s, name, shelfName, i*10);
  }
}

/****************** getters *********************/
int getCount (StoreT* s)
{
  if(!s) {
    return 0;
  }
  return set_getcount(s->goods);
}

char* getName (GoodsT* g)
{
  if(!g) {
    return NULL;
  }
  return g->name;
}

int getPrice (GoodsT* g)
{
  if(!g) {
    return 0;
  }
  return g->price;
} // In Öre.

char* getDescription(GoodsT* g)
{
  if(!g) {
    return NULL;
  }
  return g->description;
}

int goods_get_amount (GoodsT* g)
{
  if(!g) {
    return 0;
  }
  if(set_is_empty(g->shelves)) {
    return 0;
  }
  int count = 0;
  iter_t* iter = iter_new(g->shelves);
  while (!iter_end(iter)) {
    count += shelf_get_amount(iter_getdata(iter));
    iter_nsteps(iter, 1);
  }
  count += shelf_get_amount(iter_getdata(iter));
  iter_free(iter);
  return count;
}

shelf_t** goods_get_shelves (GoodsT* g)
{
  if(!g) {
    return NULL;
  }
  int count = set_getcount(g->shelves);
  if(count < 1) {
    return NULL;
  }
  iter_t* iter = iter_new(g->shelves);
  shelf_t** shelves = calloc(count+1, sizeof(shelf_t*));
  int i = 0;
  shelves[i] = iter_getdata(iter);
  while(!iter_end(iter)) {
    i++;
    iter_nsteps(iter, 1);
    shelves[i] = iter_getdata(iter);
  }
  iter_free(iter);
  return shelves;
}

int shelf_get_amount(shelf_t* shelf)
{
  if(!shelf) {
    return 0;
  }
  return shelf->amount;
}

char* shelf_get_name (shelf_t* shelf)
{
  if(!shelf) {
    return calloc(1, sizeof(char));
  }
  int length = 15; // TODO = 1+nr_of_digits+1
  char* out = calloc(length, sizeof(char));
  sprintf(out,"%c%d", shelf->letter, shelf->number);
  return out;
}

GoodsT* getGoods(StoreT* s, int i) // todo remove
{
  if(!s || !validIndex(s, i)) {
    return NULL;
  }
  iter_toindex(s->iterator, i);
  return (GoodsT*)iter_getdata(s->iterator);
}


/****************** setters *********************/
void goods_set_name (StoreT* s, char* old_name, char* new_name)
{
  if(!s || !validName(new_name) || !old_name || !new_name  || strcmp (old_name, new_name) == 0) {
    return;
  }
  GoodsT* old = store_search_goods(s, old_name);
  if(!old) {
    return;
  }
  GoodsT* new = getGoodsCpyPtr(old);
  setString(&new->name, new_name);

  store_remove_goods(s, old_name, false);
  insert_element(s->goods, new);
  // Achievement O44: Optimized insert_element (doesn't copy data)
  /* freeGoods(new); */
}

void goods_set_description (StoreT* s, char* goods_name, char* description)
{
  GoodsT* g = store_search_goods(s, goods_name);
  if(!g) {
    return;
  }
  setString(&g->description, description);
}

void goods_set_price (StoreT* s, char* goods_name, int price)
{
  if(!validPrice(price)) {
    return;
  }
  GoodsT* g = store_search_goods(s, goods_name);
  if(!g) {
    return;
  }
  g->price = price;
}

void goods_add_to_shelf (StoreT* s, char* goods_name, char* shelf_name, int amount)
{
  GoodsT* goods = store_search_goods(s, goods_name);
  if(!goods || !validName(goods_name) || !shelf_name || !validAmount(amount)) {
    return;
  }
  shelf_t* shelf = goods_search_shelf(goods, shelf_name);
  if (shelf) {
    shelf->amount += amount;
  } else if (store_search_shelf(s, shelf_name) != NULL) {
    return;
  } else {
    goods_insert_shelf (goods, shelf_name, amount);
  }
}

shelf_t* goods_take (GoodsT* g, int asked_amount)
{
  if(!g || asked_amount < 1 || asked_amount > goods_get_amount(g)) {
    return NULL;
  }
  
  shelf_t* shelves_taken = calloc(set_getcount(g->shelves)+1, sizeof(shelf_t));
  shelf_t* shelf_taken;
  shelf_t* shelf;

  for(int i = 0; asked_amount > 0; i++) {
    shelf = goods_get_largest_shelf(g);
    shelf_taken = shelf_get_copy(shelf);
    if(shelf->amount > asked_amount) {
      shelf->amount -= asked_amount;
      shelf_taken->amount = asked_amount;
    } else {
      char* key = shelf_key_generator(shelf);
      delete_element(g->shelves, key);
      free(key);
    }
    asked_amount -= shelf_taken->amount;
    *(shelves_taken+i) = *shelf_taken;
    free(shelf_taken);
  }
  return shelves_taken;
}

//// searchers

// return goods' index, -1 if not found
int searchName (StoreT* s, char* n) // todo remove
{
  if(!s) {
    return -1;
  }
  GoodsT* g;
  int count = getCount(s);
  for (int i = 0; i < count; i++) {
    g = getGoods(s, i);
    if (strcmp(getName(g), n) == 0) {
      return i;
    }
  }
  return -1;
}

GoodsT* store_search_goods (StoreT* s, char* goods_name)
{
  if(!s || !goods_name) {
    return NULL;
  }
  return searchdata(s->goods, goods_name);
}

bool store_has_goods(StoreT* s, char* goods_name)
{
  return store_search_goods(s, goods_name);
}

bool store_has_shelf(StoreT* s, char* shelf_name)
{
  return store_search_shelf(s, shelf_name);
}


/*************** data validators ****************/
bool validName (char* n)
{
  return n != NULL && n && strlen (n) > 0;
}

bool validShelfName(char* n)
{
  return n != NULL && n && isalpha(*n) && isnum(n+1);
}

bool validIndex (StoreT* s, int i)
{
  return i >= 0 && i < getCount(s);
}

bool validPrice (int i)
{
  return i >=0;
}

bool validAmount (int i)
{
  return i >=0;
}

//// undo

void undoLastAction(StoreT* s) // todo
{
  if(s->undoDB.count <= 0) {
    return;
  }
  struct UndoStep u = s->undoDB.undoSteps[s->undoDB.count-1];
  switch (u.type) {
  case INSERT:
    store_remove_goods(s, getName(u.goods), false);
    break;
  case REMOVE:
    store_insert_goods(s, getName(u.goods), getDescription(u.goods), getPrice(u.goods), false);
    // todo insert goods' shelves
    break;
  case SET:
    //replaceGoods(s, &u.goods, false); // todo, two steps in one
    s->undoDB.count++;
    break;
  default:
    break;
  }
  s->undoDB.count--;
}
