#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "config.h"

#define PROD_FILE "products.csv"
#define USER_FILE "users.csv"



int getNextID(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 1;
    int maxID = 0, currentID;
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,", &currentID) == 1) {
            if (currentID > maxID) maxID = currentID;
        }
    }
    fclose(file);
    return maxID + 1;
}

void getNameByID(int id, char *targetName) {
    FILE *f = fopen(USER_FILE, "r");
    char line[500];
    int curID;
    strcpy(targetName, "N/A"); 
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d,%99[^,]", &curID, targetName) == 2) {
                if (curID == id) break;
            }
        }
        fclose(f);
    }
}

void getPropByID(int id, char *targetProp) {
    FILE *f = fopen(PROD_FILE, "r");
    char line[500];
    int curID;
    strcpy(targetProp, "N/A");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%d,%99[^,]", &curID, targetProp) == 2) {
                if (curID == id) break;
            }
        }
        fclose(f);
    }
}


void addUser(const char *filename, struct Users u) {
    u.u_id = getNextID(filename);
    
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
 
    sprintf(u.reg_date, "%04d-%02d-%02d", 
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    
    FILE *file = fopen(filename, "a");
    if (file == NULL) return;

    fprintf(file, "%d,%s,%s,%s,%s,%s,%s\n", 
            u.u_id, u.name, u.email, u.password, u.type, u.status, u.reg_date);
    fclose(file);
    char info[100];
	sprintf(info, "New User Created: ID %d, Name: %s", u.u_id, u.name);
	writeAuditLog("USER_ADDED", info, u.name);
    
    printf("\nUSER ADDED! ID: %d | Date: %s", u.u_id, u.reg_date);
}
int authenticate(const char *filename, char email[], char password[], struct Users *loggedInUser) {
    FILE *file = fopen(filename, "r");
    struct Users u;
    char line[500];
    if (file == NULL) return 0;
    while (fgets(line, sizeof(line), file)) {
        
        int fields = sscanf(line, "%d,%99[^,],%99[^,],%99[^,],%19[^,],%19[^,],%19s",
                            &u.u_id, u.name, u.email, u.password, u.type, u.status, u.reg_date);
        if (fields >= 6 && strcmp(u.email, email) == 0 && strcmp(u.password, password) == 0) {
            if (strcmp(u.status, "Active") == 0) {
                *loggedInUser = u;
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}


void admin_viewAllSales() {
    system("cls");
    printf("=== GLOBAL SALES HISTORY ===\n\n");
    
    FILE *fo = fopen(ORDER_FILE, "r");
    if (!fo) { 
        printf("No records found.\n"); 
        system("pause"); 
        return; 
    }

    struct Orders o[500]; 
    int count = 0;
    char userName[100], propName[100];

   
    while (fscanf(fo, "%d %d %d %d %f %s", 
                  &o[count].order_id, &o[count].user_id, &o[count].product_id, 
                  &o[count].quantity, &o[count].total_price, o[count].order_date) == 6) {
        count++;
        if (count >= 500) break; 
    }
    fclose(fo);

    if (count == 0) {
        printf("No records found.\n");
        system("pause");
        return;
    }

    int i, j;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            
            if (o[j].order_id > o[j+1].order_id) {
                struct Orders temp = o[j];
                o[j] = o[j+1];
                o[j+1] = temp;
            }
        }
    }

    printf("| %-8s | %-12s | %-15s | %-10s | %-12s |\n", 
           "ORD ID", "DATE", "BUYER", "PROPERTY", "AMOUNT");
    printf("|----------|--------------|-----------------|------------|------------|\n");

    for (i = 0; i < count; i++) {
        getNameByID(o[i].user_id, userName);
        getPropByID(o[i].product_id, propName);
        
        printf("| %-8d | %-12s | %-15s | %-10s | $%-9.2f |\n", 
               o[i].order_id, o[i].order_date, userName, propName, o[i].total_price);
    }

    printf("\nTotal Sales: %d\n", count);
    system("pause");
}

void admin_viewUsers(const char *filename) {
    system("cls");
    FILE *file = fopen(filename, "r");
    struct Users u;
    char line[500];
    if (!file) return;

    printf("=== USER MANAGEMENT LIST ===\n\n");
    printf("| %-5s | %-15s | %-20s | %-8s | %-10s | %-12s |\n", "ID", "NAME", "EMAIL", "TYPE", "STATUS", "REG DATE");
    printf("|-------|-----------------|----------------------|----------|------------|--------------|\n");

  while (fgets(line, sizeof(line), file)) {
  
    if (sscanf(line, "%d,%99[^,],%99[^,],%99[^,],%19[^,],%19[^,],%19[^\n]", 
               &u.u_id, u.name, u.email, u.password, u.type, u.status, u.reg_date) >= 6) {
  
        printf("| %-5d | %-15s | %-20s | %-8s | %-10s | %-12s |\n", 
               u.u_id, u.name, u.email, u.type, u.status, u.reg_date);
    }
}
    fclose(file);
}


void addProduct(const char *filename, struct Products p) {
    p.p_id = getNextID(filename);

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(p.date_added, "%04d-%02d-%02d", 
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

    FILE *file = fopen(filename, "a");
    if (!file) return;

    fprintf(file, "%d,%s,%s,%.2f,%d,%s\n", 
            p.p_id, p.p_name, p.category, p.price, p.stock, p.date_added);
    fclose(file);
    
    printf("\nPROPERTY ADDED! ID: %d | Date: %s", p.p_id, p.date_added);
}

void viewProducts(const char *filename) {
    FILE *file = fopen(filename, "r");
    struct Products p;
    char line[500];
    if (!file) return;

    printf("\n| %-5s | %-20s | %-15s | %-10s | %-10s | %-12s |\n", 
           "ID", "PROPERTY TYPE", "LISTING STATUS", "PRICE", "STATUS", "DATE ADDED");
    printf("|-------|----------------------|-----------------|------------|------------|--------------|\n");

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d,%99[^,],%49[^,],%f,%d,%19[^\n]", 
                   &p.p_id, p.p_name, p.category, &p.price, &p.stock, p.date_added) == 6) {
            
            char priceDisp[15];
            char *statusTxt = (p.stock == 1) ? "Available" : (p.stock == 2) ? "Sold" : "N/A";

            if (strcmp(p.category, "Pending") == 0) {
                strcpy(priceDisp, "Pending");
                statusTxt = "N/A";
            } else {
                sprintf(priceDisp, "%.2f", p.price);
            }

            printf("| %-5d | %-20s | %-15s | %-10s | %-10s | %-12s |\n", 
                   p.p_id, p.p_name, p.category, priceDisp, statusTxt, p.date_added);
        }
    }
    fclose(file);
}


