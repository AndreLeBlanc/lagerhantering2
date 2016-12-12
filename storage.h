#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>

typedef struct Goods GoodsT;
typedef struct Store StoreT;
typedef struct Shelf shelf_t; // todo rename Shelf to shelf

struct Shelf { // todo move to storage.c Temp solution to get pallet working.
  char letter;
  int  number;
  int  amount;
};

//// destructors/constructors

/** Create new store
 * Constructor for a store.
 * \returns a new empty store.
 * \note free store with \ref freeStore
 * \sa freeStore
 */
StoreT* createStore   ();

/** Free a store
 * Destructor for store
 * \param s The store
 */
void freeStore (StoreT* s);

/** Create new goods
 * Constructor for a goods
 * \param name Name for goods
 * \returns new empty goods if valid name, otherwise a null ptr.
 * \note use \ref store_insert_goods To insert new goods into store!
 * \sa store_insert_goods
 * \sa freeGoods
 * \sa validName
 */
GoodsT* createGoods (char* name); // TODO Remove. Coupling/cohesion problem. Disallow "" name

/** Free a goods
 * Destructor for goods
 * \param g The goods
 */
void freeGoods (GoodsT* g); // TODO Remove. Coupling/cohesion problem.

//// getters

/** Get goods in store by index
 * \param s The store
 * \param i The index
 * \returns The goods if valid index, otherwise a null ptr.
 * \sa validIndex
 * \sa store_search_goods
 */
GoodsT* getGoods (StoreT* s, int i); // todo remove

/** Get count of goods in store
 * \param s The store
 * \returns The count of goods starting from 0.
 */
int getCount (StoreT* s);

/** Get pointer to a copy of a goods
 * \param g The goods
 * \returns The ptr if input goods was valid, otherwise a null ptr.
 */
GoodsT* getGoodsCpyPtr (GoodsT*g );

/** Get name of goods
 * \param g The goods
 * \returns The name if input goods was valid, otherwise a null ptr.
 */
char* getName (GoodsT* g);

/** Get description of a goods
 * \param g The goods
 * \returns The description if input goods was valid, otherwise a null ptr.
 */
char* getDescription (GoodsT* g);

/** Get price of a goods
 * \param g The goods
 * \returns The price if input goods was valid, otherwise always 0.
 */
int getPrice (GoodsT* g);

/** Get available amount of goods
 * \param g The goods
 * \returns Number of available goods.
 * \invariant Goods can't be null.
 */
int goods_get_amount(GoodsT* g);

/** Get shelves of a goods
 * \param g The goods
 * \returns Array of pointers to shelves.
 * \warning Returned array needs to be freed with "free (array)"
 * \sa free
 */
shelf_t** goods_get_shelves (GoodsT* g);

/** Get name of a shelf
 * \param shelf The shelf
 * \returns A newly allocated string representing the name
 * \invariant Shelf can't be null.
 * \warning Caller responsible for freeing the name.
 * \sa free
 */
char* shelf_get_name (shelf_t* shelf);

/** Get a shelf's amount
 * \param shelf The shelf.
 * \returns The amount of goods on the shelf.
 * \invariant Shelf can't be null.
 */
int shelf_get_amount (shelf_t* shelf);

//// setters

/** Set name of a goods.
 * Sets name of a goods. If name is invalid nothing is set.
 * \param s Store with goods
 * \param goods_name Name of goods to change
 * \param n The new name
 * \note Use \validName and \store_has_goods to ensure the goods can be inserted
 * using \ref insertGoods.
 * \invariant \ref validName
 * \sa validName
 * \sa store_has_goods
 */
void goods_set_name (StoreT* s, char* goods_name, char* new_name);

/** Set price of a goods.
 * \param s Store with goods in it.
 * \param goods_name Name of goods
 * \param price New price.
 */
void goods_set_price (StoreT* s, char* goods_name, int price);

/** Set description of a goods.
 * \param s Store with goods in it.
 * \param goods_name Name of goods
 * \param description New description.
 */
void goods_set_description (StoreT* s, char* goods_name, char* description);

//// searchers

/** Search for index of name
 * \param s The store.
 * \param n Name to search for.
 * \returns -1 if not found, otherwise the index.
 */
int searchName (StoreT* s, char* n);

/** Check if store contains a certain goods
 * \param s The store.
 * \param goods_name Name of goods
 * \returns 'true' if found, otherwise 'false'.
 */
bool store_has_goods(StoreT* s, char* goods_name);

/** Check if store contains a certain shelf
 * \param s The store.
 * \param shelf_name Name of shelf
 * \returns 'true' if found, otherwise 'false'.
 */
bool store_has_shelf(StoreT* s, char* shelf_name);

