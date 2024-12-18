// shopping_cart.h
#ifndef shopping_cart_h
#define shopping_cart_h

#include "structures.h"
#include "owner.h"

void initializeCart(ShoppingCart* cart);
int addToCart(ShoppingCart* cart, Product* product, int quantity);
void displayCart(ShoppingCart* cart);
void applyCoupon(ShoppingCart* cart, Coupon* coupon);
void checkout(ShoppingCart* cart, Product* products, int productCount);

#endif /* shopping_cart_h */
