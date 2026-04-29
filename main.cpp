#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

// =======================
// STRUCT: PaymentDetails
// =======================
struct PaymentDetails
{
    int paymentId;
    string paymentMethod;
    double amount;
    string transactionTime;
    string status;
    string authorizationCode;
};

// =======================
// CLASS: PaymentProcessor
// =======================
class PaymentProcessor
{
private:
    static int nextPaymentId;

public:
    PaymentProcessor() {}
    ~PaymentProcessor() {}

    PaymentDetails processCashPayment(double amount, double tendered);
    PaymentDetails processCardPayment(double amount, string cardType);
    PaymentDetails processMobilePayment(double amount, string provider);

    void displayPaymentReceipt(const PaymentDetails &p);
    string generateAuthorizationCode();
    string getCurrentTime();
    void saveToFile(const PaymentDetails &p);
};

// initialize static variable
int PaymentProcessor::nextPaymentId = 1001;

// ===================
// Utility Functions
// ===================
string PaymentProcessor::getCurrentTime()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[30];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

string PaymentProcessor::generateAuthorizationCode()
{
    string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string code = "AUTH-";
    for (int i = 0; i < 6; ++i)
        code += chars[rand() % chars.size()];
    return code;
}

void PaymentProcessor::saveToFile(const PaymentDetails &p)
{
    ofstream file("transactions.txt", ios::app);
    file << "PAY-" << p.paymentId << " | " << p.paymentMethod << " | $" << p.amount
         << " | " << p.status << " | " << p.transactionTime << endl;
    file.close();
}

// ===================
// Process Functions
// ===================
PaymentDetails PaymentProcessor::processCashPayment(double amount, double tendered)
{
    PaymentDetails p;
    p.paymentId = nextPaymentId++;
    p.paymentMethod = "Cash";
    p.amount = amount;
    p.transactionTime = getCurrentTime();

    if (tendered < amount)
    {
        p.status = "Failed (Insufficient)";
        p.authorizationCode = "N/A";
    }
    else
    {
        p.status = "Completed";
        p.authorizationCode = generateAuthorizationCode();
    }

    saveToFile(p);
    displayPaymentReceipt(p);
    return p;
}

PaymentDetails PaymentProcessor::processCardPayment(double amount, string cardType)
{
    PaymentDetails p;
    p.paymentId = nextPaymentId++;
    p.paymentMethod = cardType;
    p.amount = amount;
    p.transactionTime = getCurrentTime();

    // Random approval simulation
    bool approved = rand() % 2;
    if (approved)
    {
        p.status = "Completed";
        p.authorizationCode = generateAuthorizationCode();
    }
    else
    {
        p.status = "Declined";
        p.authorizationCode = "N/A";
    }

    saveToFile(p);
    displayPaymentReceipt(p);
    return p;
}

PaymentDetails PaymentProcessor::processMobilePayment(double amount, string provider)
{
    PaymentDetails p;
    p.paymentId = nextPaymentId++;
    p.paymentMethod = "Mobile (" + provider + ")";
    p.amount = amount;
    p.transactionTime = getCurrentTime();

    bool success = rand() % 2;
    if (success)
    {
        p.status = "Completed";
        p.authorizationCode = generateAuthorizationCode();
    }
    else
    {
        p.status = "Failed";
        p.authorizationCode = "N/A";
    }

    saveToFile(p);
    displayPaymentReceipt(p);
    return p;
}

// ===================
// Display Receipt
// ===================
void PaymentProcessor::displayPaymentReceipt(const PaymentDetails &p)
{
    cout << "\n=========== RECEIPT ===========\n";
    cout << "Transaction ID: PAY-" << p.paymentId << endl;
    cout << "Method: " << p.paymentMethod << endl;
    cout << "Amount: $" << fixed << setprecision(2) << p.amount << endl;
    cout << "Status: " << p.status << endl;
    cout << "Authorization Code: " << p.authorizationCode << endl;
    cout << "Time: " << p.transactionTime << endl;
    cout << "===============================\n";
}

// =======================
// CLASS: TransactionManager
// =======================
class TransactionManager
{
private:
    vector<PaymentDetails> transactions;

public:
    void addTransaction(const PaymentDetails &t) { transactions.push_back(t); }
    void showDailyReport();
};

void TransactionManager::showDailyReport()
{
    cout << "\n=== DAILY REPORT ===\n";
    double total = 0;
    int count = 0;
    for (size_t i = 0; i < transactions.size(); i++)
    {
        
        const PaymentDetails &t = transactions[i];
        cout << "PAY-" << t.paymentId << " | " << t.paymentMethod
             << " | $" << fixed << setprecision(2) << t.amount
             << " | " << t.status << endl;
        if (t.status == "Completed")
            total += t.amount;
        count++;
    }
    cout << "Total Transactions: " << count << endl;
    cout << "Total Completed Amount: $" << total << endl;
    cout << "=====================\n";
}

// =======================
// MAIN POS SYSTEM LOOP
// =======================
void runPOSSystem()
{
    srand((unsigned)time(0));
    PaymentProcessor processor;
    TransactionManager manager;

    int choice = 0;
    do
    {
        cout << "\n=== PAYMENT SYSTEM MENU ===\n";
        cout << "1. Cash Payment\n";
        cout << "2. Credit Card Payment\n";
        cout << "3. Debit Card Payment\n";
        cout << "4. Mobile Payment\n";
        cout << "5. View Daily Report\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Try again.\n";
            continue;
        }

        double amount, tendered;
        string provider;
        PaymentDetails result;

        switch (choice)
        {
        case 1:
            cout << "Enter amount: $";
            cin >> amount;
            cout << "Enter cash tendered: $";
            cin >> tendered;
            result = processor.processCashPayment(amount, tendered);
            manager.addTransaction(result);
            break;

        case 2:
            cout << "Enter amount: $";
            cin >> amount;
            result = processor.processCardPayment(amount, "Credit Card");
            manager.addTransaction(result);
            break;

        case 3:
            cout << "Enter amount: $";
            cin >> amount;
            result = processor.processCardPayment(amount, "Debit Card");
            manager.addTransaction(result);
            break;

        case 4:
            cout << "Enter amount: $";
            cin >> amount;
            cout << "Enter provider: ";
            cin >> provider;
            result = processor.processMobilePayment(amount, provider);
            manager.addTransaction(result);
            break;

        case 5:
            manager.showDailyReport();
            break;

        case 6:
            cout << "Exiting system..." << endl;
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

// =======================
// MAIN FUNCTION
// =======================
int main()
{
    runPOSSystem();
    return 0;
}
