// owner.c
#include "owner.h"
#include <errno.h>
#include <string.h>


// ฟังก์ชันสร้างสินค้าใหม่
void createProduct(void) {
    Product newProduct;
    FILE *file = fopen("products.csv", "a+");
    if (file == NULL) {
        printf("Cannot open file: %s\n", strerror(errno));
        return;
    }
    
    newProduct.id = getLastProductId() + 1;
    
    printf("\n=== Add New Product ===\n");
    
    printf("Product Name: ");
    scanf(" %[^\n]", newProduct.name);
    
    printf("Product Description: ");
    scanf(" %[^\n]", newProduct.description);
    
    printf("Price: ");
    scanf("%lf", &newProduct.price);
    while (newProduct.price <= 0) {
        printf("Please enter a price greater than 0: ");
        scanf("%lf", &newProduct.price);
    }
    
    printf("Stock Quantity: ");
    scanf("%d", &newProduct.stock);
    while (newProduct.stock < 0) {
        printf("Please enter a non-negative quantity: ");
        scanf("%d", &newProduct.stock);
    }
    
    printf("Minimum Stock Threshold: ");
    scanf("%d", &newProduct.minimum_threshold);
    while (newProduct.minimum_threshold < 0) {
        printf("Please enter a non-negative quantity: ");
        scanf("%d", &newProduct.minimum_threshold);
    }
    
    printf("Product Category: ");
    scanf(" %[^\n]", newProduct.category);
    
    if (fprintf(file, "%d,%s,%s,%.2f,%d,%d,%s\n",
            newProduct.id,
            newProduct.name,
            newProduct.description, 
            newProduct.price,
            newProduct.stock,
            newProduct.minimum_threshold,
            newProduct.category) < 0) {
        printf("Error writing to file\n");
        fclose(file);
        return;
    }
    
    fflush(file);
    fclose(file);
    
    char logMessage[100];
    sprintf(logMessage, "Added new product: %s", newProduct.name);
    logAction(logMessage);
    
    printf("\nProduct added successfully\n");
    printf("Your product ID: %d\n", newProduct.id);
}


// ฟังก์ชันดูข้อมูลสินค้า
void viewProduct(void) {
    FILE *file = fopen("products.csv", "r");  // แก้ไขชื่อไฟล์ให้ตรงกัน
    char line[500];
    
    if (file == NULL) {
        printf("Product not found\n");
        return;
    }
    
    printf("\n=== All Products List ===\n");
    while (fgets(line, sizeof(line), file)) {
        Product product;
        if (sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d",  // แก้ไขการอ่านค่าให้ครบทุกฟิลด์
                   &product.id, product.name, product.description,
                   &product.price, &product.stock, &product.minimum_threshold) != 6) {  // เปลี่ยนเป็นตรวจสอบ 6 ค่า
            printf("Invalid data in line: %s\n", line);
            continue;
        }
        
        printf("\nID: %d\n", product.id);
        printf("Name: %s\n", product.name);
        printf("Description: %s\n", product.description);
        printf("Price: %.2f Baht\n", product.price);
        printf("Stock: %d pieces\n", product.stock);
        printf("Minimum Threshold: %d pieces\n", product.minimum_threshold);  // เพิ่มการแสดงค่า minimum_threshold
        printf("-------------------------\n");
    }
    
    fclose(file);
}

// ฟังก์ชันบันทึกการกระทำ
void logAction(const char* action) {
    FILE *logFile = fopen("activity_log.txt", "a");
    time_t now = time(NULL);
    char timestamp[26];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(logFile, "[%s] %s\n", timestamp, action);
    fclose(logFile);
}

