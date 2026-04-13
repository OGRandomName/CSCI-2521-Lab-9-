/**
 * @file Lab9_kcherry8.cpp
 * @author Kenneth Cherry
 * @date 2026-04-12
 * @brief Demonstrates inheritance and polymorphism using Account and SavingsAccount classes.
 */

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
using namespace std;

/**
 * @brief Represents a bank customer.
 */
class Customer {
private:
    string customerID;
    string email;

public:
    /**
     * @brief Default constructor.
     */
    Customer() : customerID("Unknown"), email("none@example.com") {}

    /**
     * @brief Parameterized constructor.
     * @param id Customer ID.
     * @param emailAddr Customer email.
     */
    Customer(const string& id, const string& emailAddr)
        : customerID(id), email(emailAddr) {}

    /**
     * @brief Gets the customer ID.
     * @return Customer ID.
     */
    string getCustomerID() const { return customerID; }

    /**
     * @brief Gets the customer email.
     * @return Email address.
     */
    string getEmail() const { return email; }

    /**
     * @brief Prints customer information.
     */
    void printCustomerInfo() const {
        cout << "Customer ID: " << customerID << endl;
        cout << "Email: " << email << endl;
    }
};

/**
 * @brief Base class representing a bank account.
 */
class Account {
protected:
    string accountNumber;
    double balance;
    Customer owner;

    vector<double> deposits;
    vector<double> withdrawals;

    /**
     * @brief Prints a summary of daily account activity.
     */
    void printSummary(double previousBalance, double totalDeposits,
                      double totalWithdrawals, double newBalance) const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Previous Balance: $" << previousBalance << endl;
        cout << "Total Deposits: $" << totalDeposits << endl;
        cout << "Total Withdrawals: $" << totalWithdrawals << endl;
        cout << "New Balance after COB: $" << newBalance << endl;
    }

public:
    /**
     * @brief Default constructor.
     */
    Account() : accountNumber("0000000000"), balance(0.0), owner(Customer()) {}

    /**
     * @brief Parameterized constructor.
     */
    Account(const string& accNum, double initialBalance, const Customer& customer)
        : accountNumber(accNum), balance(initialBalance), owner(customer) {}

    /**
     * @brief Virtual destructor for polymorphism.
     */
    virtual ~Account() = default;

    /**
     * @brief Adds a deposit transaction.
     * @return True if valid.
     */
    bool deposit(double amount) {
        if (amount > 0) {
            deposits.push_back(amount);
            return true;
        }
        return false;
    }

    /**
     * @brief Adds a withdrawal transaction.
     * @return True if valid.
     */
    bool withdraw(double amount) {
        if (amount > 0) {
            withdrawals.push_back(amount);
            return true;
        }
        return false;
    }

    /**
     * @brief Virtual function to process daily transactions.
     */
    virtual void closeOfBusiness() {
        double totalDeposits = accumulate(deposits.begin(), deposits.end(), 0.0);
        double totalWithdrawals = accumulate(withdrawals.begin(), withdrawals.end(), 0.0);

        double previousBalance = balance;
        double newBalance = previousBalance + totalDeposits - totalWithdrawals;

        printSummary(previousBalance, totalDeposits, totalWithdrawals, newBalance);

        balance = newBalance;
        deposits.clear();
        withdrawals.clear();
    }

    /**
     * @brief Gets the current balance.
     */
    double getBalance() const { return balance; }

    /**
     * @brief Gets the account number.
     */
    string getAccountNumber() const { return accountNumber; }

    /**
     * @brief Gets the customer object.
     */
    Customer getCustomer() const { return owner; }

    /**
     * @brief Virtual function to print account info.
     */
    virtual void printAccountInfo() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Current Balance: $" << balance << endl;
        owner.printCustomerInfo();
    }

    /**
     * @brief Prints pending transactions.
     */
    void printTransactions() const {
        cout << "Deposits: ";
        for (double d : deposits) cout << "$" << d << " ";
        cout << endl;

        cout << "Withdrawals: ";
        for (double w : withdrawals) cout << "$" << w << " ";
        cout << endl;
    }
};

/**
 * @brief Derived class representing a savings account with daily interest.
 */
