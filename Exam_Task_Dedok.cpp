#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector> //для работы с векторами
#include <map> 
#include <algorithm> 
#include <fstream> //для работы с файлами
#include <ctime> //для работы с датами
using namespace std;
class Wallet { //создаём кошелёк 
protected:
    string name;
    double balance;

public:
    Wallet(const string& name) : name(name), balance(0.0) {}
    virtual ~Wallet() {}
    string getName() const {
        return name;
    }
    double getBalance() const {
        return balance;
    }
    virtual void deposit(double amount) = 0;
    virtual void addExpense(double amount, const string& category) = 0;
    virtual void printSummary() const = 0;
};
class CashWallet : public Wallet { //создание денежного кошелька путём наследования
public:
    CashWallet(const string& name) : Wallet(name) {}

    void deposit(double amount) override {
        balance += amount;
    }
    void addExpense(double amount, const string& category) override {
        balance -= amount;
    }
    void printSummary() const override {
        cout << "Cash Wallet: " << name << endl;
        cout << "Balance: " << balance << endl;
    }
};
class Card : public Wallet { //создание карт
private:
    string cardType;
public:
    Card(const string& name, const string& cardType) : Wallet(name), cardType(cardType) {}
    void deposit(double amount) override {
        balance += amount;
    }
    void addExpense(double amount, const string& category) override {
        balance -= amount;
    }
    void printSummary() const override {
        cout << cardType << " Card: " << name << endl;
        cout << "Balance: " << balance << endl;
    }
};
class Expense { //создание затрат
private:
    double amount;
    string category;
    time_t timestamp; 
public:
    Expense(double amount, const string& category, time_t timestamp)
        : amount(amount), category(category), timestamp(timestamp) {}
    double getAmount() const {
        return amount;
    }
    string getCategory() const {
        return category;
    }
    time_t getTimestamp() const {
        return timestamp;
    }
};
struct CompareExpense { //сравниваем затраты
    bool operator()(const Expense& expense1, const Expense& expense2) {
        return expense1.getAmount() > expense2.getAmount();
    }
};
class PersonalFinanceManager { //класс финансового менеджера
private:
    vector<Wallet*> wallets; //создаем вектор кошельков
    map<string, vector<Expense>> expenses; 
    map<string, map<time_t, double>> dailyExpenses; //статистика затрат по дням
    map<string, map<time_t, double>> weeklyExpenses; //статистика затрат по неделям
    map<string, map<time_t, double>> monthlyExpenses; //статистика затрат по месяцам
public:
    PersonalFinanceManager() {} //конструктор
    ~PersonalFinanceManager() { //деструктор
        for (Wallet* wallet : wallets) {
            delete wallet;
        }
    }
    void createWallet() { //создание и выбор типа кошелька
        if (wallets.size() >= 10) { //не больше 10 кошельков
            cout << "Maximum wallet limit reached." << endl;
            return;
        }
        int walletType;
        cout << "Select wallet type:" << endl;
        cout << "1. Cash Wallet" << endl;
        cout << "2. Debit Card" << endl;
        cout << "3. Credit Card" << endl;
        cout << "Enter wallet type: ";
        cin >> walletType;
        string walletName;
        cout << "Enter wallet name: ";
        cin >> walletName;
        Wallet* newWallet = nullptr; //по умолчанию у нас все кошельки являются nullptr-овскими
        switch (walletType) {
        case 1:
            newWallet = new CashWallet(walletName);
            break;
        case 2:
            newWallet = new Card(walletName, "Debit");
            break;
        case 3:
            newWallet = new Card(walletName, "Credit");
            break;
        default:
            cout << "Invalid wallet type." << endl;
            return;
        }
        if (newWallet != nullptr) {
            wallets.push_back(newWallet);
            cout << "Wallet created successfully." << endl;
        }
    }
    void depositToWallet() { //пополнение кошелька
        string walletName;
        cout << "Enter wallet name: ";
        cin >> walletName;
        double amount;
        cout << "Enter amount to deposit: ";
        cin >> amount;
        Wallet* wallet = findWallet(walletName);
        if (wallet != nullptr) {
            wallet->deposit(amount);
            cout << "Deposit successful." << endl;
        }
        else {
            cout << "Wallet not found." << endl;
        }
    }
    void addExpense() { //добавление затрат
        string walletName;
        cout << "Enter wallet name: ";
        cin >> walletName;
        double amount;
        cout << "Enter expense amount: ";
        cin >> amount;
        string category;
        cout << "Enter expense category: ";
        cin >> category;
        Wallet* wallet = findWallet(walletName);
        if (wallet != nullptr) {
            wallet->addExpense(amount, category);
            time_t timestamp = time(nullptr);
            Expense expense(amount, category, timestamp);
            expenses[walletName].push_back(expense);
            updateExpenseStats(walletName, expense); //обновление статистики
            cout << "Expense added successfully." << endl;
        }
        else {
            cout << "Wallet not found." << endl;
        }
    }
    void printWalletSummary() { //печать статистики кошелька(ов)
        string walletName;
        cout << "Enter wallet name: ";
        cin >> walletName;
        Wallet* wallet = findWallet(walletName);
        if (wallet != nullptr) {
            wallet->printSummary();
        }
        else {
            cout << "Wallet not found." << endl;
        }
    }
    void printExpenseSummary() { //печать статистики затрат кошелька(ов)
        cout << "Expense Summary" << endl;
        cout << "----------------" << endl;
        for (const auto& expensePair : expenses) {
            const vector<Expense>& expenseList = expensePair.second;
            for (const Expense& expense : expenseList) {
                cout << "Amount: " << expense.getAmount() << endl;
                cout << "Category: " << expense.getCategory() << endl;
                time_t timestamp = expense.getTimestamp();
                cout << "Timestamp: " << ctime(&timestamp);
                cout << "----------------" << endl;
            }
        }
    }
    void saveExpenseSummary() { //сохранение статистики в отдельный файл expense_summary.txt 
        ofstream outputFile("expense_summary.txt");
        if (!outputFile) {
            cout << "Error opening file." << endl;
            return;
        }
        outputFile << "Expense Summary" << endl;
        outputFile << "----------------" << endl;
        for (const auto& expensePair : expenses) {
            const vector<Expense> &expenseList = expensePair.second;
        for(const Expense & expense : expenseList) {
            time_t timestamp = expense.getTimestamp();
            outputFile << "Amount: " << expense.getAmount() << endl;
            outputFile << "Category: " << expense.getCategory() << endl;
            outputFile << "Timestamp: " << ctime(&timestamp);
            outputFile << "----------------" << endl;
        }
	}
        outputFile.close();
        cout << "Expense summary saved to expense_summary.txt" << endl;
    }
private:
    Wallet* findWallet(const string& name) { //поиск кошелька
        for (Wallet* wallet : wallets) {
            if (wallet->getName() == name) {
                return wallet;
            }
        }
        return nullptr;
    }
    void updateExpenseStats(const string& walletName, const Expense& expense) { //обновление статистики затрат кошелька
        time_t timestamp = expense.getTimestamp();
        struct tm* timeinfo = localtime(&timestamp);
        dailyExpenses[walletName][timestamp] += expense.getAmount(); //добавление затрат в статистику
        weeklyExpenses[walletName][timestamp] += expense.getAmount();
        monthlyExpenses[walletName][timestamp] += expense.getAmount();
        timeinfo->tm_hour = 0;
        timeinfo->tm_min = 0;
        timeinfo->tm_sec = 0;
        timestamp = mktime(timeinfo);
        dailyExpenses[walletName][timestamp] += expense.getAmount(); //обновление статистики за день, неделю и месяц
        timeinfo->tm_hour = 0;
        timeinfo->tm_min = 0;
        timeinfo->tm_sec = 0;
        timeinfo->tm_mday -= timeinfo->tm_wday;
        timestamp = mktime(timeinfo);
        weeklyExpenses[walletName][timestamp] += expense.getAmount();
        timeinfo->tm_hour = 0;
        timeinfo->tm_min = 0;
        timeinfo->tm_sec = 0;
        timeinfo->tm_mday = 1;
        timestamp = mktime(timeinfo);
        monthlyExpenses[walletName][timestamp] += expense.getAmount();
    }
};
int main() {
    PersonalFinanceManager manager; //меню программы
    while (true) {
        cout << "Personal Finance Manager" << endl;
        cout << "1. Create Wallet" << endl;
        cout << "2. Deposit to Wallet" << endl;
        cout << "3. Add Expense" << endl;
        cout << "4. Print Wallet Summary" << endl;
        cout << "5. Print Expense Summary" << endl;
        cout << "6. Save Expense Summary" << endl;
        cout << "7. Exit" << endl;
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) { //выбор действия пользователем
        case 1:
            manager.createWallet();
            break;
        case 2:
            manager.depositToWallet();
            break;
        case 3:
            manager.addExpense();
            break;
        case 4:
            manager.printWalletSummary();
            break;
        case 5:
            manager.printExpenseSummary();
            break;
        case 6:
            manager.saveExpenseSummary();
            break;
        case 7:
            cout << "Exiting program." << endl;
            return 0;
        default:
            cout << "Invalid choice." << endl;
            break;
        }
        cout << endl;
    }
    return 0;
}