#include <iostream>
#include <ctime>
#include <map>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

string currentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    dt[strlen(dt) - 1] = '\0'; // Remove newline
    return string(dt);
}

class Transaction{
public:
    string date;
    string type;
    double amount;
    string counterparty;


    string tostring(){
        return date + "|" + type+ "Ksh" + to_string (amount) + "|"  + counterparty ;
    }

};

class Account{
public:
    string account_number;
    string holder_name;
    double balance = 0.0;
    vector <Transaction> transactions;

    Account(){}
    Account(string number, string name){
        account_number = number;
        holder_name = name;
    }

    void deposit(double amount){
        if(amount <= 0 ){
            cout << "Invalid deposit amount.Must  be greater than 0" << endl;
            return;
        }
            balance += amount;
            transactions.push_back({currentTime(), "Deposit ",amount ,"-"});
            cout << "Deposited Ksh" << amount << "New Balance is : Ksh" << balance << endl;
        
    }

    void withdraw(double amount){
        if (amount > balance){
            cout << "Insufficient balance." << endl;
            return;

        }
            balance -= amount;
            transactions.push_back({currentTime(), "Withdrawal", amount, "-"});
            cout <<"You have withdrawn Ksh" << amount << " New balance is: Ksh" << balance << endl;
        
    }

    void transfer(Account &recipient , double amount){
        if (amount > balance){
            cout << "Insufficient amount to make your transfer." << endl;
            return;

        }
            balance -= amount;
            recipient.balance += amount;
            transactions.push_back({currentTime(), "Transfer Out", amount , recipient.account_number});
            recipient.transactions.push_back({currentTime(), "Transfer In", amount, account_number});
            cout << "Transfered Ksh " << amount << "To " << recipient.holder_name << endl;   
    }
    void showTransactions(){
        cout << "Transaction History " << endl;
        for (auto &t : transactions){
            cout << t.tostring() << endl;
        }
    }
    void saveToFile(){
        ofstream file (account_number + ".txt");
        file << account_number << endl;
        file << holder_name << endl;
        file << balance << endl;
        for (auto &t : transactions){
            file << t.date << "," << t.type << "," << t.amount << "," << t.counterparty << endl;
        }
        file.close();
    }
    void loadFromFile(){
        ifstream file (account_number + ".txt");
        if ( !file.is_open())return;
        getline(file ,account_number);
        getline(file , holder_name);
        file >> balance;
        file.ignore();
        string line;
        while (getline(file , line)){
            size_t p1 = line.find(',');
            size_t p2 = line.find(',', p1 + 1);
            size_t p3 = line.find(',' , p2 + 1);
            Transaction t = {
                line.substr(0 , p1),
                line.substr(p1 + 1, p2 - p1 -1),
                stod(line.substr(p2 + 1, p3 - p2 - 1)),
                line.substr(p3 + 1)
            };
            transactions.push_back(t);
        }
        file.close();
    }
};

map <string ,Account > account;

void loadAccounts(){
    Account a("1","Leah");
    a.loadFromFile();
    account[a.account_number]= a;
    Account b("2","Jane");
    b.loadFromFile();
    account[b.account_number]= b;
}

int main(){
    string user_name;
    int password;

    cout << "Login  To Our Bank" << endl ;
    cout << "ENTER USER NAME";
    cin >> user_name;
    cout << "Enter userpassword";
    cin >> password;

    if (user_name != "Customer" || password != 1234){
        cout << "Invalid Credetials.Try Again"<< endl;
        return 0;
    }

    loadAccounts();
    Account &myAccount = account["1"];

    int choice;
    do{
        cout << "WELCOME TO SMART BANK";
        cout << " Account :" << myAccount.account_number << " Holder Name " << myAccount.holder_name << " Your Balance :  Ksh " << myAccount.balance << endl;

        cout << " 1. Deposit" ;
         " 2. Withdraw" ;
        cout << " 3. Transfer" ;
        cout << " 4. Transaction History" ;
        cout << " 5. Logout" ;
        cin >> choice;
        
        if (choice == 1){
            double amount;
            cout << "Enter amout to Deposit : ";
            cin >> amount;
            myAccount.deposit(amount);

        }else if(choice == 2){
            double amount;
            cout << "Enter amount to withdraw : ";
            cin >> amount;
            myAccount.withdraw(amount);

        }else if (choice == 3){
            string recAcc;
            double amount;
            cout << " Enter the Recipient account number : ";
            cin >> recAcc;
            cout << "Enter amount to Transfer: ";
            cin >> amount;
            if(account.find(recAcc) != account.end()){
                myAccount.transfer(account[recAcc], amount);
            }else{
                cout << "Recipient Account Not Found." << endl;
            }
        }else if (choice == 4){
            myAccount.showTransactions();

        }else if (choice == 5){
            cout << " Logging out..." << endl;

        }else{
            cout << "Invalid Choice. Choose From 1-5" << endl;
        }

    }while (choice != 5);
    
    for (auto &pair : account){
        pair.second.saveToFile();
    }
    return 0;
}