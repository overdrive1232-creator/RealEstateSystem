# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "config.h"
#include <time.h>

# define ADMINCODE 1234
# define USER_FILE "users.csv"

int main() {
	srand(time(NULL));
    struct Users u;
    int choice, code;
    
    while(1) {
        system("cls");
        printf("=== REAL ESTATE SYSTEM 2026 ===");
        printf("\n1. Login\n2. Sign-up\n3. Exit");
        printf("\nEnter choice: ");
        
       
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1:
                printf("\n--- Login ---");
                printf("\nEnter Email: ");
                scanf("%s", u.email);
                printf("Enter Password: ");
                scanf("%s", u.password);
        
                
                if (authenticate(USER_FILE, u.email, u.password, &u)) {
                    printf("\nLogin successful! Welcome %s\n", u.name);
                    system("pause");
            
                    if (strcmp(u.type, "Admin") == 0) {
                        admin_menu(u);
                    } else if(strcmp(u.type, "User") == 0) {
                        user_menu(u);
                    }
                } else {
                    printf("\nInvalid credentials or account inactive!\n");
                    system("pause");
                }
            break;

            case 2:
                printf("\n--- Sign-up ---");
                printf("\nEnter Admin Registration Code: ");
                scanf("%d", &code);
                                            
                if(code != ADMINCODE) {
                    printf("\nINVALID CODE! Registration aborted.\n");
                    system("pause");
                    break;
                }
                
                
                while(getchar() != '\n');

                printf("Enter Name: ");
                fgets(u.name, sizeof(u.name), stdin);
                u.name[strcspn(u.name, "\n")] = 0;

                printf("Enter Email: ");
                scanf("%s", u.email);
                printf("Enter Password: ");
                scanf("%s", u.password);
                
                
                strcpy(u.type, "User");
                strcpy(u.status, "Inactive");
                
                
                addUser(USER_FILE, u);
                system("pause");               
            break;

            case 3:
                printf("\nExiting System 2026. Goodbye!\n");
                exit(0);
                
            default:
                printf("\nInvalid choice. Try again.\n");
                system("pause");
        }
    }
    return 0;
}
