#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "account.c"

#define MAX_ACCOUNTS (5)
Account accounts[MAX_ACCOUNTS] = {};
Account* getAccount(int id);
int count = 0;


int main(){

	char line[100];
	char input1[25];
	char input2[25];
	char input3[25];
	char input4[25];

	printf("\n Welcome to our virtual banking simulator. Enter 'help' for options\n\n");

	do{
		fgets(line, sizeof(line), stdin);
		sscanf(line, "%s %s %s %s", input1, input2, input3, input4);
		
		if(input4 == NULL){
			printf("Please enter valid account information\n\n");
			break;
		}
		

		if(strcmp(input1, "new") == 0){ // Format: new 1000 caleb zierenberg
			if(count < MAX_ACCOUNTS){
				int amount = atoi(input2);
				char name[100];
				strcpy(name, input3);
				strcat(name, " ");
				strcat(name, input4);

				printf("New Account! Name: %s, amount: $%i\n\n", name, amount);	
			
				account_init(&accounts[count], count+1, amount, name);
				count += 1;

			} else {
				printf("Max number of accounts reached\n\n");
			}
		} else if(strcmp(input1, "withdraw") == 0){ // Format: withdraw 0 1000
			int id = atoi(input2);
			int amount = atoi(input3);
			Account* acct_p;

			acct_p = getAccount(id);			
			if(acct_p != NULL){			
				if(acct_p->balance < amount){
					printf("Account %i balance is too low. Balance: $%i Withdrawal: $%i Transaction cancelled\n\n", id, acct_p->balance, amount);
				} else {
					account_withdraw(acct_p, amount);
					printf("Withdrawn $%i from %s's account. Remaining Balance: $%i\n\n", amount, acct_p->owner, acct_p->balance);
				}
			} else {
				printf("Valid account not found for id: %i\n\n", id);
			}
			
			
		} else if(strcmp(input1, "deposit") == 0){ // Format: deposit 0 1000
			
			int id = atoi(input2);
			int amount = atoi(input3);
			Account* acct_p;

			acct_p = getAccount(id);
			if(acct_p != NULL){
				account_deposit(acct_p, amount);
				printf("Deposited $%i into %s's account. New Balance: $%i\n\n", amount, acct_p->owner, acct_p->balance);
			} else {
				printf("Valid account not found for id: %i\n\n", id);
			}



		} else if (strcmp(input1, "transfer") == 0){ // Format: transfer 0 1 1000
			int id1 = atoi(input2);
			int id2 = atoi(input3);
			int amount = atoi(input4);

			Account* to_p;
			Account* from_p;

			from_p = getAccount(id1);
			to_p = getAccount(id2);
			
			if(from_p == NULL){
				printf("Account with id: %i not found.  Transfer cancelled\n\n", id1);
			} else {
				if(to_p == NULL){
					printf("Account with id: %i not found. Transfer cancelled\n\n", id2);
				} else {
				
					if(from_p->balance < amount){
						printf("Accoiunt %i balance is too low. Balance: $%i Withdrawal: $%i Transaction cancelled\n\n", id1, from_p->balance, amount);
					} else {
						account_transfer(from_p, to_p, amount);
						printf("Transfered $%i from account id: %i to account id: %i\n", amount, id1, id2);
						printf("Account id: %i, Owner: %s, New balance: $%i\n", id1, from_p->owner, from_p->balance);
						printf("Account id: %i, Owner: %s, New balance: $%i\n\n", id2, to_p->owner, to_p->balance);
					}
				
				}
			}


		} else if (strcmp(input1, "print") == 0){ // Format: print
	       		printf("All Bank accounts:\n");
			int i;
			for(i = 0; i < count; i++){
				account_print(&accounts[i]);
			}
			printf("\n");

		} else if(strcmp(input1, "quit") == 0){
			printf("Goodbye!\n\n");
		} else if(strcmp(input1, "help") == 0){
		       printf("To use this virtual bank refer to the following examples:\n\n");
		       printf("New 1000 Caleb Zierenberg - this creates an account owned by Caleb Zierenberg with a balance of $1000\n");
		       printf("Withdraw 0 1000 - this withdraws $1000 from account with id 0\n");
		       printf("Deposit 0 1000 - this deposits $1000 into account with id 0\n");
		       printf("transfer 0 1 1000 - this transfers $1000 from account id 0 to account id 1\n");
		       printf("print - this displays all currently active accounts\n");
		       printf("quit - this exits the banking program\n\n");
		} else {
			printf("Invalid input\n\n");
		}
	}
	while(strcmp(input1, "quit") != 0);
	

	return 0;
}



Account* getAccount(int id){
		
	int i;
	for(i = 0; i < count; i++){
		if(accounts[i].id == id){
			return &accounts[i];
		}
	}
	return NULL;
}




















