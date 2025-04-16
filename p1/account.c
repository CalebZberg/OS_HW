#include <stdio.h>
#include <string.h>

int account_init(Account *acct_p, int id, int balance, char *owner){
	acct_p->id = id;
	acct_p->balance = balance;
	strcpy(acct_p->owner, owner);
}

int account_deposit(Account *acct_p, int amount){
	acct_p->balance += amount;
}

int account_withdraw(Account *acct_p, int amount){
	acct_p->balance -= amount;
}

int account_transfer(Account *from_p, Account *to_p, int amount){
	from_p->balance -= amount;
	to_p->balance += amount;
}


int account_print(Account *acct_p){
	char name[100];
	strcpy(name, acct_p->owner);
	int id = acct_p->id;
	int balance = acct_p->balance;
	printf("Owner: %s, Id: %i, Balance: $%i\n", name, id, balance);
}


