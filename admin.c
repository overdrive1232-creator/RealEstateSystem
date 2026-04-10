# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <time.h>
# include "config.h"

# define USER_FILE "users.csv"
# define PROD_FILE "products.csv"

void admin_menu(struct Users loggedInUser) {
    int choice;
    while(1) {
        system("cls");
        printf("=== ADMIN DASHBOARD ===\n");
        printf("Welcome, %s!\n\n", loggedInUser.name);
        printf("1. Manage Users\n");
        printf("2. Manage Estate\n");
        printf("3. View Global Sales History\n");
        printf("4. Account Settings\n");
        printf("5. View System Audit Logs\n"); 
        printf("6. Logout\n");                 
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        if (choice == 1) {
            system("cls");
            int sub, id;
            struct Users tempUser;
            printf("--- USER MANAGEMENT ---\n1. Add\n2. View\n3. Update\n4. Delete\nBack (0): ");
            scanf("%d", &sub);
            
            if(sub == 1) {
                while(getchar() != '\n');
                printf("Enter Name: "); fgets(tempUser.name, sizeof(tempUser.name), stdin);
                tempUser.name[strcspn(tempUser.name, "\n")] = 0;
                printf("Enter Email: "); scanf("%s", tempUser.email);
                printf("Enter Password: "); scanf("%s", tempUser.password);
                printf("Enter Type (Admin/User): "); scanf("%s", tempUser.type);
                strcpy(tempUser.status, "Inactive");
                addUser(USER_FILE, tempUser);
            } else if(sub == 2) {
                admin_viewUsers(USER_FILE);
            } else if(sub == 3) {
   				admin_viewUsers(USER_FILE);
    			printf("\nEnter ID to Update: "); 
    			scanf("%d", &id);
    			while(getchar() != '\n'); 

   				 printf("New Name: "); 
   				 fgets(tempUser.name, sizeof(tempUser.name), stdin);
   				 tempUser.name[strcspn(tempUser.name, "\n")] = 0;

   				 printf("New Email: "); 
   				 scanf("%s", tempUser.email);
    
   			 	 printf("New Type(Admin/User): "); 
   			 	 scanf("%s", tempUser.type);
    
   			 	 printf("New Status(Active/Inactive): "); 
   				 scanf("%s", tempUser.status);
    
   
  			  while(getchar() != '\n'); 

   			 updateUser(USER_FILE, id, tempUser);
			}else if(sub == 4) {
                admin_viewUsers(USER_FILE);
                printf("\nEnter ID to Delete: "); scanf("%d", &id);
                char confirm;
  			    printf("Are you sure you want to delete User ID %d? (y/n): ", id);
    			scanf(" %c", &confirm); 

    			if(confirm == 'y' || confirm == 'Y') {
      			  deleteUser(USER_FILE, id, loggedInUser.name);
      			  printf("User deleted successfully.\n");
  					  } else {
    		    printf("Deletion cancelled.\n");
   				 }
   
			}
				system("pause");

        } else if (choice == 2) {
        
            int p_sub, p_target;
            struct Products tempP;

            while(1) {
                system("cls");
                printf("--- PROPERTY MANAGEMENT ---\n");
                printf("1. Add Property\n2. View Property List\n3. Update Property\n4. Delete Property\n0. Back\n");
                printf("Choice: ");
                scanf("%d", &p_sub);

                if (p_sub == 0) break;

                switch(p_sub) {
                  case 1: 
   				  			while(getchar() != '\n'); 
  				  			printf("Property Type: ");
  				  			fgets(tempP.p_name, sizeof(tempP.p_name), stdin);
  				  			tempP.p_name[strcspn(tempP.p_name, "\n")] = 0;

  				  			printf("Listing (1-Sale, 2-Rent, 3-Lease, 4-Pend): ");
  				  			int cat; 
  				  			scanf("%d", &cat);

   				 				if(cat == 4) {
        
      			  					strcpy(tempP.category, "Pending");
     			  					tempP.price = 0.0;   
       			 					tempP.stock = 0;     
       			 					printf("-> Listing set to Pending. Price and Status automated.\n");
    						} else {
        
       					 			if(cat == 1) strcpy(tempP.category, "Sale");
       					 			else if(cat == 2) strcpy(tempP.category, "Rent");
       					 			else strcpy(tempP.category, "Lease");

      					 printf("Price: "); 
       					 scanf("%f", &tempP.price);
       					 printf("Avail (1-Avail, 2-Sold): "); 
       					 scanf("%d", &tempP.stock);
   	 					}

   
    					time_t t = time(NULL);
    					struct tm *tm = localtime(&t);
    					sprintf(tempP.date_added, "%04d-%02d-%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);

    					addProduct(PROD_FILE, tempP);
    					break;

                    case 2: 
                        viewProducts(PROD_FILE);
                        break;

                    case 3: 
                        viewProducts(PROD_FILE);
                        printf("\nEnter Property ID to Update: ");
                        scanf("%d", &p_target);
                        while(getchar() != '\n');
                        printf("New Property Type: ");
                        fgets(tempP.p_name, sizeof(tempP.p_name), stdin);
                        tempP.p_name[strcspn(tempP.p_name, "\n")] = 0;
                        printf("New Listing (1-Sale, 2-Rent, 3-Lease): ");
                        int ucat; scanf("%d", &ucat);
                         printf("New Price: "); scanf("%f", &tempP.price);
                        if(ucat==1) strcpy(tempP.category, "Sale");
                        else if(ucat==2) strcpy(tempP.category, "Rent");
                        else strcpy(tempP.category, "Lease");
                        printf("New Avail (1-Avail, 2-Sold): "); scanf("%d", &tempP.stock);
                        time_t t2 = time(NULL);
   						time_t now = time(NULL);
    					struct tm *ts = localtime(&now);
    
  
    					sprintf(tempP.date_added, "%04d-%02d-%02d", 
            			ts->tm_year + 1900, 
            			ts->tm_mon + 1, 
            			ts->tm_mday);

    					updateProduct(PROD_FILE, p_target, tempP);
    					break;

                    case 4: // DELETE
                        viewProducts(PROD_FILE);
                        printf("\nEnter Property ID to Delete: ");
                        scanf("%d", &p_target);
                        char confirm;
                        printf("Are you sure? (y/n): ");
                        scanf(" %c", &confirm);
                        if(confirm == 'y' || confirm == 'Y') {
                            deleteProduct(PROD_FILE, p_target, loggedInUser.name);
                        }
                        break;
               			 		}
                				system("pause");
           					 }

        				} else if (choice == 3) {
            				admin_viewAllSales();
        				} else if (choice == 4) {
            				system("cls");
            				printf("=== MY ACCOUNT DETAILS ===\n");
            				printf("Name: %s\nEmail: %s\nRole: %s\n", loggedInUser.name, loggedInUser.email, loggedInUser.type);
            				system("pause");
        				} else if (choice == 5) {
  						  admin_viewAuditLog(); 
						} else if (choice == 6) {
    					// --- LOGOUT IS NOW 6 ---
					    break; 
					}
    		}
}
