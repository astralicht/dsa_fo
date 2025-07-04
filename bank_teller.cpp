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
	memLoad("csv\\customers.csv", "customers");
	memLoad("csv\\accounts.csv", "accounts");
	memLoad("csv\\transactions.csv", "transactions");
	
	cout << css["111"].fname;
//	progLoop();
}

void progLoop() {
	char inp;
	
	while(true) {
		system("cls");
		header();
		cout << "Press any key to start.";
		_getch();
		
		while(true) {
			system("cls");
			header();
			cout << "[1] View Account\n";
			cout << "[2] Open New Account\n";
			cout << "[3] View Customer Profile\n";
			cout << "[4] Create New Customer\n";
			cout << "[5] Update Customer Profile\n";
			cout << "[6] Withdrawal\n";
			cout << "[7] Deposit\n";
			cout << "[ESC] Cancel\n";
			cout << "> ";
			inp = _getch();
			
			if (inp == ASCII_ESC) break;
			
			system("cls");
			
			if (inp == '1') {
				viewAccount();
			} else continue;
		}
	}
}

void header() {
	cout << "=====           Three Blind Mice Bank           =====\n\n";
}

void viewAccount() {
	string input;
	char charInp;
	Account acc;
	Customer cus;
	
	while (true) {
		system("cls");
		input = "";
		charInp = '\0';
		header();
		cout << "Enter account number.\nEnter to confirm. ESC to cancel.\n> ";
		
		while (true) {
			charInp = _getch();
			if (charInp == ASCII_ESC) return;
			else if (charInp == ASCII_ENTER) break;
			else if (charInp == ASCII_BACKSPACE) {
				system("cls");
				header();
				cout << "Enter account number.\nEnter to confirm. ESC to cancel.\n> ";
				if (input.length() > 0) input.erase(input.length()-1, 1);
				cout << input;
			}
			else {
				input += charInp;
				cout << charInp;
			}
		}
		
		acc = acs[input];
		
		if (acc.accNum == "") {
			cout << "\nAccount not found.\nPress any key to try again.\n";
			_getch();
			continue;
		} else break;
	}
	
	cus = css[acc.cusNum];
	
	while (true) {
		system("cls");
		header();
		cout << "Account Number: " << acc.accNum << endl;
		cout << "Customer Name: " << cus.lname << ", " << cus.fname << " " << cus.mname << endl;
		cout << "\n---------------\n";
		cout << "Balance\n";
		cout << "---------------\n";
		cout << "Php " << acc.balance << endl;
		cout << "---------------\n";
		_getch();
	}
}

void memLoad(string filepath, string tableName) {
	string line, str, cusNum, accNum, transId, balance;
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
			getline(ifs, cus.email, ',');
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
	}
	
	ifs.close();
}

void memSave(string filename, string tableName) {
    // write to file code here
}

//string genAccNum() {
//	// code here to generate account number for new customer
//}
