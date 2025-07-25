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
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <sstream>

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
void depositAmtToAcc(float amt, string accNum);
void withdrawAmtFromAcc(float amt, string accNum);

map<string, Customer> css;
map<string, Account> acs;
map<string, Transaction> trs;
map<string, int> glbl_ctrs;

int main() {
	memLoad("csv\\customers.csv", "customers");
	memLoad("csv\\accounts.csv", "accounts");
	memLoad("csv\\transactions.csv", "transactions");
	memLoad("csv\\glbl_ctrs.csv", "global_counters");
	
	progLoop();
	return 0;
}

void progLoop() {
	cout << glbl_ctrs.at("TR");
    return;
    
	srand(time(0));
	
	char inp;
	string accNumInput;
	
	system("cls");
	header();
	cout << "Press any key to start";
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
			cout << "Account not found!\n";
			cout << "Press any key to continue";
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
	return;
}

void viewAccount(Account acc) {
	Customer cus = css[acc.cusNum];
	char inp;
	string input;
	
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
		cout << "\nOptions\n";
		cout << "---------------\n";
		cout << "1. Deposit\n";
		cout << "2. Withdraw\n";
		cout << "3. Bank Transfer\n";
		cout << "R. Refresh\n\n";
		cout << "ESC to exit account";
		inp = _getch();
		if (inp == ASCII_ESC) break;
		else if (inp == '1') {
			while (true) {
				system("cls");
				header();
				cout << "Deposit\n";
				cout << "'000' to cancel\n";
				cout << "Enter amount: ";
				cin >> input;
				if (input == "000") break;
				
				float amt = stof(input);
				if (amt <= 0) {
					cout << "\nAmount must be more than 0.\nPress any key to try again.\n";
					_getch();
					continue;
				} else {
					system("cls");
					header();
					depositAmtToAcc(amt, acc.accNum);
					cout << "An amount of Php " << setprecision(2) << amt << " was successfully deposited into the account.\n";
					cout << "Press any key to return to account menu";
					_getch();
					break;
				}
			}
		} else if (tolower(inp) == '2') {
			while (true) {
				system("cls");
				header();
				cout << "Withdraw\n";
				cout << "'000' to cancel\n";
				cout << "Enter amount: ";
				cin >> input;
				if (input == "000") break;
				
				float amt = stof(input);
				if (amt > acc.balance) {
					cout << "\nAmount cannot be more than the available balance.\nPress any key to try again.\n";
					_getch();
					continue;
				} else {
					system("cls");
					header();
					withdrawAmtFromAcc(amt, acc.accNum);
					cout << "An amount of Php " << setprecision(2) << amt << " was successfully withdrawn from the account.\n";
					cout << "Press any key to return to account menu";
					_getch();
					break;
				}
			}
		} else if (tolower(inp) == 'r') {
			refreshAccBal(acc.accNum);
			acc = acs[acc.accNum];
		} else continue;
		
		refreshAccBal(acc.accNum);
		acc = acs[acc.accNum];
	}
}

Account fetchAccount(string accNum) {
	if (acs.count(accNum)) return acs[accNum];
	else return Account(); 
}

string genAccNum() {
	// account number format: AC-YYYY-00000000
	while (true) {
		const auto now = chrono::system_clock::now();
		time_t now_c = chrono::system_clock::to_time_t(now);
		tm* ltm = localtime(&now_c);
		string year = to_string(ltm->tm_year + 1900);
		
		ostringstream oss;
		oss << right << setfill('0') << setw(8) << to_string(glbl_ctrs["AC"]);
		
		string transId = "AC-" + year + "-" + oss.str();
		
		try {
			trs.at(transId);
		} catch (const out_of_range& e) {
			glbl_ctrs["AC"]++;
			memSave("csv\\glbl_ctrs.csv", "global_counters");
			return transId;
		}
	}
}

string genTransId() {
	// transaction id  number format: TR-YYYY-00000000
	while (true) {
		const auto now = chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm* ltm = std::localtime(&now_c);
		string year = to_string(ltm->tm_year + 1900);
		
		ostringstream oss;
		oss << right << setfill('0') << setw(8) << to_string(glbl_ctrs["TR"]);
		
		string transId = "TR-" + year + "-" + oss.str();
		
		try {
			trs.at(transId);
		} catch (const out_of_range& e) {
			glbl_ctrs["TR"]++;
			memSave("csv\\glbl_ctrs.csv", "global_counters");
			return transId;
		}
	}
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
    } else if (tableName == "glbl_ctrs") {
        for (const auto& pair : glbl_ctrs) {
            ofs << pair.first << "," << pair.second << "\n";
        }
    }
    ofs.close();
    return;
}


void memLoad(string filepath, string mapName) {
	string line, str, cusNum, accNum, transId, balance, amountStr; 
	ifstream ifs(filepath);
	
	if (!ifs.is_open()) {
		system("cls");
		header();
		cout << "System error!\n";
		cout << "Please contact the system administrator to resolve the issue.\n";
		cout << "The program will exit now.";
		_getch();
		exit(0);
	}
	
	if (mapName == "customers") {
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
	} else if (mapName == "accounts") {
		Account acc;
		while (getline(ifs, accNum, ',')) {
			acc = Account();
			acc.accNum = accNum;
			getline(ifs, acc.cusNum, ',');
			getline(ifs, balance); 
			acc.balance = stof(balance);
			acs[accNum] = acc;
		}
	} else if (mapName == "transactions") {
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
	} else if (mapName == "global_counters") {
		string code, c;
		while (getline(ifs, code, ',')) {
			getline(ifs, c, ',');
			glbl_ctrs[code] = stoi(c);
		}
	}
	ifs.close();
	return;
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
	return;
}

void refreshAccBal(string accNum) {
    float bal = 0;
	Transaction tr;
	for (const auto& pair : trs | views::reverse) {
		tr = pair.second;
		if (tr.accNum == accNum) {
			if (tr.type == "DEPOSIT" || tr.type == "BANK TRANSFER") bal += tr.amount;
			else if (tr.type == "WITHDRAW") bal -= tr.amount;
		}
	}
	acs[accNum].balance = bal;
	return;
}

void depositAmtToAcc(float amt, string accNum) {
	Transaction tr = Transaction();
	tr.transId = genTransId();
	tr.accNum = accNum;
	tr.amount = amt;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ostringstream oss;
	oss << put_time(ltm, "%m/%d/%Y %H:%M:%S");
	tr.datetime = oss.str();
	tr.type = "DEPOSIT";
	trs[tr.transId] = tr;
	memSave("csv\\transactions.csv", "transactions");
	return;
}

void withdrawAmtFromAcc(float amt, string accNum) {
	Transaction tr = Transaction();
	tr.transId = genTransId();
	tr.accNum = accNum;
	tr.amount = amt;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ostringstream oss;
	oss << put_time(ltm, "%m/%d/%Y %H:%M:%S");
	tr.datetime = oss.str();
	tr.type = "WITHDRAW";
	trs[tr.transId] = tr;
	memSave("csv\\transactions.csv", "transactions");
	return;
}
