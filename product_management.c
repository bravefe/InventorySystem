// product_management.c
#include "product_management.h"
#include <ctype.h>
#include <string.h>

void loadProducts(Product* products, int* productCount) {
    FILE* file = fopen("products.csv", "r");
    if (file == NULL) {
        printf("Error opening products file\n");
        return;
    }
    
    *productCount = 0;
    char line[1024];
    
    // Skip header line
    // fgets(line, 1024, file);
    
    while (fgets(line, 1024, file) && *productCount < MAX_PRODUCTS) {
        Product* p = &products[*productCount];
        sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d,%[^,\n]",
               &p->id, p->name, p->description,
               &p->price, &p->stock, &p->minimum_threshold, p->category);
        (*productCount)++;
    }
    
    fclose(file);
}

void saveProducts(Product* products, int productCount) {
    FILE* file = fopen("products.csv", "w");
    if (file == NULL) {
        printf("Error opening products file for writing\n");
        return;
    }
    
    fprintf(file, "id,name,description,price,stock,minimum_threshold,category\n");
    
    for (int i = 0; i < productCount; i++) {
        fprintf(file, "%d,%s,%s,%.2f,%d,%d,%s\n",
                products[i].id, products[i].name, products[i].description,
                products[i].price, products[i].stock, products[i].minimum_threshold, products[i].category);
    }
    
    fclose(file);
}

void displayProducts(Product* products, int productCount) {
    printf("\n==================== Available Products ====================\n");
    printf("ID\tName\t\t\t\t\t\t\tPrice\tStock\tCategory\n");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < productCount; i++) {
        printf("%d\t%-16s\t\t\t\t$%.2f\t%d\t\t%s\n",
               products[i].id, products[i].name, products[i].price,
               products[i].stock, products[i].category);
    }
}
char *toLower(const char *str) {
    char *lowerStr = malloc(strlen(str) + 1);
    for (int i = 0; str[i]; i++) {
        lowerStr[i] = tolower((unsigned char)str[i]);
    }
    lowerStr[strlen(str)] = '\0'; 

    return lowerStr;
}


void searchProducts(Product* products, int productCount, char* searchTerm) {
    printf("\n==================================================================\n");
    printf("ID\t%-30s%-10s%-8s%-10s\n", "Name", "Price", "Stock", "Category");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < productCount; i++) {
        if (strstr(toLower(products[i].name), toLower(searchTerm)) ||
            strstr(toLower(products[i].category), toLower(searchTerm)) || strstr(toLower(products[i].description), toLower(searchTerm))) {
            printf("%-8d%-30s$%-9.2f%-8d%-10s\n",
                   products[i].id, products[i].name, products[i].price,
                   products[i].stock, products[i].category);
        }
    }
}

void filterByPrice(Product* products, int productCount, double minPrice, double maxPrice) {
    printf("\n=== Products within price range $%.2f - $%.2f ===\n", minPrice, maxPrice);
    printf("ID\t%-30s%-10s%-8s%-10s\n", "Name", "Price", "Stock", "Category");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < productCount; i++) {
        if (products[i].price >= minPrice && products[i].price <= maxPrice) {
            printf("%-8d%-30s$%-9.2f%-8d%-10s\n",
                   products[i].id, products[i].name, products[i].price,
                   products[i].stock, products[i].category);
        }
    }
}