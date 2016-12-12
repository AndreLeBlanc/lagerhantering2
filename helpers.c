#define DEBUG
#define STRMAX 128

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "alloc_check.h"
#include "helpers.h"
#include "storage.h"

/** Lists a range of goods of a storage.
 * \param s The
 storage.
 * \param start The start of the range.
 * \param end The end of the range.
 * \returns the last listed goods if start index is valid,
 * otherwise start index is returned.
*/
int uiListGoods(StoreT* s, int start, int end)
{
  int count = getCount(s);
  while(start < count && start < end ) {
    printf("  %d. %s\n", (start+1), getName(getGoods(s, start)));
    start++;
  }
  return start-1;
}

void goods_print_fun1 (GoodsT* g)
{
  printf("%s", getName(g));
}

void goods_print_fun2 (GoodsT* g)
{
  printf("%d x %s", goods_get_amount(g), getName(g));
}

/** Lists a range of goods of a storage.
 * \param s The store
 * \param start The start of the range.
 * \param end The end of the range.
 * \param goods_print_fun Takes a goods and prints it in one line.
 * \returns the last listed goods if start index is valid,
 * otherwise start index is returned.
*/
int store_list_goods(StoreT* s, int start, int end,  void goods_print_fun(GoodsT* g))
{
  int count = getCount(s);
  GoodsT* g;
  while(start < count && start < end ) {
    g = getGoods(s, start);
    printf("  %d. ", (start+1));
    goods_print_fun(g);
    puts("");
    start++;
  }
  return start-1;
}

void stringtoupper (char* s) {
  for(int i = 0; s[i]; i++) {
    s[i] = toupper(s[i]);
  }
}


bool isnum (char* n)
{
  if (!n) {
    return false;
  }
  int i = 0;

  while (n[i]) {
    if (!isdigit (n[i])) {
      return false;
    }
    i++;
  }
  return i;
}

// Achievement O43 (optimization): old isnum
/* bool isnum (char* n) */
/* { */
/*   if (!n) { */
/*     return false; */
/*   } */
/*   int i = 0; */

/*   while (n && n[i]) { // removed the check for n */
/*     if (!isdigit (n[i])) { */
/*       return false; */
/*     } */
/*     i++; */
/*   } */
/*   return strlen(n); // instead of strlen the index is used */
/* } */

void readString (char** out, int maxBuffer)
{
  *out = calloc(1, maxBuffer+1); // TODO only reserve input length?
  check_allocation(*out);
  fgets(*out, maxBuffer, stdin);
  (*out)[ strlen(*out)-1 ] = '\0';
}

// prompts

char askForChar (char* valid, char* menu, char showMenu)
{
  char* q = "Enter command:";
  char* in;
  bool invalid = false;
  char a = '0';

  printf("%s\n", menu);

  do {
    if (invalid) {
      printf("Invalid: '%s'. Press '%c' for the menu.\n", in, showMenu);
    }
    printf("%s ", q);
    readString(&in, STRMAX);
    
    a = toupper(in[0]);
    invalid = strlen(in) != 1
      || (a != showMenu && strchr(valid, a) == NULL);
    
    free(in);

    if (a == showMenu) {
      printf("\n%s\n", menu);
    }
  } while(invalid);
  return a;
}

void askForString (char* question, char** out, int maxBuffer)
{
  printf ("%s ", question);
  readString(out, maxBuffer);
}

int askForInt (char* question)
{
  char* in;
  bool invalid = false;
  int r = -1;
  do {
    printf("%s ", question);
    readString(&in, 128);
    invalid = !isnum(in);
    if(invalid) {
      printf("Invalid: '%s'. ", in);
    } else {
      r = atoi(in);
    }
    free(in);
  } while(invalid);
  return r;
}

int askValidInt(char* question, bool validFun(int))
{
  int out = askForInt(question);
  while(!validFun(out)) {
    printf("Invalid value '%d'!\n", out);
    out = askForInt(question);
  }
  return out;
}

void askSetInt(GoodsT* g, char* attributeName, bool validFun(int), void setFun(GoodsT*, int))
{
  int buf;
  bool valid = true; 
  do {
    if (!valid) { printf("Invalid %s '%d'!\n", attributeName, buf); }
    char question[512];
    sprintf(question, "New %s:", attributeName); // todo comment length limit of 512-7
    buf = askForInt(question);
    valid = validFun(buf);  
  } while (!valid);
  setFun (g, buf);
}

int store_browse_goods(StoreT* s, void goods_print_fun(GoodsT*))
{
  int count = getCount(s);
  char* menuDone  = "[choose index]                              [Q]uit browser\nEnter choice:";
  char* menuStart = "[choose index]                 [N]ext 20    [Q]uit browser\nEnter choice:";
  char* menuMid   = "[choose index]    [P]rev 20    [N]ext 20    [Q]uit browser\nEnter choice:";
  char* menuEnd   = "[choose index]    [P]rev 20                 [Q]uit browser\nEnter choice:";
  char* menuCurrent;
  char* cmd = "";
  char* msg = "";
  int max = 20; // max to list at a time
  printf("===== Goods browser =====\n");  
  int i = store_list_goods(s, 0, max, goods_print_fun);
  bool loop = true;
  
  while (loop) {
    bool prevExist = i >= max; // i==19 => no, i=>20 => yes
    bool nextExist = i < count - 1;

    if      (!prevExist && !nextExist) { menuCurrent = menuDone;  }
    else if (!prevExist &&  nextExist) { menuCurrent = menuStart; }
    else if ( prevExist &&  nextExist) { menuCurrent = menuMid;   }
    else                               { menuCurrent = menuEnd;   }

    askForString (menuCurrent, &cmd, 128);
    stringtoupper(cmd);

    if(strcmp(cmd, "P") == 0) {
      if(!prevExist) {
        msg = "Invalid 'P', no preceding goods!";
      } else {
        i -= 2*max;
        if (i < 0) { i = 0; }
        i = store_list_goods(s, i, i+max, goods_print_fun);
      }
    } else if (strcmp(cmd, "N") == 0) {
      if(!nextExist) {
        msg = "Invalid 'N', no more goods!";
      } else {
        i = store_list_goods(s, i, i+max, goods_print_fun);
      }
    } else if (isnum(cmd)) {
      int chosenIndex = atoi(cmd) - 1;
      if(validIndex(s, chosenIndex)) {
        free(cmd);
        return chosenIndex;
      } else {
        msg = "Invalid index!";
      }
    }
    if(msg) {
      printf("%s\n", msg);
      msg = "";
    }
    loop = strcmp(cmd, "Q") != 0;
    free(cmd);
  }
  return -1;
}

void goods_print (GoodsT* g)
{
  printf("\n%s\n","===== Goods info =====");
  printf("NAME:\t%s\nDESCR:\t%s\nPRICE:\t%d.%d kr\n",
         getName(g), getDescription(g),
         getPrice(g)/100, getPrice(g) %100);

  shelf_t** shelves = goods_get_shelves(g);
  if(shelves != 0) {
    int i = 0;
    printf("%s", "AVAILABILITY:");
    char* shelf_name;
    while(shelves[i] != 0) {
      shelf_name = shelf_get_name(shelves[i]);
      printf(" Shelf %s: %dpcs.", shelf_name, shelf_get_amount(shelves[i]));
      i++;
      free(shelf_name);
    }
    free(shelves);
    puts("");
  }
}
