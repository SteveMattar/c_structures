#include <stdio.h>
#include <string.h>
#include <time.h>

#define CUSTOMERS_SIZE 100
#define ACCOUNTS_SIZE 250
#define ACCT_DEPOSITS_SIZE 250
#define ACCT_WITHDRAWALS_SIZE 250
#define TRAN_BUFFER_SIZE 40
#define TRUE 1
#define FALSE 0

typedef struct Customer Customer;
typedef struct Account Account;
typedef struct Transaction Transaction;
typedef struct Date Date;

struct Customer {
    int id;
    char first_name[TRAN_BUFFER_SIZE];
    char last_name[TRAN_BUFFER_SIZE];
    Account *accounts[ACCT_DEPOSITS_SIZE];
    int accounts_len;
} default_cust = {-1, "\0", "\0", {NULL}, 0};

struct Date {
    int day;
    int month;
    int year;
};

struct Transaction {
    Date date;
    float amount;
    char str[TRAN_BUFFER_SIZE];
} default_tran = {-1, -1, -1, 0, "\0"};

struct Account {
    int id;
    Customer *pCustomer;
    float balance;
    Transaction deposits[ACCT_DEPOSITS_SIZE];
    Transaction withdrawals[ACCT_WITHDRAWALS_SIZE];
    Transaction *transactions[ACCT_DEPOSITS_SIZE + ACCT_WITHDRAWALS_SIZE];
    int deposits_len;
    int withdrawals_len;
    int transactions_len;
};

Customer newCustomer(int, char[], char[]);

Account newAccount(Customer *, int);

Transaction newTransaction(float, char[]);

int deposit(Account *, Transaction);

int withdraw(Account *, Transaction);

void listCustomerAccounts(Customer[], int);

void generateReport(Account[], int, int, Date, Date);

Customer *findCustomerById(Customer[], int, int);

Account *findAccountById(Account[], int, int);

Date getCurrentDate();

Date inputDate();

int dateCmp(Date, Date);

int isDateBetween(Date, Date, Date);


