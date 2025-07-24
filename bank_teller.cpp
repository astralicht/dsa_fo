/*

To do list:
- customer details not showing when checking account details

*/

/*
CS105-1

Julac Ontina
Jericho Capistrano
Ricoh Ng
*/

#include <iostream>
#include <map>
#include <cstring>
#include <cctype>
#include <fstream>
#include <conio.h>
#include <random> 
#include <string>   
#include <iomanip>  
#include <ranges>

#define ASCII_ESC 27
#define ASCII_ENTER 13
#define ASCII_BACKSPACE 8

using namespace std;

struct Customer {
	string cusNum, fname, mname, lname, phone_number, email;
};

struct Account {
	string accNum, cusNum;
	float balance;
};

struct Transaction {
	string transId, accNum, datetime, type;
	float amount;
};

void progLoop(); 
void memLoad(string filename, string tableName);
void memSave(string filename, string tableName); 
string genAccNum(); 

void header();
void viewAccount(Account acc);
Account fetchAccount(string accNum);
void printRecentTransactions(int count, string accNum);
void refreshAccBal(string accNum);

map<string, Customer> css;
map<string, Account> acs;
map<string, Transaction> trs;

int main() {
	memLoad("csv\\customers.csv", "customers");
	memLoad("csv\\accounts.csv", "accounts");
	memLoad("csv\\transactions.csv", "transactions");
	progLoop();
	return 0;
}

void progLoop() {
	char inp;
	string accNumInput;
	
	system("cls");
	header();
	cout << "Press any key to start...";
	inp = _getch();
	
	while(true) {
		if(inp == ASCII_ESC) break;
		
		system("cls");
		header();
		cout << "Enter account number: ";
		cin >> accNumInput;
	
		Account acc = fetchAccount(accNumInput);
		if (acc.accNum == "") {
			system("cls");
			header();
			cout << "Account not found!";
			cout << "Press any key to continue...";
			_getch();
			continue;
		}
		
		viewAccount(acc);
	}
}

void header() {
	cout << "--------------------------------\n";
	cout << "        BANKING SYSTEM         \n";
	cout << "--------------------------------\n\n";
}

void viewAccount(Account acc) {
	Customer cus = css[acc.cusNum];
	char inp;
	
	while (true) {
		system("cls");
		header();
		cout << "Account Number: " << acc.accNum << endl;
		cout << "Customer Name: " << cus.lname << ", " << cus.fname << " " << cus.mname << endl;
		cout << "\nBalance\n";
		cout << "---------------\n";
		cout << "Php " << fixed << setprecision(2) << acc.balance << endl;
		cout << "\nMost Recent Transactions\n";
		printRecentTransactions(10, acc.accNum);
		cout << "Options\n";
		cout << "---------------\n";
		cout << "1. Refresh Balance";
		cout << "ESC to exit account";
		inp = _getch();
		if (inp == ASCII_ESC) break;
		switch(inp) {
			case 1:
				refreshAccBal(acc.accNum);
			default:
				continue;
		}
	}
}

Account fetchAccount(string accNum) {
	if (acs.count(accNum)) {
		return acs[accNum];
	}
	return Account(); 
}

string genAccNum() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(100000, 999999); 

    string newAccNum;
    bool unique = false;
    while (!unique) {
        newAccNum = to_string(distrib(gen));
        if (acs.find(newAccNum) == acs.end()) {
            unique = true;
        }
    }
    return newAccNum;
}

void memSave(string filepath, string tableName) {
    ofstream ofs(filepath);

    if (!ofs.is_open()) {
        system("cls");
        header();
        cout << "System error: Could not open " << filepath << " for writing!\n";
        cout << "Please contact the system administrator to resolve the issue.\n";
        cout << "The program will exit now.";
        _getch();
        exit(0);
    }

    if (tableName == "customers") {
        for (const auto& pair : css) {
            const Customer& cus = pair.second;
            ofs << cus.cusNum << ","
                << cus.fname << ","
                << cus.mname << ","
                << cus.lname << ","
                << cus.phone_number << ","
                << cus.email << "\n";
        }
    } else if (tableName == "accounts") {
        for (const auto& pair : acs) {
            const Account& acc = pair.second;
            ofs << acc.accNum << ","
                << acc.cusNum << ","
                << fixed << setprecision(2) << acc.balance << "\n";
        }
    } else if (tableName == "transactions") {
        for (const auto& pair : trs) {
            const Transaction& tr = pair.second;
            ofs << tr.transId << ","
                << tr.accNum << ","
                << tr.datetime << ","
                << tr.type << ","
                << fixed << setprecision(2) << tr.amount << "\n";
        }
    }
    ofs.close();
}


void memLoad(string filepath, string tableName) {
	string line, str, cusNum, accNum, transId, balance, amountStr; 
	ifstream ifs(filepath);
	
	if (!ifs) {
		system("cls");
		header();
		cout << "System error!\n";
		cout << "Please contact the system administrator to resolve the issue.\n";
		cout << "The program will exit now.";
		_getch();
		exit(0);
	}
	
	if (tableName == "customers") {
		Customer cus;
		
		while (getline(ifs, cusNum, ',')) {
			cus = Customer();
			cus.cusNum = cusNum;
			getline(ifs, cus.fname, ',');
			getline(ifs, cus.mname, ',');
			getline(ifs, cus.lname, ',');
			getline(ifs, cus.phone_number, ',');
			getline(ifs, cus.email);
			css[cusNum] = cus;
		}
	} else if (tableName == "accounts") {
		Account acc;
		
		while (getline(ifs, accNum, ',')) {
			acc = Account();
			acc.accNum = accNum;
			getline(ifs, acc.cusNum, ',');
			getline(ifs, balance); 
			acc.balance = stof(balance);
			acs[accNum] = acc;
		}
	} else if (tableName == "transactions") {
		Transaction tr;
		
		while (getline(ifs, transId, ',')) {
			tr = Transaction();
			tr.transId = transId;
			getline(ifs, tr.accNum, ',');
			getline(ifs, tr.datetime, ',');
			getline(ifs, tr.type, ',');
			getline(ifs, amountStr); 
			tr.amount = stof(amountStr); 
			trs[transId] = tr;
		}
	}
	ifs.close();
}

void printRecentTransactions(int count, string accNum) {
	int i = 0;
	Transaction tr;
	for (const auto& pair : trs | views::reverse) {
		if (i == count) break;
		
		tr = pair.second;
		if (tr.accNum == accNum) {
			cout << tr.transId << " | " << tr.datetime << " | " << tr.type << " " << tr.amount << setprecision(2) << endl;
		}
		
		i++;
	}
}

void refreshAccBal(string accNum) {
    int i = 0;
    float bal = 0;
	Transaction tr;
	for (const auto& pair : trs | views::reverse) {
		if (i == count) break;
		
		tr = pair.second;
		if (tr.accNum == accNum) {
			if (tr.type == "DEPOSIT" || tr.type == "BANK TRANSFER") bal += tr.amount;
			else if (tr.type == "WITHDRAW") bal -= tr.amount;
		}
		
		i++;
	}
}
