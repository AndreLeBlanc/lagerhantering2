#ifndef pallet_H
#define pallet_H

#include "bst.h"
#include "storage.h"

/**************/
/*   PUBLIC   */
/**************/

typedef set_t pallet_t;

/** Create a new pallet
 *  
 */
set_t * create_pallet();

/** Pallet destructor.
 *  Frees the pallet.
 * \param p The pallet to free.
 */
void free_pallet (pallet_t* p);

/**
 *  A function that adds an item to the pall with goods.
 */
void pallet_add_goods(pallet_t * pall, GoodsT * vara);

/** Packa pallen.
 *  This removes the chosen items from the warehouse and returns
 *  the user to the main menu of the program.
 */
void packa_pall(pallet_t * pall);

typedef void* pallet_data_copier_t (void* data);

#endif
