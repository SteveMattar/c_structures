#include <stdio.h>
#include <string.h>
#include <time.h>

#define CUSTOMERS_SIZE 100
#define ACCOUNTS_SIZE 250
#define CUST_ACCOUNTS_SIZE 5
#define ACCT_DEPOSITS_SIZE 250
#define ACCT_WITHDRAWALS_SIZE 750
#define TRAN_BUFFER_SIZE 40
#define TRUE 1
#define FALSE 0

struct Customer;
struct Account;
struct Transaction;
struct Date;

struct Customer {
    int id;
    char first_name[TRAN_BUFFER_SIZE];
    char last_name[TRAN_BUFFER_SIZE];
    struct Account *accounts;
    int accounts_len;
};

struct Account {
    int id;
    struct Customer *pCustomer;
    float balance;
    struct Transaction *deposits;
    struct Transaction *withdrawals;
    struct Transaction *transactions;
    int deposits_len;
    int withdrawals_len;
    int transactions_len;
};

struct Date {
    int day;
    int month;
    int year;
};

struct Transaction {
    struct Date date;
    float amount;
    char str[TRAN_BUFFER_SIZE];
};

struct Customer newCustomer(int, char[], char[]);

struct Account newAccount(struct Customer *, int);

struct Transaction newTransaction(float, char[]);

int deposit(struct Account *, struct Transaction);

int withdraw(struct Account *, struct Transaction);

void listCustomerAccounts(struct Customer[], int);

void generateReport(struct Account[], int, struct Date, struct Date);

struct Customer *findCustomerById(struct Customer[],int, int);

struct Account *findAccountById(struct Account[], int, int);

struct Date getCurrentDate();

struct Date inputDate();


int main() {
    struct Customer customers[CUSTOMERS_SIZE] = {-1,"\0","\0",NULL,0};
    struct Account accounts[ACCOUNTS_SIZE] = {-1,NULL,0, NULL,NULL,NULL,0,0,0};
    int customersLength = 0, accountsLength = 0;

    int choice;
    struct Customer *pCustomer;
    struct Account *pAccount;
    struct Transaction transaction;
    int customer_id;
    char first_name[TRAN_BUFFER_SIZE];
    char last_name[TRAN_BUFFER_SIZE];
    int account_id;
    int amount;
    char str[TRAN_BUFFER_SIZE];
    struct Date from_date;
    struct Date to_date;

    do {
        printf("\nBanking System Menu:\n");
        printf("1) Add a new customer\n");
        printf("2) Create a new account\n");
        printf("3) Deposit amount\n");
        printf("4) Withdraw amount\n");
        printf("5) List all customers\n");
        printf("6) Generate report\n");
        printf("0) Exit\n");
        printf("\nEnter choice(0-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (customersLength < CUSTOMERS_SIZE) {
                    printf("Add a new Customer:\n");
                    printf("ID: ");
                    scanf("%d", &customer_id);
                    fflush(stdin);
                    printf("First Name: ");
                    fgets(first_name, TRAN_BUFFER_SIZE, stdin);
                    first_name[strcspn(first_name, "\n")] = 0;
                    printf("Last Name: ");
                    fgets(last_name, TRAN_BUFFER_SIZE, stdin);
                    last_name[strcspn(last_name, "\n")] = 0;
                    customers[customersLength] = newCustomer(customer_id, first_name, last_name);
                    printf("Customer %d was successfully added.\n", customer_id);
                    customersLength++;
                } else {
                    printf("Error: number of customers has reached the limit %d.", CUSTOMERS_SIZE);
                }
                break;
            case 2:
                if (accountsLength < ACCOUNTS_SIZE) {
                    printf("Create a new account:\n");
                    printf("Enter Customer ID: ");
                    scanf("%d", &customer_id);
                    pCustomer = findCustomerById(customers, customersLength, customer_id);
                    if (pCustomer != NULL) {
                        account_id = accountsLength + 200;
                        accounts[accountsLength] = newAccount(pCustomer, account_id);
                        printf("Account %d was successfully added.\n", account_id);
                        accountsLength++;
                    } else {
                        printf("Error: customer %d could not be found. Account %d creation aborted.\n", customer_id,
                               account_id);
                    }
                } else {
                    printf("Error: number of accounts has reached the limit %d.", ACCOUNTS_SIZE);
                }
                break;
            case 3:
                printf("Account ID: ");
                scanf("%d", &account_id);
                pAccount = findAccountById(accounts, accountsLength, account_id);
                if (pAccount != NULL) {
                    printf("Amount to deposit: ");
                    scanf("%d", &amount);
                    fflush(stdin);
                    printf("Your Name: ");
                    fgets(str, TRAN_BUFFER_SIZE, stdin);
                    str[strcspn(str, "\n")] = 0;
                    transaction = newTransaction(amount, str);
                    if (deposit(pAccount, transaction) == TRUE) {
                        printf("Deposit %d$ to account_id %d is done.\n", amount, account_id);
                    } else {
                        printf("Error: number of deposits has reached the limit %d.\n", ACCT_DEPOSITS_SIZE);
                    }
                } else {
                    printf("Error: Deposit failed. Account %d was not found.\n", account_id);
                }
                break;
            case 4:
                printf("Account ID: ");
                scanf("%d", &account_id);
                pAccount = findAccountById(accounts, accountsLength,account_id);
                if (pAccount != NULL) {
                    printf("Amount to withdraw: ");
                    scanf("%f", &amount);
                    amount*=-1;
                    fflush(stdin);
                    printf("Enter the reason for the withdrawal: ");
                    fgets(str, TRAN_BUFFER_SIZE, stdin);
                    str[strcspn(str, "\n")] = 0;
                    transaction = newTransaction(amount, str);
                    if (withdraw(pAccount, transaction) == TRUE) {
                        printf("Withdraw %d$ to account_id %d is done.\n", amount, account_id);
                    } else {
                        printf("Error: number of withdrawals has reached the limit %d.\n", ACCT_WITHDRAWALS_SIZE);
                    }
                } else {
                    printf("Error: Withdrawal failed. Account %d was not found.\n", account_id);
                }
                break;
            case 5:
                listCustomerAccounts(customers, customersLength);
                break;
            case 6:
                from_date = inputDate();
                to_date = inputDate();
                generateReport(accounts, account_id, from_date, to_date);
                break;
        }
    } while (choice != 0);

    return 0;
}

