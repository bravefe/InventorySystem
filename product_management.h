#ifndef product_management_h
#define product_management_h

#include "structures.h"
#include "owner.h"

void loadProducts(Product* products, int* productCount);
void saveProducts(Product* products, int productCount);
void displayProducts(Product* products, int productCount);
void filterByPrice(Product* products, int productCount, double minPrice, double maxPrice);

#endif /* product_management_h */
