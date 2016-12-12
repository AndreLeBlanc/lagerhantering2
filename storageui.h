#ifndef STORAGEUI_H
#define STORAGEUI_H

#include <stdbool.h>
#include "storage.h"

#define STRMAX 128
#define BORDER1 "#########################"
#define BORDER2 "-------------------------"


/**
 Starts the user interface with a store.
 \param store The store to work against.
*/

void startMainMenu(StoreT* store);

#endif //STORAGEUI_H



