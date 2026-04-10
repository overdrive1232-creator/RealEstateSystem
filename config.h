#ifndef CONFIG_H
#define CONFIG_H
#define ORDER_FILE "orders.txt"

struct Users {
    int u_id;
    char name[100];
    char email[100];
    char password[100];
    char type[20];
    char status[20];
    char reg_date[20];
};

struct Products {
    int p_id;
    char p_name[100];
    char category[50];
    float price;
    int stock;
    char date_added[20];
};
struct Orders {
    int order_id;
    int user_id;
    int product_id;
    int quantity;
    float total_price;
    char order_date[20];
};

//ADMIN PROTOTYPES

int authenticate(const char *filename, char email[], char password[], struct Users *loggedInUser);
int getNextID(const char *filename);
void addUser(const char *filename, struct Users u);
void updateUser(const char *filename, int target_id, struct Users newData);
void deleteUser(const char *filename, int target_id, const char *adminName);
void admin_viewUsers(const char *filename);
void admin_menu(struct Users loggedInUser);
void getNameByID(int id, char *targetName);
void getPropByID(int id, char *targetProp);
void admin_viewAllSales();
void writeAuditLog(const char *action, const char *details, const char *performedBy);
//USER PROTOTYPES
void user_menu(struct Users loggedInUser);


//PRODUCT PROTOTYPES

void addProduct(const char *filename, struct Products p);
void viewProducts(const char *filename);
void updateProduct(const char *filename, int target_id, struct Products newData);

#endif