class SavingsAccount : public Account {
private:
    double dailyInterestRate;

public:
    /**
     * @brief Constructor for SavingsAccount.
     */
    SavingsAccount(const string& accNum, double initBal, const Customer& cust, double rate)
        : Account(accNum, initBal, cust), dailyInterestRate(rate) {}

    /**
     * @brief Prints account info including interest rate.
     */
    void printAccountInfo() const override {
        Account::printAccountInfo();
        cout << "Interest Rate: " << dailyInterestRate * 100 << "%" << endl;
    }

    /**
     * @brief Applies transactions and interest.
     */
    void closeOfBusiness() override {
        double totalDeposits = accumulate(deposits.begin(), deposits.end(), 0.0);
        double totalWithdrawals = accumulate(withdrawals.begin(), withdrawals.end(), 0.0);

        double previousBalance = balance;
        double newBalance = previousBalance + totalDeposits - totalWithdrawals;

        if (newBalance > 0) {
            double interest = newBalance * dailyInterestRate;
            newBalance += interest;
        }

        printSummary(previousBalance, totalDeposits, totalWithdrawals, newBalance);

        balance = newBalance;
        deposits.clear();
        withdrawals.clear();
    }
};

// ----------------------
// Menu Functions (unchanged)
// ----------------------

void printMenu();
int getMenuOption();
bool isAmountValid(double amount);
bool isMenuChoiceValid(int choice);
void printInvalidMenuChoiceMessage();
double getDepositAmount();
double getWithdrawalAmount();
void printInvalidAmountMessage();

const int DEPOSIT = 1;
const int WITHDRAWAL = 2;
const int CLOSE_OF_BUSINESS = 3;
const int QUIT = 4;

/**
 * @brief Main entry point demonstrating polymorphism.
 * @return 0 on success.
 */
int main() {
    Customer customer("0123456789", "customer@cscc.edu");

    Account* account = new SavingsAccount("5423678409", 100.00, customer, 0.02);

    account->printAccountInfo();

    int menuChoice = getMenuOption();
    while (menuChoice != QUIT) {
        double deposit = 0, withdrawal = 0;

        switch (menuChoice) {
        case DEPOSIT:
            deposit = getDepositAmount();
            account->deposit(deposit);
            break;
        case WITHDRAWAL:
            withdrawal = getWithdrawalAmount();
            account->withdraw(withdrawal);
            break;
        case CLOSE_OF_BUSINESS:
            account->closeOfBusiness();
            break;
        }

        menuChoice = getMenuOption();
    }

    delete account;
    return 0;
}

void printMenu() {
    cout << "\nCSCC Bank and Trust" << endl;
    cout << "Select an option:" << endl;
    cout << DEPOSIT << ". Make a deposit" << endl;
    cout << WITHDRAWAL << ". Make a withdrawal" << endl;
    cout << CLOSE_OF_BUSINESS << ". Close of Business" << endl;
    cout << QUIT << ". Quit" << endl;
}

int getMenuOption() {
    int choice = 0;

    do {
        printMenu();
        cin >> choice;

        if (!isMenuChoiceValid(choice)) {
            printInvalidMenuChoiceMessage();
        }
    } while (!isMenuChoiceValid(choice));

    return choice;
}

bool isMenuChoiceValid(int choice) {
    return choice >= DEPOSIT && choice <= QUIT;
}

bool isAmountValid(double amount) {
    return amount > 0;
}

void printInvalidMenuChoiceMessage() {
    cout << "Invalid option selected. Please try again." << endl;
}

double getDepositAmount() {
    double deposit = 0;
    do {
        cout << "Please enter the deposit amount: $";
        cin >> deposit;

        if (!isAmountValid(deposit)) {
            printInvalidAmountMessage();
        }

    } while (!isAmountValid(deposit));

    return deposit;
}

double getWithdrawalAmount() {
    double withdrawal = 0;
    do {
        cout << "Please enter the withdrawal amount: $";
        cin >> withdrawal;

        if (!isAmountValid(withdrawal)) {
            printInvalidAmountMessage();
        }

    } while (!isAmountValid(withdrawal));

    return withdrawal;
}

void printInvalidAmountMessage() {
    cout << "The dollar amount must be greater than $0.00. Please try again." << endl;
}