/** Searches and returns goods in store.
 * Uses goods name to search.
 * \param s The store.
 * \param goods_name Name to search for.
 * \returns Goods if found, otherwise a null ptr.
 * \note "store_search_goods(g, shelf_goods) == 0" checks if store has shelf
 */
GoodsT* store_search_goods (StoreT* s, char* goods_name);

/** Searches and returns shelf in store.
 * Uses shelf name to search.
 * \param s The store.
 * \param shelf_name Name of shelf to search for.
 * \returns Shelf if found, otherwise a null ptr.
 * \note if(store_search_shelf(g, shelf_name)) checks if store has shelf
 */
shelf_t* store_search_shelf(StoreT* s, char* shelf_name);

/** Searches and returns shelf of a goods
 * if it exists. Uses shelf name to search.
 * \param g The goods.
 * \param shelf_name Name of shelf to search for.
 * \returns Shelf if found, otherwise a null ptr.
 * \note if(goods_get_shelf(g, shelf_name)) checks if goods has shelf
 */
shelf_t* goods_search_shelf(GoodsT* g, char* shelf_name);

//// validators

/** Check if a goods name is valid
 * \param name The name
 * \returns 'true' if valid, 'false' otherwise
 */
bool validName (char* name);

/** Check if a shelf name is valid
 * \param name The name
 * \returns 'true' if valid, 'false' otherwise
 */
bool validShelfName(char* name);

/** Check if an index to a goods exists
 * \param s The store to check in
 * \param i The index to check for
 * \returns 'true' if index exists, 'false' otherwise
 */
bool validIndex (StoreT* s, int i);

/** Check if a goods price is valid
 * \param price The price
 * \returns 'true' if valid, 'false' otherwise
 */
bool validPrice (int price);

/** Check if an amount (for goods in a shelf) is valid
 * \param name The amount
 * \returns 'true' if valid, 'false' otherwise
 */
bool validAmount(int amount);

//// inserters/removers

/** Add goods to a shelf
 * Adds specified amount of goods to a shelf.
 * \param s The store the goods belongs to.
 * \param goods_name Name of the goods to add to the shelf.
 * \param shelf_name Name of the shelf.
 * \param amount Amount to add or increment.
 * \warn Nothing done if goods non-existent, bad shelf, shelf occupied by another goods or amount < 1.
 * \warn If shelf occupied by this goods existing amount is incremented by given amount.
 */
void goods_add_to_shelf (StoreT* s, char* goods_name, char* shelf_name, int amount);

/** Take goods from storage
 * Take specified amount of goods from storage.
 * \param g Goods to take.
 * \param amount Amount to take.
 * \returns If nothing taken or bad amount/goods a null ptr.
 * Otherwise an array of \ref shelf_t showing taken amount from each shelf. Array ends with 0.
 * \note Returned array needs to be freed by caller.
 * \warning Nothing done if invalid goods, amount <= 0 or amount > available. \ref goods_get_amount checks availability.
 * \sa shelf_get_name
 * \sa shelf_get_amount
 * \sa goods_get_amount
 * \sa free
 */
shelf_t* goods_take (GoodsT* g, int amount);

/** Insert new goods into store.
 * \param s The store.
 * \param name Name of new goods.
 * \param description Description of new goods.
 * \param price Price of new goods in Swedish Öre.
 * \param saveUndo Whether to save an undo step upon insertion.
 * \warning If name, description or amount invalid nothing is added. Name must be new!
 * \note To add goods to shelves use \ref goods_add_to_shelf, to take goods use \ref goods_take.
 * \note To make changes to existing goods use setters.
 * \sa validName
 * \sa validAmount
 * \sa goods_add_to_shelf
 * \sa goods_take
 */
void store_insert_goods (StoreT* s, char* name, char* description, int price, bool saveUndo);

/** Remove a goods from store.
 * \param s The store.
 * \param name Name of goods to remove.
 * \param saveUndo Whether to save an undo step upon removal.
 * \note If no such goods exists in store nothing is removed.
 * \sa store_has_goods
 */
void store_remove_goods(StoreT* s, char* name, bool saveUndo);

/** Replace a goods in a store.
 * Replace one goods in a store with another.
 * \param s The store.
 * \param old Goods to replace.
 * \param new Replacement goods.
 * \param saveUndo Whether to save undo state.
 * \warning Makes copies of old and new, user has to free them both after call.
 */
void replaceGoods (StoreT* s, GoodsT* old, GoodsT* new, bool saveUndo);

/** Populate store with test goods
 * \param s The store
 * \param amount Amount to add.
 */
void addNTestGoods (StoreT* s, int amount);  // TODO low cohesion, move?

//// undo
/** Undos last action made in store
 * \param s The store.
 * \note If there are no more undo steps to do nothing changes.
 */
void undoLastAction(StoreT* s);

#endif //STORAGE_H
