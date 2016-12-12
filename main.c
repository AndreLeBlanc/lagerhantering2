#include <stdio.h>
#include "storage.h"   // TODO high coupling
#include "storageui.h"


int main(int argc, char** argv)
{
  StoreT* s = createStore();
  
  addNTestGoods(s, 500);
  startMainMenu(s);
 
  freeStore(s);

  return 0;
}