// ฟังก์ชันแก้ไขข้อมูลสินค้า
void updateProduct(void) {
    int targetId;
    const char* originalFile = "products.csv";
    const char* tempFile = "temp_update";
    
    FILE *file = fopen(originalFile, "r");
    FILE *temp = fopen(tempFile, "w");
    Product product;
    char line[500];
    int found = 0;
    
    if (file == NULL || temp == NULL) {
        printf("Cannot open file!\n");
        return;
    }
    
    printf("Enter product ID to edit: ");
    scanf("%d", &targetId);
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d,%[^,]",
               &product.id, product.name, product.description,
               &product.price, &product.stock, &product.minimum_threshold, product.category);
        
        if (product.id == targetId) {
            found = 1;
            printf("\nCurrent Product Details:\n");
            printf("Name: %s\n", product.name);
            printf("Description: %s\n", product.description);
            printf("Price: %.2f\n", product.price);
            printf("Stock: %d\n", product.stock);
            printf("Minimum: %d\n", product.minimum_threshold);
            
            printf("\nEnter new details:\n");
            printf("Product Name: ");
            scanf(" %[^\n]s", product.name);
            printf("Description: ");
            scanf(" %[^\n]s", product.description);
            printf("Price: ");
            scanf("%lf", &product.price);
            printf("Stock Quantity: ");
            scanf("%d", &product.stock);
            printf("Minimum Quantity: ");
            scanf("%d", &product.minimum_threshold);
            scanf(" %[^\n]s", product.category);
            printf("Category: ");
        }
        
        fprintf(temp, "%d,%s,%s,%.2f,%d,%d,%s\n",
                product.id, product.name, product.description,
                product.price, product.stock, product.minimum_threshold, product.category);
    }
    
    fclose(file);
    fclose(temp);
    
    if (found) {
        remove(originalFile);
        rename(tempFile, originalFile);
        logAction("Edit Product Information");
        printf("Edit successful\n");
    } else {
        remove(tempFile);
        printf("Product not found\n");
    }
}

// ฟังก์ชันลบสินค้า
void deleteProduct(void) {
    int targetId;
    // กำหนด path เต็มสำหรับไฟล์
    const char* originalFile = "products.csv";
    const char* tempFile = "delete_products";
    
    FILE *file = fopen(originalFile, "r");
    FILE *temp = fopen(tempFile, "w");
    Product product;
    char line[500];
    int found = 0;
    
    if (file == NULL || temp == NULL) {
        printf("Cannot open file!\n");
        return;
    }
    
    printf("Enter product ID to delete: ");
    scanf("%d", &targetId);
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d,%[^,]",
               &product.id, product.name, product.description,
               &product.price, &product.stock, &product.minimum_threshold, product.category);
        
        if (product.id == targetId) {
            found = 1;
            continue;
        }
        
        fprintf(temp, "%d,%s,%s,%.2f,%d,%d,%s",
                product.id, product.name, product.description,
                product.price, product.stock, product.minimum_threshold, product.category);
    }
    
    fclose(file);
    fclose(temp);
    
    if (found) {
        // ใช้ path เต็มในการลบและเปลี่ยนชื่อไฟล์
        remove(originalFile);
        rename(tempFile, originalFile);
        logAction("Delete Product");
        printf("Product deleted successfully\n");
    } else {
        remove(tempFile);
        printf("Product not found\n");
    }
}


// ฟังก์ชันค้นหาสินค้าตามชื่อ
void searchByName(const char* searchTerm) {
    FILE *file = fopen("products.csv", "r");
    Product products[100];  // สมมติว่าเก็บได้สูงสุด 100 รายการ
    int count = 0;
    char line[500];
    
    if (file == NULL) {
        printf("Cannot open file!\n");
        return;
    }
    
    printf("\nSearch results for '%s':\n", searchTerm);
    
    while (fgets(line, sizeof(line), file) && count < 100) {
        Product product;
        sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d,%[^,]",
               &product.id, product.name, product.description,
               &product.price, &product.stock, &product.minimum_threshold, product.category);
        
        // ค้นหาคำในชื่อสินค้า (ไม่สนใจตัวพิมพ์เล็ก/ใหญ่)
        char lowerName[100], lowerSearch[100];
        strcpy(lowerName, product.name);
        strcpy(lowerSearch, searchTerm);
        for(int i = 0; lowerName[i]; i++) lowerName[i] = tolower(lowerName[i]);
        for(int i = 0; lowerSearch[i]; i++) lowerSearch[i] = tolower(lowerSearch[i]);
        
        if (strstr(lowerName, lowerSearch)) {
            products[count++] = product;
        }
    }
    
    fclose(file);
}

