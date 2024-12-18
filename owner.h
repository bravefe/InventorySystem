// owner.h
#ifndef OWNER_H
#define OWNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define MAX_NAME 100
#define MAX_DESC 200
#define MAX_PRODUCTS 100
#define MAX_CART 10
#define MAX_COUPON_CODE 20
#define DATE_FORMAT_LENGTH 11

// โครงสร้างข้อมูลสำหรับสินค้า
typedef struct {
    int id;
    char name[MAX_NAME];
    char description[MAX_DESC];
    double price;  // ใช้ double สำหรับเงิน
    int stock;
    int minimum_threshold;
    char category[MAX_NAME];
} Product;

// โครงสร้างข้อมูลสำหรับรายงาน
typedef struct {
    char date[DATE_FORMAT_LENGTH];
    int total_sales;
    float total_revenue;
    struct {
        int product_id;
        char product_name[MAX_NAME];
        int quantity;
    } most_sold_product;
} DailyReport;

// ฟังก์ชันสำหรับคุณสมบัติของเจ้าของร้าน
void viewReport(void);
void createProduct(void);
void viewProduct(void);
void updateProduct(void);
void deleteProduct(void);
void restockProduct(void);
void searchProducts(Product* products, int productCount, char* searchTerm);
void logAction(const char* action);

// เพิ่มประกาศฟังก์ชันสำหรับการค้นหาและกรองสินค้า
void searchByName(const char* searchTerm);
void searchByPriceRange(float minPrice, float maxPrice);
void displaySearchResults(Product* products, int count);

// ประกาศฟังก์ชันสำหรับระบบรายงาน
void generateDailyReport(void);
void showSalesReport(void);
void showStockReport(void);
void showProductPerformance(void);

// เพิ่มประกาศฟังก์ชันในไฟล์ owner.h
void getProductName(int productId, char* productName);

// โครงสร้างข้อมูลสำหรับการเติมสต็อก
typedef struct {
    int product_id;
    int current_stock;
    int restock_amount;
    char timestamp[26];
} RestockRecord;

// ประกาศฟังก์ชันสำหรับระบบเติมสต็อก
void restockProduct(void);
void autoRestockCheck(void);
void logRestockActivity(RestockRecord record);
int updateProductStock(int productId, int newStock);

// ประกาศฟังก์ชันค้นหาสินค้า
int findProduct(int productId, Product* product);

// โครงสร้างข้อมูลสำหรับคูปอง
typedef struct {
    char code[MAX_COUPON_CODE];
    float discount_percent;     // เปลี่ยนจาก discount_percentage
    char expiry_date[DATE_FORMAT_LENGTH];  // เปลี่ยนจาก expiration_date
    int is_active;
} Coupon;

// ประกาศฟังก์ชันสำหรับระบบคูปอง
void createCoupon(void);
void listCoupons(void);
int validateCoupon(const char* code, float* discount);
void deactivateCoupon(const char* code);
int isCouponExpired(const char* expiryDate);

// เพิ่มประกาศฟังก์ชัน
void loadProductsFromCSV(void);

// ประกาศฟังก์ชันสร้างสินค้าใหม่
void createProduct(void);
int getLastProductId(void);  // ฟังก์ชันเสริมสำหรับหา ID สินค้าล่าสุด
void loadProductsFromCSV(void);

typedef struct {
    Product* product;
    int quantity;
} CartItem;

typedef struct {
    CartItem items[MAX_CART];
    int itemCount;
    double total;
    Coupon* applied_coupon;
} ShoppingCart;

// ประกาศฟังก์ชันที่เกี่ยวกับคูปอง
int findCoupon(const char* couponCode, Coupon* coupon);

#endif