struct Customer newCustomer(int customer_id, char first_name[], char last_name[]) {
    struct Customer customer;
    struct Account accounts[CUST_ACCOUNTS_SIZE];
    customer.id = customer_id;
    strcpy(customer.first_name, first_name);
    strcpy(customer.last_name, last_name);
    customer.accounts = accounts;
    customer.accounts_len = 0;
    return customer;
}

struct Account newAccount(struct Customer *pCustomer, int account_id) {
    struct Account account;
    struct Transaction deposits[ACCT_DEPOSITS_SIZE];
    struct Transaction withdrawals[ACCT_WITHDRAWALS_SIZE];
    struct Transaction transactions[ACCT_DEPOSITS_SIZE+ACCT_WITHDRAWALS_SIZE];
    account.id = account_id;
    account.balance = 0;
    account.pCustomer = pCustomer;
    account.deposits=deposits;
    account.deposits_len = 0;
    account.withdrawals=withdrawals;
    account.withdrawals_len = 0;
    account.transactions=transactions;
    account.transactions_len = 0;
    *((pCustomer->accounts) + pCustomer->accounts_len) = account;
    pCustomer->accounts_len++;
    return account;
}

struct Transaction newTransaction(float amount, char str[]) {
    struct Transaction transaction;
    transaction.amount = amount;
    strcpy(transaction.str, str);
    transaction.date = getCurrentDate();
    return transaction;
}

int deposit(struct Account *pAccount, struct Transaction transaction) {
    if (pAccount->deposits_len < ACCT_DEPOSITS_SIZE) {
        *((pAccount->deposits) + pAccount->deposits_len) = transaction;
        pAccount->deposits_len++;
        *((pAccount->transactions) + pAccount->transactions_len) = transaction;
        pAccount->transactions_len++;
        pAccount->balance += transaction.amount;
        return TRUE;
    } else {
        return FALSE;
    }
}

int withdraw(struct Account *pAccount, struct Transaction transaction) {
    if (pAccount->withdrawals_len < ACCT_WITHDRAWALS_SIZE) {
        *((pAccount->withdrawals) + pAccount->withdrawals_len) = transaction;
        pAccount->withdrawals_len++;
        *((pAccount->transactions) + pAccount->transactions_len) = transaction;
        pAccount->transactions_len++;
        pAccount->balance += transaction.amount;
        return TRUE;
    } else {
        return FALSE;
    }
}

void listCustomerAccounts(struct Customer customers[], int customers_len) {
    if (customers_len > 0) {
        printf("\n\nCustomer\tName\n");
        for (int i = 0; i < customers_len; i++) {
            if (customers[i].id != 0) {
                printf("%d\t%s %s\n", customers[i].id, customers[i].first_name, customers[i].last_name);
                if (customers->accounts_len > 0) {
                    printf("\t-> Account\tBalance\n");
                    for (int j = 0; j < customers->accounts_len; j++) {
                        printf("\t-> %d\t%.2f\n", (customers->accounts + j)->id, (customers->accounts + j)->balance);
                    }
                } else {
                    printf("\t -> No accounts found.");
                }
            }
        }
    } else {
        printf("\t -> No customers found.");
    }
}

void generateReport(struct Account accounts[], int account_id, struct Date from_date, struct Date to_date) {
    printf("report");
}

struct Customer *findCustomerById(struct Customer customers[], int customers_len, int id) {
    for (int i = 0; i < customers_len; i++) {
        if (customers[i].id == id) {
            return &customers[i];
        }
    }
    return NULL;
}

struct Account *findAccountById(struct Account accounts[], int accounts_len, int id) {
    for (int i = 0; i < accounts_len; i++) {
        if (accounts[i].id == id) {
            return &accounts[i];
        }
    }
    return NULL;
}

struct Date getCurrentDate() {
    struct Date date;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date.year = tm.tm_year + 1900;
    date.month = tm.tm_mon + 1;
    date.day = tm.tm_mday;
    return date;
}

struct Date inputDate() {
    struct Date date;
    printf("Enter year: ");
    scanf("%d", &date.year);
    printf("Enter month: ");
    scanf("%d", &date.month);
    printf("Enter day: ");
    scanf("%d", &date.day);
    return date;
}