// ฟังก์ชันค้นหาสินค้าตามช่วงราคา
void searchByPriceRange(float minPrice, float maxPrice) {
    FILE *file = fopen("products.csv", "r");
    Product products[100];
    int count = 0;
    char line[500];
    
    if (file == NULL) {
        printf("Cannot open file!\n");
        return;
    }
    
    printf("\nSearch results for price range %.2f - %.2f Baht:\n", minPrice, maxPrice);
    
    while (fgets(line, sizeof(line), file) && count < 100) {
        Product product;
        sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d,%[^,]",
               &product.id, product.name, product.description,
               &product.price, &product.stock, &product.minimum_threshold, product.category);
        
        if (product.price >= minPrice && product.price <= maxPrice) {
            products[count++] = product;
        }
    }
    
    fclose(file);
    displaySearchResults(products, count);
}

// ฟังก์ชันแสดงผลการค้นหา
void displaySearchResults(Product* products, int count) {
    if (count == 0) {
        printf("No products found matching search criteria\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        printf("\nID: %d\n", products[i].id);
        printf("Name: %s\n", products[i].name);
        printf("Description: %s\n", products[i].description);
        printf("Price: %.2f Baht\n", products[i].price);
        printf("Stock: %d pieces\n", products[i].stock);
        printf("-------------------------\n");
    }
    
    
    printf("Found %d products\n", count);
}

// ฟังก์ชันสร้างรายงานประจำวัน
void generateDailyReport(void) {
    // เปิดไฟล์สำหรับอ่านข้อมูลการขาย
    FILE *salesFile = fopen("daily_sales.csv", "r");
    if (salesFile == NULL) {
        printf("Cannot open daily_sales.csv\n");
        return;
    }

    // เปิดไฟล์สำหรับเขียนรายงานในโหมด append
    FILE *reportFile = fopen("daily_report.csv", "a"); // เปิดโหมด append
    if (reportFile == NULL) {
        printf("Cannot open daily_report.csv\n");
        fclose(salesFile);
        return;
    }

    char line[1024];
    char currentDate[11];
    int totalSales = 0;
    float totalRevenue = 0.0;
    int bestProductId = 0;
    char bestProductName[100] = "";
    int bestProductQuantity = 0;

    // รับวันที่ปัจจุบัน
    time_t now = time(NULL);
    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", localtime(&now));

    // ข้ามบรรทัดแรก (header)
    fgets(line, sizeof(line), salesFile);

    // อ่านข้อมูลการขายทีละบรรทัด
    while (fgets(line, sizeof(line), salesFile)) {
        char date[11];
        int productId;
        char productName[100];
        int quantity;
        float price;

        // แยกข้อมูลจากแต่ละบรรทัด
        if (sscanf(line, "%[^,],%d,%[^,],%d,%f",
                   date, &productId, productName, &quantity, &price) == 5) {
            
            // ตรวจสอบว่าเป็นการขายของวันนี้
            if (strcmp(date, currentDate) == 0) {
                totalSales += quantity;
                totalRevenue += price * quantity;

                // ตรวจสอบสินค้าขายดีที่สุด
                if (quantity > bestProductQuantity) {
                    bestProductId = productId;
                    strcpy(bestProductName, productName);
                    bestProductQuantity = quantity;
                }
            }
        }
    }

    // ถ้าไม่มีข้อมูลขายวันนี้
    if (totalSales == 0) {
        printf("No sales data available for today (%s).\n", currentDate);
        fclose(salesFile);
        fclose(reportFile);
        return;
    }

    // เขียนข้อมูลสรุปลงในรายงาน
    fprintf(reportFile, "%s,%d,%.2f,%s,%d\n",
            currentDate, totalSales, totalRevenue, bestProductName, bestProductQuantity);

    // ปิดไฟล์
    fclose(salesFile);
    fclose(reportFile);

    printf("\n=== Daily Report for %s ===\n", currentDate);
    printf("Total Sales: %d pieces\n", totalSales);
    printf("Total Revenue: %.2f Baht\n", totalRevenue);
    printf("\nBest Selling Product:\n");
    printf("- Product ID: %d\n", bestProductId);
    printf("- Product Name: %s\n", bestProductName);
    printf("- Sales Quantity: %d pieces\n", bestProductQuantity);

    // แสดงข้อมูลจากไฟล์รายงาน
    printf("\nComplete Sales Details:\n");
    FILE *showReport = fopen("daily_report.csv", "r");
    if (showReport != NULL) {
        char reportLine[1024];
        while (fgets(reportLine, sizeof(reportLine), showReport)) {
            printf("%s", reportLine);
        }
        fclose(showReport);
    }

    printf("\nDaily report appended successfully!\n");
}


void showSalesReport(void) {
    FILE *reportFile = fopen("daily_report.csv", "r");
    if (reportFile == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }
    char line[1000];
    
    printf("\n=== Sales Report ===\n");
    printf("Date         | Total Sales | Total Revenue  | Best Selling Product\n");
    printf("--------------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), reportFile)) {

        char productName[1000];  
        char date[1000];        
        int totalSales = 0;     
        float totalRevenue = 0.0; 
        int quantity = 0;       
        
        if (sscanf(line, "%[^,],%d,%f,%[^,],%d",
                date, &totalSales, &totalRevenue, productName, &quantity) == 5) {
            printf("%-12s | %11d | %14.2f | %-20s (%d)\n",
                        date, totalSales, totalRevenue, productName, quantity);
        }
    }
    fclose(reportFile);
}
// ฟังก์ชันแสดงรายงานสต็อกสินค้า
void showStockReport(void) {
    FILE *file = fopen("products.csv", "r");
    char line[500];
    
    printf("\n============== Stock Report ==============\n");
    printf("ID   | Product Name       | Stock | Status\n");
    printf("------------------------------------------\n");
    
    while (fgets(line, sizeof(line), file)) {
        Product product;
        sscanf(line, "%d,%[^,],%*[^,],%*f,%d,%d",
               &product.id, product.name, &product.stock, &product.minimum_threshold);
        
        char status[50] = "Normal";
        if (product.stock <= product.minimum_threshold) {
            snprintf(status, sizeof(status), "Below Threshold");
        }
        
        printf("%-4d | %-12s       | %d  | %s\n",
            product.id,
            product.name,
            product.stock,
            status);
    }
    
    fclose(file);
}

// เพิ่มการทำงานของฟังก์ชันในไฟล์ owner.c
void getProductName(int productId, char* productName) {
    FILE *file = fopen("products.csv", "r");
    char line[500];
    
    if (file == NULL) {
        strcpy(productName, "Data not found");
        return;
    }
    
    while (fgets(line, sizeof(line), file)) {
        Product product;
        sscanf(line, "%d,%[^,],%*[^,],%*f,%*d,%*d",
               &product.id, product.name);
        
        if (product.id == productId) {
            strcpy(productName, product.name);
            fclose(file);
            return;
        }
    }
    
    strcpy(productName, "Data not found");
    fclose(file);
}

// ฟังก์ชันเติมสต็อกสินค้าด้วยตนเอง
void restockProduct(void) {
    int productId, amount;
    Product product;
    
    printf("\n=== Restock Product ===\n");
    printf("Enter Product ID: ");
    scanf("%d", &productId);
    
    // ตรวจสอบว่าสินค้ามีอยู่จริง
    if (!findProduct(productId, &product)) {
        printf("Invalid product ID %d\n", productId);
        return;
    }
    
    printf("Product: %s\n", product.name);
    printf("Current Stock: %d\n", product.stock);
    printf("Enter quantity to restock: ");
    scanf("%d", &amount);
    
    if (amount <= 0) {
        printf("Please enter a quantity greater than 0\n");
        return;
    }
    
    // อัพเดตสต็อก
    int newStock = product.stock + amount;
    if (updateProductStock(productId, newStock)) {
        if (!updateProductStock(productId, newStock)) {
            printf("Error updating stock\n");
            return;
        }
        // บันทึกประวัติการเติมสต็อก
        RestockRecord record = {
            .product_id = productId,
            .current_stock = newStock,
            .restock_amount = amount
        };
        time_t now = time(NULL);
        strftime(record.timestamp, sizeof(record.timestamp),
                "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        logRestockActivity(record);
        printf("Stock updated successfully. New: %d\n", newStock);
    } else {
        printf("Error updating stock!\n");
    }
}


void autoRestockCheck(void) {
    FILE *file = fopen("products.csv", "r+");  
    if (!file) {
        printf("Error: Could not open products.csv\n");
        return;
    }

    Product tempProducts[MAX_PRODUCTS]; 
    int productCount = 0;

    while (fscanf(file, "%d,%99[^,],%199[^,],%lf,%d,%d,%99[^\n]",
                  &tempProducts[productCount].id,
                  tempProducts[productCount].name,
                  tempProducts[productCount].description,
                  &tempProducts[productCount].price,
                  &tempProducts[productCount].stock,
                  &tempProducts[productCount].minimum_threshold,
                  tempProducts[productCount].category) == 7) {
        productCount++;
        if (productCount >= MAX_PRODUCTS) break; 
    }

    for (int i = 0; i < productCount; i++) {
        if (tempProducts[i].stock <= tempProducts[i].minimum_threshold) {
            int restockAmount = (tempProducts[i].minimum_threshold * 2) - tempProducts[i].stock;
            tempProducts[i].stock += restockAmount;

            RestockRecord record;
            record.product_id = tempProducts[i].id;
            record.current_stock = tempProducts[i].stock;
            record.restock_amount = restockAmount;

            time_t now = time(NULL);
            strftime(record.timestamp, sizeof(record.timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

            logRestockActivity(record);

            // Log and print the restock details
            printf("Automatic restock: %s, Restocked: %d, New Stock: %d\n",
                   tempProducts[i].name, restockAmount, tempProducts[i].stock);
            // printf("Restock recorded for product ID: %d, Timestamp: %s\n", 
            //        record.product_id, record.timestamp);
        }
    }

    freopen("products.csv", "w", file);
    for (int i = 0; i < productCount; i++) {
        fprintf(file, "%d,%s,%s,%.2f,%d,%d,%s\n",
                tempProducts[i].id,
                tempProducts[i].name,
                tempProducts[i].description,
                tempProducts[i].price,
                tempProducts[i].stock,
                tempProducts[i].minimum_threshold,
                tempProducts[i].category);
    }

    fclose(file);  
}



// ฟังก์ชันบันทึกประวัติการเติมสต็อก
void logRestockActivity(RestockRecord record) {
    FILE *logFile = fopen("restock_history.txt", "a");
    fprintf(logFile, "%s,%d,%d,%d\n",
            record.timestamp,
            record.product_id,
            record.restock_amount,
            record.current_stock);
    fclose(logFile);
}

// ฟังก์ชันอัพเดตสต็อกสินค้า
int updateProductStock(int productId, int newStock) {
    const char* originalPath = "products.csv";
    const char* tempPath = "temp.csv";
    
    FILE *file = fopen(originalPath, "r");
    if (!file) {
        printf("Error: Cannot open products file\n");
        return 0;
    }
    
    FILE *tempFile = fopen(tempPath, "w");
    if (!tempFile) {
        printf("Error: Cannot create temp file\n");
        fclose(file);
        return 0;
    }
    
    char line[500];
    int success = 0;
    
    while (fgets(line, sizeof(line), file)) {
        Product product;
        sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d",
               &product.id, product.name, product.description,
               &product.price, &product.stock, &product.minimum_threshold, product.category);
        
        if (product.id == productId) {
            product.stock = newStock;
            success = 1;
        }
        
        fprintf(tempFile, "%d,%s,%s,%.2f,%d,%d\n",
                product.id, product.name, product.description,
                product.price, product.stock, &product.minimum_threshold, product.category);
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (success) {
        if (remove(originalPath) != 0) {
            printf("Error: Cannot remove original file\n");
            remove(tempPath);
            return 0;
        }
        if (rename(tempPath, originalPath) != 0) {
            printf("Error: Cannot rename temp file\n");
            return 0;
        }
    } else {
        remove(tempPath);
    }
    
    return success;
}

// ฟังก์ชันค้นหาสินค้าจากรหัสสินค้า
int findProduct(int productId, Product* product) {
    FILE *file = fopen("products.csv", "r");
    char line[500];
    int found = 0;
    
    if (file == NULL) {
        return 0;
    }
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%[^,],%[^,],%lf,%d,%d",
               &product->id, product->name, product->description,
               &product->price, &product->stock, &product->minimum_threshold, product->category);
               
        if (product->id == productId) {
            found = 1;
            break;
        }
    }
    
    fclose(file);
    return found;
}

// ฟังก์ชันสร้างคูปองใหม่
void createCoupon(void) {
    FILE *file = fopen("coupons.csv", "a");
    Coupon coupon;
    
    printf("\n=== Create Discount Coupon ===\n");
    printf("Enter coupon code: ");
    scanf(" %[^\n]s", coupon.code);
    
    printf("Enter discount percentage (1-99): ");
    scanf("%f", &coupon.discount_percent);
    
    if (coupon.discount_percent <= 0 || coupon.discount_percent >= 100) {
        printf("Invalid discount percentage\n");
        fclose(file);
        return;
    }
    
    printf("Enter expiry date (YYYY-MM-DD): ");
    scanf(" %[^\n]s", coupon.expiry_date);
    
    coupon.is_active = 1;
    
    fprintf(file, "%s,%.2f,%s,%d\n",
            coupon.code, coupon.discount_percent,
            coupon.expiry_date, coupon.is_active);
    
    fclose(file);
    printf("Coupon created successfully\n");
}

// ฟังก์ชันแสดงรายการคูปองทั้งหมด
void listCoupons(void) {
    FILE *file = fopen("coupons.csv", "r");
    Coupon coupon;
    char line[100];
    
    printf("\n=== All Coupons List ===\n");
    printf("Coupon Code | Discount | Expiry Date | Status\n");
    printf("----------------------------------------\n");
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%f,%[^,],%d",
               coupon.code, &coupon.discount_percent,
               coupon.expiry_date, &coupon.is_active);
        
        printf("%s | %.0f%% | %s | %s\n",
               coupon.code, coupon.discount_percent,
               coupon.expiry_date,
               coupon.is_active ? "Active" : "Inactive");
    }
    
    fclose(file);
}

