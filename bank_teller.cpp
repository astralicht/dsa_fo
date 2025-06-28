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

void checkBalScreen();
void withdrawScreen();
void activateATMCardScreen();
Account fetchAccount(string cardNum, string pin);
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
		cout << "=====           Three Blind Mice Bank           =====\n";
		cout << "=====                    ATM                    =====\n";
		cout << "\n               Press any key to start.\n";
		_getch();
		
		while(true) {
			system("cls");
			cout << "=====           Three Blind Mice Bank           =====\n";
			cout << "=====                    ATM                    =====\n";
			cout << "[1] Check Balance\n";
			cout << "[2] Withdraw\n";
			cout << "[3] Activate ATM Card\n";
			cout << "[0] Cancel\n";
			inp = _getch();
			
			system("cls");
			
			if (inp == '1') {
				checkBalScreen();
			} else if (inp == '2') {
				withdrawScreen();
			} else if (inp == '3') {
				activateATMCardScreen();
			} else if (inp == '0') {
				break;
			} else {
				continue;
			}
		}
	}
}

void checkBalScreen() {
//	char inp;
//	Account acc;
//		
//	while(true) {
//		system("cls");
//		cout << "=====           Three Blind Mice Bank           =====\n";
//		cout << "Enter Card Number > ";
//		cin >> cardNum;
//		cout << "Enter PIN > ";
//		cin >> pin;
//		
//		acc = fetchAccount(cardNum, pin);
//		
//		while (acc.atmPIN != pin) {
//			system("cls");
//			cout << "=====           Three Blind Mice Bank           =====\n";
//			cout << "Incorrect PIN. Please try again.";
//			cout << "Enter PIN > ";
//			cin >> pin;
//		}
//		
//		while (true) {
//			system("cls");
//			cout << "=====           Three Blind Mice Bank           =====\n";
//			cout << "Your balance is: Php " << acc.bal << endl;
//			cout << "Press enter to return to main menu.";
//			_getch();
//			return;
//		}
//	}
}

void withdrawScreen() {
	// enter code here
}

void activateATMCardScreen() {
	// enter code here
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

//Account fetchAccount(string cardNum, string atmPIN) {
//	Account acc;
//	for (map<string, Account>::iterator itr = accs.begin(); itr != accs.end(); itr++) {
//        if (itr->second.cardNum == cardNum) {
//        	acc = itr->second;
//		}
//    }
//    return acc;
//}
//
//string genAccNum() {
//	// code here to generate account number for new customer
//}