void manageOrders(struct Users loggedInUser) {
    system("cls");
    printf("=== PROPERTY LISTINGS ===\n\n");

    struct Products p[100];
    int count = 0;
    char line[500];
    FILE *fp = fopen(PROD_FILE, "r");
    if (!fp) return;

    printf("| %-5s | %-20s | %-15s | %-10s | %-10s | %-12s |\n", 
           "ID", "PROPERTY TYPE", "LISTING STATUS", "PRICE", "STATUS", "DATE ADDED");
    printf("|-------|----------------------|-----------------|------------|------------|--------------|\n");

    while (fgets(line, sizeof(line), fp) && count < 100) {
        if (sscanf(line, "%d,%99[^,],%49[^,],%f,%d,%19[^,\n]", 
                   &p[count].p_id, p[count].p_name, p[count].category, 
                   &p[count].price, &p[count].stock, p[count].date_added) == 6) {
            
            char priceDisplay[15];
            char *statusText;

            if (strcmp(p[count].category, "Pending") == 0) {
                strcpy(priceDisplay, "Pending");
                statusText = "N/A";
            } else {
                sprintf(priceDisplay, "%.2f", p[count].price);
                statusText = (p[count].stock == 1) ? "Available" : (p[count].stock == 2) ? "Sold" : "N/A";
            }

            printf("| %-5d | %-20s | %-15s | %-10s | %-10s | %-12s |\n", 
                   p[count].p_id, p[count].p_name, p[count].category, 
                   priceDisplay, statusText, p[count].date_added);
            count++;
        }
    }
    fclose(fp);

    int productId;
    printf("\nEnter Property ID to Buy: ");
    scanf("%d", &productId);

    int found = -1, i;
    for (i = 0; i < count; i++) {
        if (p[i].p_id == productId && p[i].stock == 1) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\nError: This property is either Sold, Pending, or the ID is invalid.\n");
    } else {
        char confirm;
        printf("\n--- PURCHASE CONFIRMATION ---");
        printf("\nBuying: %s", p[found].p_name);
        printf("\nPrice:  $%.2f", p[found].price);
        printf("\nAre you sure? (y/n): ");
        
        while(getchar() != '\n'); 
        scanf("%c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            p[found].stock = 2;
            fp = fopen(PROD_FILE, "w");
            int j;
            for (j = 0; j < count; j++) {
                fprintf(fp, "%d,%s,%s,%.2f,%d,%s\n", 
                        p[j].p_id, p[j].p_name, p[j].category, 
                        p[j].price, p[j].stock, p[j].date_added);
            }
            fclose(fp);

           FILE *fo = fopen(ORDER_FILE, "a");
if (fo) {
    int randomID = (rand() % 900) + 100;

    char dateStr[20];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(dateStr, "%04d-%02d-%02d", 
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
                fprintf(fo, "%d %d %d %d %.2f %s\n", 
        randomID, loggedInUser.u_id, p[found].p_id, 1, p[found].price, dateStr);
				 fclose(fo);
                printf("\n Order #%d saved.", randomID);
            }

            printf("\n => SUCCESS! The property is now SOLD.\n");
        } else {
            printf("\nPurchase cancelled.\n");
        }
    }
    system("pause");
}
void viewHistoryLogs(struct Users loggedInUser) {
    system("cls");
    printf("=== YOUR PURCHASE HISTORY ===\n\n");
    
    FILE *fo = fopen("orders.txt", "r");
    if (!fo) { 
        printf("No history found.\n"); 
        system("pause"); 
        return; 
    }

    struct Orders o;
    int found = 0; 

   
    while (fscanf(fo, "%d %d %d %d %f %s", 
                  &o.order_id, &o.user_id, &o.product_id, 
                  &o.quantity, &o.total_price, o.order_date) == 6) {
        
        if (o.user_id == loggedInUser.u_id) {
            printf("Date: %s | Order #%-5d | Product ID: %-3d | Total: $%.2f\n", 
                   o.order_date, o.order_id, o.product_id, o.total_price);
            found = 1;
        }
    }

    if (found == 0) { 
        printf("No orders found for your account.\n");
    }

    fclose(fo);
    printf("\n");
    system("pause");
}
void updateUser(const char *filename, int target_id, struct Users newData) {
    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("temp.csv", "w");
    char line[500];
    int current_id;
    struct Users oldData;

    if (!file || !temp) return;

    while (fgets(line, sizeof(line), file)) {
     
        if (sscanf(line, "%d,%99[^,],%99[^,],%99[^,],%19[^,],%19[^,],%19[^\n]", 
                   &current_id, oldData.name, oldData.email, oldData.password, 
                   oldData.type, oldData.status, oldData.reg_date) >= 6) {
            
            if (current_id == target_id) {
            
                fprintf(temp, "%d,%s,%s,%s,%s,%s,%s\n", 
                        target_id, newData.name, newData.email, 
                        oldData.password, 
                        newData.type, newData.status, 
                        oldData.reg_date);
                printf("\nUpdate Successful!");
            } else {
                fprintf(temp, "%s", line);
            }
        }
    }
    fclose(file); 
    fclose(temp);
    remove(filename);
    rename("temp.csv", filename);
}


void deleteUser(const char *filename, int target_id, const char *adminName) {
    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("temp.csv", "w");
    char line[500];
    int found = 0, current_id;

    if (!file || !temp) return;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,", &current_id);
        if (current_id == target_id) {
            found = 1;
        } else {
            fprintf(temp, "%s", line);
        }
    }
    fclose(file); 
    fclose(temp);
    remove(filename);
    rename("temp.csv", filename);

   if (found) {
        char info[100];
        snprintf(info, sizeof(info), "Property Deleted: ID %d", target_id);
        writeAuditLog("PROP_DELETED", info, adminName);
        printf("\nProperty ID %d deleted successfully.\n", target_id);
    }
}