// ฟังก์ชันตรวจสอบความถูกต้องของคูปอง
int validateCoupon(const char* code, float* discount) {
    FILE *file = fopen("coupons.csv", "r");
    Coupon coupon;
    char line[100];
    int valid = 0;
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%f,%[^,],%d",
               coupon.code, &coupon.discount_percent,
               coupon.expiry_date, &coupon.is_active);
        
        if (strcmp(coupon.code, code) == 0) {
            if (coupon.is_active && !isCouponExpired(coupon.expiry_date)) {
                *discount = coupon.discount_percent;
                valid = 1;
            }
            break;
        }
    }
    
    fclose(file);
    return valid;
}

// ฟังก์ชันตรวจสอบว่าคูปองหมดอายุหรือยัง
int isCouponExpired(const char* expiryDate) {
    time_t now = time(NULL);
    struct tm tm_expiry = {0};
    
    // แปลงวันหมดอายุเป็น timestamp
    sscanf(expiryDate, "%d-%d-%d",
           &tm_expiry.tm_year, &tm_expiry.tm_mon, &tm_expiry.tm_mday);
    tm_expiry.tm_year -= 1900;
    tm_expiry.tm_mon -= 1;
    
    time_t expiry = mktime(&tm_expiry);
    
    return difftime(now, expiry) > 0;
}

