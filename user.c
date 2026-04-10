# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "config.h"

# define PROD_FILE "products.csv"

void user_menu(struct Users loggedInUser) {
    int choice;

    while(1) {
        system("cls");
        printf("=== USER DASHBOARD ===\n");
        printf("Welcome, %s!\n\n", loggedInUser.name);
        printf("1. Buying Properties List \n");
        printf("2. View Properties\n");
        printf("3. History Logs\n");
        printf("4. Account Settings\n");
        printf("5. Logout\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }

        switch(choice) {
            case 1:
                printf("\nOrder Management \n");
             
                manageOrders(loggedInUser);
    			break;

            case 2:
                system("cls");
                printf("--- PROPERTIES ---\n");
                viewProducts(PROD_FILE);
                printf("\n");
                system("pause");
                break;

            case 3:
                printf("\nTransaction history \n");
                viewHistoryLogs(loggedInUser);
                break;

            case 4:
                system("cls");
                printf("=== MY ACCOUNT DETAILS ===\n");
                printf("User ID:  %d\n", loggedInUser.u_id);
                printf("Name:     %s\n", loggedInUser.name);
                printf("Email:    %s\n", loggedInUser.email);
                printf("Status:   %s\n", loggedInUser.status);
                printf("--------------------------\n");
                system("pause");
                break;

            case 5:
                return; // Returns to main.c login loop

            default:
                printf("\nInvalid choice!\n");
                system("pause");
        }
    }
}