int main() {
    Customer customers[CUSTOMERS_SIZE] = {default_cust};
    Account accounts[ACCOUNTS_SIZE] = {{-1, NULL, 0, {default_tran}, {default_tran}, {NULL}, 0, 0, 0}};
    int customersLength = 0, accountsLength = 0;

    int choice;
    Customer *pCustomer;
    Account *pAccount;
    Transaction transaction;
    int customer_id;
    char first_name[TRAN_BUFFER_SIZE];
    char last_name[TRAN_BUFFER_SIZE];
    int account_id;
    float amount;
    char str[TRAN_BUFFER_SIZE];
    Date from_date;
    Date to_date;

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
        fflush(stdin);

        switch (choice) {
            case 1:
                if (customersLength < CUSTOMERS_SIZE) {
                    printf("Add a new Customer:\n");
                    printf("ID: ");
                    scanf("%d", &customer_id);
                    fflush(stdin);
                    pCustomer = findCustomerById(customers, customersLength, customer_id);
                    if (pCustomer != NULL) {
                        printf("Error: customer %d already exists.\n", customer_id);
                        break;
                    }
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
                    printf("Error: number of customers has reached the limit %d.\n", CUSTOMERS_SIZE);
                }
                break;
            case 2:
                if (accountsLength < ACCOUNTS_SIZE) {
                    printf("Create a new account:\n");
                    printf("Enter Customer ID: ");
                    scanf("%d", &customer_id);
                    fflush(stdin);
                    pCustomer = findCustomerById(customers, customersLength, customer_id);
                    if (pCustomer != NULL) {
                        account_id = accountsLength + 200;
                        accounts[accountsLength] = newAccount(pCustomer, account_id);
                        pCustomer->accounts[pCustomer->accounts_len] = &accounts[accountsLength];
                        pCustomer->accounts_len++;
                        printf("Account %d was successfully added.\n", account_id);
                        accountsLength++;
                    } else {
                        printf("Error: customer %d could not be found. Account %d creation aborted.\n", customer_id,
                               account_id);
                    }
                } else {
                    printf("Error: number of accounts has reached the limit %d.\n", ACCOUNTS_SIZE);
                }
                break;
            case 3:
                printf("Account ID: ");
                scanf("%d", &account_id);
                fflush(stdin);
                pAccount = findAccountById(accounts, accountsLength, account_id);
                if (pAccount != NULL) {
                    printf("Amount to deposit: ");
                    scanf("%f", &amount);
                    fflush(stdin);
                    printf("Your Name: ");
                    fgets(str, TRAN_BUFFER_SIZE, stdin);
                    str[strcspn(str, "\n")] = 0;
                    transaction = newTransaction(amount, str);
                    if (deposit(pAccount, transaction) == TRUE) {
                        printf("Deposit %.2f$ to account_id %d is done.\n", amount, account_id);
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
                fflush(stdin);
                pAccount = findAccountById(accounts, accountsLength, account_id);
                if (pAccount != NULL) {
                    printf("Amount to withdraw: ");
                    scanf("%f", &amount);
                    fflush(stdin);
                    amount *= -1;
                    fflush(stdin);
                    printf("Enter the reason for the withdrawal: ");
                    fgets(str, TRAN_BUFFER_SIZE, stdin);
                    str[strcspn(str, "\n")] = 0;
                    transaction = newTransaction(amount, str);
                    if (withdraw(pAccount, transaction) == TRUE) {
                        printf("Withdraw %.2f$ to account_id %d is done.\n", amount, account_id);
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
                printf("Account ID: ");
                scanf("%d", &account_id);
                fflush(stdin);
                printf("From date\n");
                from_date = inputDate();
                printf("To date\n");
                to_date = inputDate();
                generateReport(accounts, accountsLength, account_id, from_date, to_date);
                break;
        }
    } while (choice != 0);

    return 0;
}

Customer newCustomer(int customer_id, char first_name[], char last_name[]) {
    Customer customer;
    customer.id = customer_id;
    strcpy(customer.first_name, first_name);
    strcpy(customer.last_name, last_name);
    customer.accounts_len = 0;
    return customer;
}

Account newAccount(Customer *pCustomer, int account_id) {
    Account account;
    account.id = account_id;
    account.balance = 0;
    account.pCustomer = pCustomer;
    account.deposits_len = 0;
    account.withdrawals_len = 0;
    account.transactions_len = 0;
    return account;
}

Transaction newTransaction(float amount, char str[]) {
    Transaction transaction;
    transaction.amount = amount;
    strcpy(transaction.str, str);
    transaction.date = getCurrentDate();
    return transaction;
}

int deposit(Account *pAccount, Transaction transaction) {
    if (pAccount->deposits_len < ACCT_DEPOSITS_SIZE) {
        pAccount->deposits[pAccount->deposits_len] = transaction;
        pAccount->transactions[pAccount->transactions_len] = &(pAccount->deposits[pAccount->deposits_len]);
        pAccount->deposits_len++;
        pAccount->transactions_len++;
        pAccount->balance += transaction.amount;
        return TRUE;
    } else {
        return FALSE;
    }
}

int withdraw(Account *pAccount, Transaction transaction) {
    if (pAccount->withdrawals_len < ACCT_WITHDRAWALS_SIZE) {
        pAccount->withdrawals[pAccount->withdrawals_len] = transaction;
        pAccount->transactions[pAccount->transactions_len] = &(pAccount->withdrawals[pAccount->withdrawals_len]);
        pAccount->withdrawals_len++;
        pAccount->transactions_len++;
        pAccount->balance += transaction.amount;
        return TRUE;
    } else {
        return FALSE;
    }
}

void listCustomerAccounts(Customer customers[], int customers_len) {
    char *padding = "----------------------------------------------------------------------------------------------------";
    if (customers_len > 0) {
        printf("\n| %-12s\t| %-20s | %-12s\t| %-10s |\n", "Customer ID", "Customer Name", "Account ID", "Balance");
        printf("%*.*s\n", 0, 70, padding);
        for (int i = 0; i < customers_len; i++) {
            if (customers[i].id != -1) {
                char full_name[TRAN_BUFFER_SIZE];
                strcpy(full_name, customers[i].first_name);
                strcat(full_name, " ");
                strcat(full_name, customers[i].last_name);
                printf("| %-12d\t| %-20s |", customers[i].id, full_name);
                if (customers[i].accounts_len > 0) {
                    for (int j = 0; j < customers[i].accounts_len; j++) {
                        if (j == 0) {
                            printf(" %-12d\t| %-10.2f |\n", customers[i].accounts[j]->id,
                                   customers[i].accounts[j]->balance);
                        } else {
                            printf("| %-12s\t| %-20s | %-12d\t| %-10.2f |\n", "", "", customers[i].accounts[j]->id,
                                   customers[i].accounts[j]->balance);
                        }
                    }
                } else {
                    printf(" %-12s\t| %-10s |\n", "-", "-");
                }
            }
            printf("%*.*s\n", 0, 70, padding);
        }
    } else {
        printf("\nNo customers found.\n");
    }
}

void generateReport(Account accounts[], int accounts_len, int id, Date from_date, Date to_date) {
    char *padding = "----------------------------------------------------------------------------------------------------";
    int padding_len = 55;
    char full_name[TRAN_BUFFER_SIZE];
    printf("\nGenerating Report...\n");
    for (int i = 0; i < accounts_len; i++) {
        if (accounts[i].id == id) {
            strcpy(full_name, (accounts[i].pCustomer)->first_name);
            strcat(full_name, " ");
            strcat(full_name, (accounts[i].pCustomer)->last_name);
            printf("%*.*s\n", 0, padding_len, padding);
            printf("| Report for %-20s\tAccount %-9d |\n", full_name, id);
            printf("%*.*s\n", 0, padding_len, padding);
            if (accounts[i].transactions_len > 0) {
                printf("| %-10s\t| %-15s\t| %-15s |\n", "Date", "Deposit", "Withdraw");
                printf("%*.*s\n", 0, padding_len, padding);
                for (int j = 0; j < accounts[i].transactions_len; j++) {
                    if (isDateBetween(accounts[i].transactions[j]->date, from_date, to_date)) {
                        printf("| %02d/%02d/%-4d\t", accounts[i].transactions[j]->date.day,
                               accounts[i].transactions[j]->date.month, accounts[i].transactions[j]->date.year);
                        if (accounts[i].transactions[j]->amount > 0) {
                            printf("| %-15.2f\t| %-15s |\n", accounts[i].transactions[j]->amount, "-");
                        } else {
                            printf("| %-15s\t| %-15.2f |\n", "-", accounts[i].transactions[j]->amount);
                        }
                    }
                }
            } else {
                printf("-> No transactions found.\n");
            }
            printf("%*.*s\n", 0, padding_len, padding);
            return;
        }
    }
    printf("Error: Account %d was not found.\n", id);
}

Customer *findCustomerById(Customer customers[], int customers_len, int id) {
    for (int i = 0; i < customers_len; i++) {
        if (customers[i].id == id) {
            return &customers[i];
        }
    }
    return NULL;
}

Account *findAccountById(Account accounts[], int accounts_len, int id) {
    for (int i = 0; i < accounts_len; i++) {
        if (accounts[i].id == id) {
            return &accounts[i];
        }
    }
    return NULL;
}

Date getCurrentDate() {
    Date date;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date.year = tm.tm_year + 1900;
    date.month = tm.tm_mon + 1;
    date.day = tm.tm_mday;
    return date;
}

Date inputDate() {
    Date date;
    printf(" -> year: ");
    scanf("%d", &date.year);
    fflush(stdin);
    printf(" -> month: ");
    scanf("%d", &date.month);
    fflush(stdin);
    printf(" -> day: ");
    scanf("%d", &date.day);
    fflush(stdin);
    return date;
}

int dateCmp(Date date1, Date date2) {
    if (date1.year < date2.year) return TRUE; // compare years
    if (date1.year == date2.year && date1.month < date2.month)
        return TRUE; // compare months - but make sure years are equal
    if (date1.year == date2.year && date1.month == date2.month && date1.day < date2.day)
        return TRUE; // compare days but make sure years and months are equal
    return FALSE;
}

int isDateBetween(Date date, Date from_date, Date to_date) {
    if (dateCmp(from_date, date) && dateCmp(date, to_date)) {
        return TRUE;
    } else {
        return FALSE;
    }
}