// ฟังก์ชันยกเลิกคูปอง
void deleteCoupon(const char* code) {
    FILE *file = fopen("coupons.csv", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Coupon coupon;
    char line[100];
    int found = 0;
    
    if (file == NULL || tempFile == NULL) {
        printf("Error: Cannot open file!\n");
        return;
    }
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%f,%[^,],%d",
               coupon.code, &coupon.discount_percent,
               coupon.expiry_date, &coupon.is_active);
        
        // ถ้ารหัสคูปองไม่ตรงกับที่ต้องการลบ จะเก็บไว้ในไฟล์ชั่วคราว
        if (strcmp(coupon.code, code) != 0) {
            fprintf(tempFile, "%s,%.2f,%s,%d\n",
                    coupon.code, coupon.discount_percent,
                    coupon.expiry_date, coupon.is_active);
        } else {
            found = 1; // พบคูปองที่ต้องการลบ
        }
    }
    
    fclose(file);
    fclose(tempFile);

    // ลบไฟล์เดิมและเปลี่ยนชื่อไฟล์ชั่วคราว
    if (remove("coupons.csv") == 0 && rename("temp.txt", "coupons.csv") == 0) {
        if (found) {
            printf("Coupon %s deleted successfully\n", code);
        } else {
            printf("Coupon %s not found\n", code);
        }
    } else {
        printf("Error: File update failed!\n");
    }
}

