#include "alloc_check.h"

void check_allocation(void* p) {
  if (p == NULL) {
    puts("Ran out of memory");
    exit(1);
  }
}
