
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50

// Structure for a bank account
typedef struct {
    int accountNumber;
    char accountHolder[MAX_NAME_LENGTH];
    float balance;
} Account;

// Structure for a mini statement entry
typedef struct MiniStatementEntry {
    char transactionType[10]; // "Deposit" or "Withdraw"
    float amount;
    struct MiniStatementEntry* next;
} MiniStatementEntry;

// Structure for a bank account with a mini statement
typedef struct {
    Account account;
    MiniStatementEntry* miniStatementHead;
} BankAccount;

// Function prototypes
void createAccount(BankAccount* accounts, int* accountCount);
void deposit(BankAccount* accounts, int accountCount);
void withdraw(BankAccount* accounts, int accountCount);
void printMiniStatement(BankAccount* accounts, int accountCount);
void freeMiniStatements(BankAccount* accounts, int accountCount);

int main() {
    BankAccount accounts[MAX_ACCOUNTS];
    int accountCount = 0;
    int choice;

    while (1) {
        printf("\nSimple Banking Application\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Print Mini Statement\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount(accounts, &accountCount);
                break;
            case 2:
                deposit(accounts, accountCount);
                break;
            case 3:
                withdraw(accounts, accountCount);
                break;
            case 4:
                printMiniStatement(accounts, accountCount);
                break;
            case 5:
                freeMiniStatements(accounts, accountCount);
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void createAccount(BankAccount* accounts, int* accountCount) {
    if (*accountCount >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached.\n");
        return;
    }

    BankAccount newAccount;
    newAccount.account.accountNumber = *accountCount + 1;
    printf("Enter account holder name: ");
    scanf("%s", newAccount.account.accountHolder);
    newAccount.account.balance = 0.0;
    newAccount.miniStatementHead = NULL;

    accounts[*accountCount] = newAccount;
    (*accountCount)++;
    printf("Account created successfully! Account Number: %d\n", newAccount.account.accountNumber);
}

void deposit(BankAccount* accounts, int accountCount) {
    int accountNumber;
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    if (accountNumber < 1 || accountNumber > accountCount) {
        printf("Invalid account number.\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid deposit amount.\n");
        return;
    }

    accounts[accountNumber - 1].account.balance += amount;

    // Add to mini statement
    MiniStatementEntry* newEntry = (MiniStatementEntry*)malloc(sizeof(MiniStatementEntry));
    strcpy(newEntry->transactionType, "Deposit");
    newEntry->amount = amount;
    newEntry->next = accounts[accountNumber - 1].miniStatementHead;
    accounts[accountNumber - 1].miniStatementHead = newEntry;

    printf("Deposited %.2f to account number %d. New balance: %.2f\n",
           amount, accountNumber, accounts[accountNumber - 1].account.balance);
}

void withdraw(BankAccount* accounts, int accountCount) {
    int accountNumber;
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    if (accountNumber < 1 || accountNumber > accountCount) {
        printf("Invalid account number.\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0 || amount > accounts[accountNumber - 1].account.balance) {
        printf("Invalid withdraw amount.\n");
        return;
    }

    accounts[accountNumber - 1].account.balance -= amount;

    // Add to mini statement
    MiniStatementEntry* newEntry = (MiniStatementEntry*)malloc(sizeof(MiniStatementEntry));
    strcpy(newEntry->transactionType, "Withdraw");
    newEntry->amount = amount;
    newEntry->next = accounts[accountNumber - 1].miniStatementHead;
    accounts[accountNumber - 1].miniStatementHead = newEntry;

    printf("Withdrew %.2f from account number %d. New balance: %.2f\n",
           amount, accountNumber, accounts[accountNumber - 1].account.balance);
}

void printMiniStatement(BankAccount* accounts, int accountCount) {
    int accountNumber;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    if (accountNumber < 1 || accountNumber > accountCount) {
        printf("Invalid account number.\n");
        return;
    }

    BankAccount* account = &accounts[accountNumber - 1];
    printf("\nMini Statement for Account Number: %d (%s)\n", 
           account->account.accountNumber, account->account.accountHolder);
    printf("Current Balance: %.2f\n", account->account.balance);
    printf("Transactions:\n");

    MiniStatementEntry* current = account->miniStatementHead;
    if (!current) {
        printf("No transactions found.\n");
    }

    while (current != NULL) {
        printf("%s: %.2f\n", current->transactionType, current->amount);
        current = current->next;
    }
}

void freeMiniStatements(BankAccount* accounts, int accountCount) {
    for (int i = 0; i < accountCount; i++) {
        MiniStatementEntry* current = accounts[i].miniStatementHead;
        while (current != NULL) {
            MiniStatementEntry* temp = current;
            current = current->next;
            free(temp);
        }
    }
}
