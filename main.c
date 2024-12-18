#include "owner.h"
#include "product_management.h"
#include "shopping_cart.h"
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void showMainMenu(void) {
    printf("\n=== Online Store ===\n");
    printf("1. Login for Owner\n");
    printf("2. Login for Customer\n");
    printf("0. Exit Program\n");
    printf("Select menu : ");
}

void ownerMenu(void) {
    loadProductsFromCSV();
    int choice;
    
    while(1) {
        autoRestockCheck();
        
        printf("\n=== Menu for Store Owner  ===\n");
        printf("1. Add Product\n");
        printf("2. View Product\n");
        printf("3. Edit Product\n");
        printf("4. Delete Product\n");
        printf("5. Find Product\n");
        printf("6. View Sales\n");
        printf("7. Add Stock Product\n");
        printf("8. Manage Promotion\n");
        printf("0. Back to Main Menu\n");
        printf("Select menu: ");
        scanf("%d", &choice);
        
        if (choice == 0) {
            break;
        }
        
        switch(choice) {
            case 1:
                createProduct();
                break;
            case 2:
                viewProduct();
                break;
            case 3:
                updateProduct();
                break;
            case 4:
                deleteProduct();
                break;
            case 5: {
                int searchChoice;
                printf("\n=== Find Product ===\n");
                printf("1. Search by Name\n");
                printf("2. Search by Price Range\n");
                printf("Select search method: ");
                scanf("%d", &searchChoice);
                
                if (searchChoice == 1) {
                    char searchTerm[100];
                    printf("Enter search term: ");
                    scanf(" %[^\n]s", searchTerm);
                    searchByName(searchTerm);
                } else if (searchChoice == 2) {
                    float minPrice, maxPrice;
                    printf("Enter minimum price: ");
                    scanf("%f", &minPrice);
                    printf("Enter maximum price: ");
                    scanf("%f", &maxPrice);
                    searchByPriceRange(minPrice, maxPrice);
                }
                break;
            }
            case 6: {
                int reportChoice;
                printf("\n=== Reports ===\n");
                printf("1. Sales Report\n");
                printf("2. Stock Report\n");
                printf("3. Generate Daily\n");
                printf("Select report: ");
                scanf("%d", &reportChoice);
                
                switch(reportChoice) {
                    case 1:
                        showSalesReport();
                        break;
                    case 2:
                        showStockReport();
                        break;
                    case 3:
                        generateDailyReport();
                        break;
                }
                break;
            }
            case 7:
                restockProduct();
                break;
            case 8: {
                int couponChoice;
                printf("\n=== Promotion Management ===\n");
                printf("1. Create New Coupon\n");
                printf("2. Show Coupon List\n");
                printf("3. Deactive Coupon\n");
                printf("Select Action: ");
                scanf("%d", &couponChoice);
                
                switch(couponChoice) {
                    case 1:
                        createCoupon();
                        break;
                    case 2:
                        listCoupons();
                        break;
                    case 3: {
                        char code[20];
                        printf("Enter coupon code to deactivate: ");
                        scanf(" %[^\n]s", code);
                        deactivateCoupon(code);
                        break;
                    }
                }
                break;
            }
        }
    }
}

void customerMenu(void) {
    Product products[MAX_PRODUCTS];
    int productCount = 0;
    loadProducts(products, &productCount);
    
    ShoppingCart cart;
    initializeCart(&cart);
    
    int choice;
    do {
        printf("\n=== Store Menu ===\n");
        printf("1. View All Products\n");
        printf("2. Search Products\n");
        printf("3. Filter by Price\n");
        printf("4. Add Product to cart\n");
        printf("5. View Shopping\n");
        printf("6. Apply Coupon\n");
        printf("7. Checkout\n");
        printf("0. Back to Main Menu\n");
        printf("Select menu: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                displayProducts(products, productCount);
                break;
            case 2: {
                char searchTerm[MAX_NAME];
                printf("Enter search term: ");
                scanf(" %[^\n]", searchTerm);
                searchProducts(products, productCount, searchTerm);
                break;
            }
            case 3: {
                double minPrice, maxPrice;
                printf("Enter minimum price: ");
                scanf("%lf", &minPrice);
                printf("Enter maximum price: ");
                scanf("%lf", &maxPrice);
                filterByPrice(products, productCount, minPrice, maxPrice);
                break;
            }
            case 4: {
                int productId, quantity;
                printf("Enter product ID: ");
                scanf("%d", &productId);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                
                Product* selectedProduct = NULL;
                for (int i = 0; i < productCount; i++) {
                    if (products[i].id == productId) {
                        selectedProduct = &products[i];
                        break;
                    }
                }
                
                if (selectedProduct != NULL) {
                    addToCart(&cart, selectedProduct, quantity);
                    displayCart(&cart);
                } else {
                    printf("Product not found!\n");
                }
                break;
            }
            case 5:
                displayCart(&cart);
                break;
            case 6: {
                char couponCode[20];
                printf("Enter Coupon code: ");
                scanf(" %[^\n]", couponCode);
                Coupon coupon;
                if (findCoupon(couponCode, &coupon)) {
                    applyCoupon(&cart, &coupon);
                } else {
                    printf("Coupon not found!\n");
                }
                break;
            }
            case 7:
                displayCart(&cart);
                checkout(&cart, products, productCount);
                break;
        }
    } while (choice != 0);
}

int main(void) {
    int choice;
    
    while(1) {
        showMainMenu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                ownerMenu();
                break;
            case 2:
                customerMenu();
                break;
            case 0:
                printf("Thank you for using our service!\n");
                return 0;
            default:
                printf("Please select a valid menu option!\n");
        }
    }
    
    return 0;
}