// ฟังก์ชันอ่านข้อมูลจากไฟล์ CSV
void loadProductsFromCSV(void) {
    const char* CSV_PATH = "products.csv";  // กำหนดชื่อไฟล์เป็นค่าคงที่
    const char* DATA_PATH = "products.txt"; // กำหนดชื่อไฟล์เป้าหมายเป็นค่าคงที่
    
    FILE *csvFile = fopen(CSV_PATH, "r");
    if (csvFile == NULL) {
        perror("Error opening CSV file");
        printf("Cannot open file %s\n", CSV_PATH);
        printf("Please check:\n");
        printf("1. Is file %s in the same folder as the program?\n", CSV_PATH);
        printf("2. Does the program have file access permissions?\n");
        printf("3. Are the filename and extension correct?\n");
        return;
    }

    FILE *dataFile = fopen(DATA_PATH, "w");
    if (dataFile == NULL) {
        perror("Error opening data file");
        printf("Cannot open file %s\n", DATA_PATH);
        fclose(csvFile);  // ปิดไฟล์ CSV ก่อนออกจากฟังก์ชัน
        return;
    }

    char line[500];
    int lineCount = 0;
    int successCount = 0;

    // อ่านทีละบรรทัด
    while (fgets(line, sizeof(line), csvFile)) {
        lineCount++;
        
        // // ข้าม header บรรทัดแรก
        // if (lineCount == 1) {
        //     continue;
        // }

        // ตรวจสอบความถูกต้องของข้อมูล
        if (strlen(line) > 0) {
            // ตัด newline ที่อาจมีอยู่ท้ายบรรทัด
            line[strcspn(line, "\n")] = 0;
            
            // เขียนข้อมูลลงในไฟล์
            fprintf(dataFile, "%s\n", line);
            successCount++;
        }
    }

    fclose(csvFile);
    fclose(dataFile);

    if (successCount > 0) {
        printf("Data loaded successfully: Imported %d items\n", successCount);
    } else {
        printf("No data found in CSV file or import failed\n");
    }
}

