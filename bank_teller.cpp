#include <iostream>
#include <map>
#include <cstring>
#include <fstream>
#include <sstream>
#include <conio.h>

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

void progLoop(); // main program loop; put in main func
void memLoad(string filename, string tableName);
void memSave(string filename, string tableName);

void header();
void viewAccount();

Account fetchAccount(string accNum);
string genAccNum();

map<string, Customer> css;
map<string, Account> acs;
map<string, Transaction> trs;

int main() {
	memLoad("customers.csv", "customers");
	memLoad("accounts.csv", "accounts");
	memLoad("transactions.csv", "transactions");
	progLoop();
}

void progLoop() {
	char inp;
	
	while(true) {
		system("cls");
		header();
		cout << "\nPress any key to start.\n";
		_getch();
		
		while(true) {
			system("cls");
			header();
			cout << "[1] View Account\n";
			cout << "[2] Open New Account\n";
			cout << "[0] Cancel\n";
			inp = _getch();
			
			system("cls");
			
			if (inp == '1') {
				viewAccount();
			} else {
				continue;
			}
		}
	}
}

void header() {
	cout << "=====           Three Blind Mice Bank           =====\n";
}

void viewAccount() {
	string input;
	Account acc;
	
	while (true) {
		system("cls");
		header();
		cout << "Enter account number";
		cout << "> ";
		cin >> input;
		acc = fetchAccount(input);
		
		if (acc.accNum == "") {
			cout << "\nAccount not found.\nPress any key to try again.\n";
			_getch();
			continue;
		} else {
			break;
		}
	}
	
	while (true) {
		system("cls");
		header();
		
	}
}

void memLoad(string filepath, string tableName) {
	string line, str, cusNum, accNum, transId, balance;
	ifstream ifs(filepath);
	
	if (!ifs) {
		system("cls");
		cout << "System error!";
		cout << "Please contact the system administrator to resolve the issue.";
		cout << "The program will exit now.";
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
			getline(ifs, cus.email, ',');
			css.insert(make_pair(cusNum, cus));
		}
	} else if (tableName == "accounts") {
		Account acc;
		
		while (getline(ifs, accNum, ',')) {
			acc = Account();
			acc.accNum = accNum;
			getline(ifs, acc.cusNum, ',');
			getline(ifs, balance);
			acc.balance = stof(balance);
		}
	}
	
	ifs.close();
}

//void memSave(string filename, string tableName) {
//    // write to file code here
//    
//    // use this for loop iterator
//    for (map<string, Customer>::iterator itr = css.begin(); itr != css.end(); itr++) {
//        cout << "Saving " << itr->first << "...\n";
//    }
//}

Account fetchAccount(string accNum) {
	for (map<string, Account>::iterator itr = acs.begin(); itr != acs.end(); itr++) {
        if (itr->second.accNum == accNum) {
        	return itr->second;
		}
    }
}

//string genAccNum() {
//	// code here to generate account number for new customer
//}