void updateProduct(const char *filename, int target_id, struct Products newData) {
    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("temp_p.csv", "w");
    char line[500];
    int current_id, found = 0;

    if (!file || !temp) {
    	if (file) fclose(file);
        if (temp) fclose(temp);
        printf("Error: Could not open files.\n");
        return;
    }
    
    if (fgets(line, sizeof(line), file)) {
        fprintf(temp, "%s", line);
    }
    
   while (fgets(line, sizeof(line), file)) {
        // Use %d, to ensure we catch the ID before the first comma
        if (sscanf(line, "%d,", &current_id) == 1 && current_id == target_id) {
            // Write the NEW data
            fprintf(temp, "%d,%s,%s,%.2f,%d,%s\n", 
                    target_id, newData.p_name, newData.category, 
                    newData.price, newData.stock, newData.date_added);
            found = 1;
        } else {
            // Write the OLD line exactly as it was
            fprintf(temp, "%s", line);
        }
    }
    fclose(file); 
    fclose(temp);

  
    remove(filename);
    if (rename("temp_p.csv", filename) != 0) {
        printf("Error: Could not update the database file. Check if it's open elsewhere.\n");
    }

    if(found) 
        printf("\nProperty ID %d updated successfully!\n", target_id);
    else 
        printf("\nProperty ID %d not found.\n", target_id);
}
void deleteProduct(char *filename, int id, const char *adminName) { 
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp.csv", "w");
    char line[300];
    int current_id;
    int found = 0;

    if (!fp || !temp) return;

    while (fgets(line, sizeof(line), fp)) {
     	   if (sscanf(line, "%d,", &current_id) == 1) {
            if (current_id == id) {
                found = 1; 
            } else {
                fprintf(temp, "%s", line);
            }
        }
    }
    fclose(fp);
    fclose(temp);

    remove(filename);
    rename("temp.csv", filename);

    if (found) {
       
        char info[100];
        snprintf(info, sizeof(info), "Property Deleted: ID %d", id);
        writeAuditLog("PROP_DELETED", info, adminName); 
        // -----------------------------
        printf("\nProperty ID %d deleted successfully.\n", id);
    } else {
        printf("\nProperty ID %d not found.\n", id);
    }
}
void writeAuditLog(const char *action, const char *details, const char *performedBy) {
    FILE *fa = fopen("audit_log.txt", "a");
    if (fa == NULL) return;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char dateStr[25];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", tm);

    fprintf(fa, "[%s] [%-10s] %-15s | %s\n", dateStr, performedBy, action, details);
    fclose(fa);
}
void admin_viewAuditLog() {
    system("cls");
    printf("=== SYSTEM AUDIT LOG ===\n\n");
    FILE *fa = fopen("audit_log.txt", "r");
    if (!fa) {
        printf("No logs found.\n");
        system("pause");
        return;
    }
    char line[500];
    while (fgets(line, sizeof(line), fa)) {
        printf("%s", line);
    }
    fclose(fa);
    printf("\n");
    system("pause");
}