// ฟังก์ชันหา ID สินค้าล่าสุด
int getLastProductId(void) {
    FILE *file = fopen("products.csv", "r");
    char line[500];
    int lastId = 0;
    int currentId;
    
    if (file == NULL) {
        return 0;
    }
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,", &currentId);
        if (currentId > lastId) {
            lastId = currentId;
        }
    }
    
    fclose(file);
    return lastId;
}

int findCoupon(const char* couponCode, Coupon* coupon) {
    FILE *file = fopen("coupons.csv", "r");
    if (file == NULL) return 0;

    char line[100];
    char tempCode[50]; 

    while (fgets(line, sizeof(line), file)) {

        if (sscanf(line, "%[^,],%f,%[^,],%d",
                   tempCode,
                   &coupon->discount_percent,
                   coupon->expiry_date,
                   &coupon->is_active) == 4) {
            if (strcmp(tempCode, couponCode) == 0) {
                strcpy(coupon->code, tempCode);
                fclose(file);
                return 1; 
            }
        }
    }

    fclose(file);
    return 0; // No match found
}

// เพิ่มฟังก์ชัน deactivateCoupon
void deactivateCoupon(const char* code) {
    FILE *file = fopen("coupons.csv", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Coupon coupon;
    char line[100];
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%f,%[^,],%d",
               coupon.code, &coupon.discount_percent,
               coupon.expiry_date, &coupon.is_active);
        
        if (strcmp(coupon.code, code) == 0) {
            coupon.is_active = 0;  // ยกเลิกคูปอง
            found = 1;
        }
        
        fprintf(tempFile, "%s,%.2f,%s,%d\n",
                coupon.code, coupon.discount_percent,
                coupon.expiry_date, coupon.is_active);
    }
    
    fclose(file);
    fclose(tempFile);

    if (remove("coupons.csv") == 0 && rename("temp.txt", "coupons.csv") == 0) {
        if (found) {
            printf("Coupon %s deactivated successfully\n", code);
        } else {
            printf("Coupon %s not found\n", code);
        }
    } else {
        printf("Error: File update failed!\n");
    }
}
