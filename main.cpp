#include <iostream>
#include <map>
#include <cstring>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <conio.h>

using namespace std;

struct Account {
	string accNum, cardNum, atmPIN, fname, mname, lname;
	float bal;
};

void progLoop(); // main program loop; put in main func
void memLoad(string filename, string tableName);
void memSave(string filename, string tableName);

void checkBalScreen();
void withdrawScreen();
void activateATMCardScreen();
Account fetchAccount(string cardNum, string pin);
string genAccNum();

map<string, Account> accs;

int main() {
	memLoad("accounts.csv", "accounts");
	progLoop();
//	memSave("csv\accounts.csv", "accounts");
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
	char inp;
	string cardNum;
	string pin;
	Account acc;
		
	while(true) {
		system("cls");
		cout << "=====           Three Blind Mice Bank           =====\n";
		cout << "=====                    ATM                    =====\n";
		cout << "Enter Card Number > ";
		cin >> cardNum;
		cout << "Enter PIN > ";
		cin >> pin;
		
		acc = fetchAccount(cardNum, pin);
		
		while (acc.atmPIN != pin) {
			system("cls");
			cout << "=====           Three Blind Mice Bank           =====\n";
			cout << "=====                    ATM                    =====\n";
			cout << "Incorrect PIN. Please try again.";
			cout << "Enter PIN > ";
			cin >> pin;
		}
		
		while (true) {
			system("cls");
			cout << "=====           Three Blind Mice Bank           =====\n";
			cout << "=====                    ATM                    =====\n";
			cout << "Your balance is: Php " << acc.bal << endl;
			cout << "Press enter to return to main menu.";
			_getch();
			return;
		}
	}
}

void withdrawScreen() {
	// enter code here
}

void activateATMCardScreen() {
	// enter code here
}

void memLoad(string filename, string tableName) {
	string line, str, accNum;
	ifstream CSV(filename);
	
	if (tableName == "accounts") {
		Account acc;
		string bal;
		
		while (getline(CSV, accNum, ',')) {
			acc = Account();
			acc.accNum = accNum;
			getline(CSV, acc.cardNum, ',');
			getline(CSV, acc.atmPIN, ',');
			getline(CSV, acc.fname, ',');
			getline(CSV, acc.mname, ',');
			getline(CSV, acc.lname, ',');
			getline(CSV, bal);
			acc.bal = stof(bal);
			accs.insert(make_pair(accNum, acc));
			accNum = "";
			bal = "";
		}
	}
	
	CSV.close();
}

void memSave(string filename, string tableName) {
    // write to file code here
    
    // use this for loop iterator
    for (map<string, Account>::iterator itr = accs.begin(); itr != accs.end(); itr++) {
        cout << "Saving " << itr->first << "...\n";
    }
}

Account fetchAccount(string cardNum, string atmPIN) {
	Account acc;
	for (map<string, Account>::iterator itr = accs.begin(); itr != accs.end(); itr++) {
        if (itr->second.cardNum == cardNum) {
        	acc = itr->second;
		}
    }
    return acc;
}

string genAccNum() {
	// code here to generate account number for new customer
}

