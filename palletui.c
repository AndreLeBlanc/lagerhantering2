#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "palletui.h"
#include "pallet.h"
#include "helpers.h"
#include "storageui.h"
#include "storage.h"
#include "bst.h"

void putBackGoods(StoreT * s, StoreT * pallet) {
  int count = getCount(pallet);;
  for(int i = 0; i < count; i++) {
    GoodsT * g = getGoods(pallet, i);
    shelf_t* takenShelves = goods_take(g, goods_get_amount(g));
    char * shelfName = shelf_get_name(takenShelves);
    for (int i = 0; strcmp(shelfName, "") != 0; i++) {
      int amountGoods = shelf_get_amount(takenShelves+i);
      goods_add_to_shelf(s, getName(g), shelfName, amountGoods);
      free(shelfName);
      shelfName = shelf_get_name(takenShelves+i+1);
    }
    free(shelfName);
    free(takenShelves);
  }
}

void putBackItems (StoreT * s, StoreT * pallet) {
  putBackGoods(s, pallet);
  freeStore(pallet);
}

GoodsT * look_in_catalogue(StoreT * s)
{
    int i = store_browse_goods(s, goods_print_fun1);
    GoodsT* g = getGoods(s, i);
    // goods_print(g);
    return g;
}

int priceOfGood(GoodsT * g){
  return getPrice(g)*goods_get_amount(g);
}

int total_cost(StoreT * pallet){
  int sum = 0;
  int count = getCount(pallet);;
  for(int i = 0; i < count; i++){
    GoodsT * g = getGoods(pallet, i);
    sum += priceOfGood(g);
  }
  return sum;
}

int choose_amount(GoodsT * g){
  int max = goods_get_amount(g);
  int choice = 0;

  while (choice < 1 || max < choice){
    char * name = getName(g);
    char question[sizeof(name)+40];
    sprintf (question, "Choose how many '%s' to take (%d left):", getName(g), goods_get_amount(g));
    choice = askForInt(question);
  }
  return choice;
}


void palletAddGoods(StoreT * s, StoreT * pallet)
{
  GoodsT* g = look_in_catalogue(s);
  if(g != NULL) {
    if (goods_get_amount(g) > 0){
      char * name = getName(g);

      if (!store_has_goods(pallet, name)) {
	store_insert_goods(pallet, name, getDescription(g), getPrice(g), false);
      }

      puts("");
    
      shelf_t* takenShelves = goods_take(g, choose_amount(g));
      char* shelfName = shelf_get_name(takenShelves);
      for (int i = 0; strcmp(shelfName, "") != 0; i++){
	int amountGoods = shelf_get_amount(takenShelves+i);
	goods_add_to_shelf(pallet, name, shelfName, amountGoods);
	free(shelfName);
	shelfName = shelf_get_name(takenShelves+i+1);
      }
      free(shelfName);
      free(takenShelves);
    }

    else {
      printf("There are no %s in storage", getName(g));
    }
  }
}

void palletPacked(StoreT * s, StoreT * pallet) {
  char* pallet_menu = \
    "======= PALLET MENU =======\n"
    " [A]dd goods to pallet\n"
    " [C]heck-out\n"
    " [V]iew pallet\n"
    " [Q]uit without check-out";
  char what_to_do = 0;
  
  while (toupper(what_to_do) != 'Q' && toupper(what_to_do) != 'C') {
    int sum = total_cost(pallet);
    printf("\nTotal pallet cost: %d.%d kr\n", sum/100, sum%100);
    
    what_to_do = askForChar("ACVQ", pallet_menu,'M');
    
    switch(toupper(what_to_do)) {
    case 'A':
      palletAddGoods (s, pallet);
      break;
    case 'C':
      freeStore(pallet);
      break;
    case 'Q':
      putBackItems(s, pallet);
      break;
    case 'V':
      store_browse_goods(pallet, goods_print_fun2);
      break;
    default:
      break;
    }
  }
}

//////////////////
//    PUBLIC    //
//////////////////

// ui functions related to shopping mode

/** Start packing pallet
 * Starts the pallet packaging process.
 * \param s The storage to work against.
 */
void packPallet(StoreT* s) {
  StoreT * pallet = createStore();
  palletPacked(s, pallet);
}

