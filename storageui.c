#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include "storageui.h"
#include "storage.h"
#include "helpers.h"
#include "palletui.h"

// ui functions directly related to storage

/** Prompts user to enter a valid name.
 * The valid name is written to 'out'.
 * \param out Address to write the answer to.
 * \param stringMax Max chars in answer.
 * \invariant out has to have stringMax characters allocated
 */
void uiGetName (char* out, int stringMax)
{
  char* buf;
  bool valid = false;  
  while (!valid) {
    askForString("New name:", &buf, stringMax);
    valid = validName(buf);
    if (!valid) {
      printf("Invalid name '%s'!\n", buf);
    }
  }
  strcpy(out, buf);
  free(buf);
}

void uiSetNGoodsToShelf (StoreT* s, char* goodsName)
{
  GoodsT* g = store_search_goods(s, goodsName);
  assert(g != 0 && "goods doesn't exist");

  char* shelf;
  bool valid = true;  
  bool takenByThis = false;
  bool taken = false;
  
  do {
    askForString("Shelf to put goods on:", &shelf, STRMAX);
    valid = validShelfName(shelf);
    if (!valid) {
      printf("Invalid shelf name '%s'!\n", shelf);
      free(shelf);
      continue;
    }
    takenByThis = goods_search_shelf(g, shelf); 
    if(takenByThis != 0) {
      break;
    }
    taken = store_has_shelf(s, shelf);
    if (taken != 0) {
      printf("Invalid, '%s' already taken by another goods!\n", shelf);
      free(shelf);
      continue;
    }
  } while (!valid);

  int amount = -1;
  amount = askForInt("Enter amount to put in shelf:");
  while (!validAmount(amount) || amount < 1) {
    printf("Amount '%d' invalid, must be over '0'!", amount);
    amount = askForInt("Enter amount to put in shelf:");
  }
  goods_add_to_shelf(s, goodsName, shelf, amount);
  free(shelf);
}

void uiEditShelves(StoreT* s, char* goodsName)
{
  uiSetNGoodsToShelf(s, goodsName); // todo repeat until user is done
}

void uiEditGoodsRunCmd(StoreT* s, char* goodsName, char command)
{
  char buf[STRMAX];
  char* description;
  int price = -1;
  switch(toupper(command)){
  case 'N' :
    while(true) {
      uiGetName(buf, STRMAX);
      if(strcmp(goodsName, buf) == 0) {
        printf("Name is unchanged, '%s' is the name of the opened goods!\n", buf);
        break;
      } else if(!store_has_goods(s, buf)) { // correct new name entered
        goods_set_name (s, goodsName, buf);
        break;
      }
      printf("Please pick a unique name, '%s' is taken by another goods!\n", buf);
    }
    break;    
  case 'D' :
    askForString("New description:", &description, STRMAX);
    goods_set_description(s, goodsName, description);
    free(description);
    break; 
  case 'S' :
    uiEditShelves(s, goodsName);
    break;
  case 'P' :
    price = askValidInt("New price:", validPrice);
    goods_set_price(s, goodsName, price);
    break;
  default :
      break;
  }
}

void uiAddGoods(StoreT* s)
{
  char buf[STRMAX];
  char name[STRMAX];
  char* description;
  int price;
  uiGetName(name, STRMAX);
  if(!store_has_goods(s, name)) {
    askForString("New description:", &description, STRMAX);
    price = askValidInt("Enter price:", validPrice);
    store_insert_goods(s, name, description, price, false);
    free(description);
  } else {
    printf("'%s' is already in the database.\nExisting description and price of goods is used.\n", buf);
  }
  uiEditShelves(s, name);
}

 void uiEditGoods(StoreT* s, char* goodsName) // todo
{
  GoodsT* g = store_search_goods(s, goodsName);
  
  char* menu = 
    "Editing choices: [N]ame\n"
    "                 [D]escription\n"
    "                 [S]helf and amount\n"
    "                 [P]rice\n"
    "                 [Q]uit editor\n";
  char* valid = "NDSPQ";
  char command;
 
  do {
    goods_print(g);
    puts("");
    command = askForChar(valid, menu, 'M');
    uiEditGoodsRunCmd (s, goodsName, command);
  } while(command!='Q');
}

void runCommand(StoreT* s, char c)
{
  puts(BORDER2);
  c = toupper(c);
  int i = -1;
  switch(c) {
  case 'A':
    uiAddGoods(s);
    break;
  case 'R':
    i = store_browse_goods(s, goods_print_fun1);
    if(i == -1) {
      puts("Cancelled goods removal!");
      break;
    }
    store_remove_goods(s, getName(getGoods(s, i)), true);
    break;
  case 'E':
    i = store_browse_goods(s, goods_print_fun1);
    if(i == -1) {
      puts("Cancelled goods editing!");
      break;
    }
    uiEditGoods(s, getName(getGoods(s, i)));
    break;
  case 'U':
    undoLastAction(s);
    break;
  case 'B':
    i = store_browse_goods(s, goods_print_fun1);
    if(i != -1) {
      goods_print(getGoods(s, i));
    }
    break;
  case 'P':
    packPallet(s);
  default:
    break;
  }
}

void startMainMenu (StoreT* s)
{
  char* welcomeMsg = "Starting warehouse system...\n";
  char* menu =                              \
    "===== MAIN MENU =====\n"
    " [A]dd goods\n"
    " [R]remove goods\n"
    " [E]dit goods\n"
    //" [U]ndo last action\n"
    " [B]rowse catalogue\n"
    " [P]ack pallet\n"
    " [Q]uit program\n";

  char* validCommands = "AREBPQM";
  char cmd = 0;

  puts(welcomeMsg);
  do {
    cmd = askForChar(validCommands, menu, 'M');
    if (cmd != 'Q') {
      puts("");
      runCommand(s, cmd);
      puts("");
    }
  }
  while (cmd != 'Q');
  
  puts("Quitting program...");
}

