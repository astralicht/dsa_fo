/*
CS105-1

Julac Ontina
Jericho Capistrano
Ricoh Ng

Please compile with C++20
*/

#include <iostream>
#include <map>
#include <cstring>
#include <cctype>
#include <fstream>
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

void header();
void viewAccount(Account acc);
Account fetchAccount(string accNum);
void printRecentTransactions(int count, string accNum);
void refreshAccBal(string accNum);
void depositAmtToAcc(float amt, string accNum);
void withdrawAmtFromAcc(float amt, string accNum);
void openAccount(Customer cs);
void bankTransferOut(string destNum, float amt);
void bankTransferIn(string destNum, float amt);
Customer fetchCustomer(string cusNum);
string genAccNum();
string genCusNum();
float calcMonthlyInterest(float monthlySum, int timePeriod);
void generateMonthlyReport(int month, int year, string accNum);

map<string, Customer> css;
map<string, Account> acs;
map<string, Transaction> trs;
map<string, int> glbl_ctrs;

const string months[] = {
	"JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE",
	"JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"
};

int main() {
	memLoad("customers.txt", "customers");
	memLoad("accounts.txt", "accounts");
	memLoad("transactions.txt", "transactions");
	memLoad("glbl_ctrs.txt", "glbl_ctrs");
	progLoop();
	return 0;
}

void progLoop() {
	srand(time(0));
	
	string input;
	string accNumInput;
	bool errFlag = false;
	
	while(true) {
		system("cls");
		system("clear");
		header();

		errFlag = false;
		
	 	cout << "MAIN MENU\n";
	 	cout << "1. View Account\n";
		cout << "2. Open New Account\n";
	 	cout << "3. View Customer\n";
		cout << "4. Create New Customer\n";
	 	cout << "> ";
	 	cin >> input;
	 	
	 	for (auto& x : input) x = toupper(x);
	 	
	 	if (input == "1") {
			Account acc;
	 		while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag) {
					cout << "ERR: Account not found!\n";
				}
				cout << "Enter Account Number\n";
				cout << "> ";
				cin >> input;
			
				for (auto& x : input) x = toupper(x);
				
				acc = fetchAccount(input);
				if (acc.accNum == "") {
					errFlag = true;
					continue;
				} else break;
			}
			
			viewAccount(acc);
	    } else if (input == "2") {
			Customer cs;
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag) {
					cout << "ERR: Customer not found!\n";
				}
				cout << "Open New Account\n";
				cout << "Enter customer number\n";
				cout << "'000' to cancel\n";
				cout << "> ";
				cin >> input;
				if (input == "000") break;
				for (auto& x : input) x = toupper(x);
				cs = fetchCustomer(input);
				if (cs.cusNum == "") {
					errFlag = true;
					continue;
				} else break;
			}
			
			openAccount(cs);
			
			system("cls");
			system("clear");
			header();
			cout << "Successfully opened a new account for customer " << cs.cusNum << "!\n";
			cout << "Input any key to continue\n";
			cin >> input;
			continue;
		} else if (input == "3") {
			Customer cs;
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag) {
					cout << "ERR: Customer not found!\n";
				}
				cout << "View Customer Details\n";
				cout << "Enter customer number\n";
				cout << "'000' to cancel\n";
				cout << "> ";
				cin >> input;
				if (input == "000") break;
				for (auto& x : input) x = toupper(x);
				cs = fetchCustomer(input);
				if (cs.cusNum == "") {
					errFlag = true;
					continue;
				} else break;
			}
			system("cls");
			system("clear");
			header();
			cout << "Customer Details\n\n";
			cout << "Customer #" << cs.cusNum << endl;
			cout << "Name: " << cs.lname << ", " << cs.fname << " " << cs.mname << endl;
			cout << "Email: " << cs.email << endl;
			cout << "Phone Number: " << cs.phone_number << endl;
			cout << "\nEnter any key to return to main menu\n";
			cout << "> ";
			cin >> input;
		} else if (input == "4") {
			Customer cs;

			system("cls");
			system("clear");
			header();
			cout << "Enter first name\n";
			cout << "'000' to cancel\n";
			cout << "> ";
			cin >> input;
			for (auto& x : input) x = toupper(x);
			if (input == "000") return;
			cs.fname = input;

			system("cls");
			system("clear");
			header();
			cout << "Enter middle name\n";
			cout << "'000' to cancel\n";
			cout << "> ";
			cin >> input;
			for (auto& x : input) x = toupper(x);
			if (input == "000") return;
			cs.mname = input;

			system("cls");
			system("clear");
			header();
			cout << "Enter last name\n";
			cout << "'000' to cancel\n";
			cout << "> ";
			cin >> input;
			for (auto& x : input) x = toupper(x);
			if (input == "000") return;
			cs.lname = input;

			system("cls");
			system("clear");
			header();
			cout << "Enter email\n";
			cout << "'000' to cancel\n";
			cout << "> ";
			cin >> input;
			for (auto& x : input) x = toupper(x);
			if (input == "000") return;
			cs.email = input;

			while (true) {
				system("cls");
				system("clear");
				header();
				cout << "Enter phone number [+63] ";
				cout << "'000' to cancel\n";
				cin >> input;

				if (input == "000") return;

				if (input.length() == 10) {
					bool validPhone = true;
					for (size_t i = 0; i < input.length(); i++) {
						if (!isdigit(input[i])) {
							validPhone = false;
							break;
						}
					}

					if (validPhone) {
						cs.phone_number = "+63" + cs.phone_number;
						break;
					} else {
						cout << "Invalid phone number! Please ensure it is 10 digits long and contains only numbers." << endl;
						continue;
					}
				}
				else {
					cout << "Invalid phone number length! It should be exactly 10 digits." << endl;
					continue;
				}
			}

			cs.cusNum = genCusNum();
			css[cs.cusNum] = cs;
			memSave("customers.txt", "customers");
		}
		else {
			errFlag = false;	
		}
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
	string input;
	int errFlag = 0;
	bool otherBankFlag = false;
	
	while (true) {
		system("cls");
		system("clear");
		header();
		cout << "Account Number: " << acc.accNum << endl;
		cout << "Customer Name: " << cus.lname << ", " << cus.fname << " " << cus.mname << endl;
		cout << "\nBalance\n";
		cout << "---------------\n";
		cout << "Php " << fixed << setprecision(2) << acc.balance << endl;
		cout << "\nMost Recent Transactions\n";
		cout << "---------------\n";
		printRecentTransactions(5, acc.accNum);
		cout << "\nOptions\n";
		cout << "---------------\n";
		cout << "1. Deposit\n";
		cout << "2. Withdraw\n";
		cout << "3. Bank Transfer\n";
		cout << "4. View All Transactions\n";
		cout << "5. Generate Report\n";
		cout << "R. Refresh\n";
		cout << "0. to exit account\n";
		cout << "> ";
		cin >> input;
		
		for (auto& x : input) x = toupper(x);
		
		if (input == "0") break;
		else if (input == "1") {
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag == 1) {
					cout << "ERR: Amount must be more than 0";
					errFlag = 0;
				}
				cout << "Deposit\n";
				cout << "'000' to cancel\n";
				cout << "Enter amount: ";
				cin >> input;
				if (input == "000") break;
				
				float amt = stof(input);
				if (amt <= 0) {
					errFlag = 1;
					continue;
				} else {
					system("cls");
					system("clear");
					header();
					depositAmtToAcc(amt, acc.accNum);
					cout << "An amount of Php " << setprecision(2) << amt << " was successfully deposited into the account.\n";
					cout << "Input any key to continue\n";
					cout << "> ";
					cin >> input;
					errFlag = 0;
					break;
				}
			}
		} else if (input == "2") {
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag == 1) {
					cout << "ERR: Amount cannot be more than the available balance\n";
					errFlag = 0;
				}
				cout << "Withdraw\n";
				cout << "'000' to cancel\n";
				cout << "Enter amount: ";
				cin >> input;
				if (input == "000") break;
				
				float amt = stof(input);
				if (amt > acc.balance) {
					errFlag = 1;
					continue;
				} else {
					system("cls");
					system("clear");
					header();
					withdrawAmtFromAcc(amt, acc.accNum);
					cout << "An amount of Php " << setprecision(2) << amt << " was successfully withdrawn from the account.\n";
					cout << "Input any key to continue\n";
					cout << "> ";
					cin >> input;
					errFlag = 0;
					break;
				}
			}
		} else if (input == "3") {
			float amt;
			string destNum;
			while (true) {
				system("cls");
				system("clear");
				header();
				cout << "Bank Transfer\n";
				cout << "'000' to cancel\n";
				cout << "Funds transfer same bank? [y/n]\n";
				cout << "> ";
				cin >> input;

				for (auto& x : input) x = toupper(x);

				if (input == "000") break;
				else if (input == "N") {
					otherBankFlag = true;
					amt += 50;
					break;
				} else if (input == "Y") {
					break;
				}
			}
			errFlag = false;
			if (input == "000") continue;
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag) cout << "No such account or same as origin account.\n";
				cout << "Enter account number\n";
				cout << "'000' to cancel\n";
				cout << "> ";
				cin >> input;

				for (auto& x : input) x = toupper(x);

				if (input == "000") break;
				destNum = fetchAccount(input).accNum;
				if (otherBankFlag == true) break;

				if (destNum == "" || destNum == acc.accNum) {
					errFlag = true;
					continue;
				} else break;
			}
			errFlag = false;
			if (input == "000") continue;
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag) cout << "Amount cannot be negative or more than the available balance!\n";
				cout << "Enter amount\n";
				cout << "'000' to cancel\n";
				cout << "> ";
				cin >> input;

				if (input == "000") break;
				else if (stof(input) < 0 || stof(input) > acc.balance) {
					errFlag = true;
					continue;
				}

				amt = stof(input);
				break;
			}
			errFlag = false;
			if (input == "000") continue;
			bankTransferOut(acc.accNum, amt);
			if (otherBankFlag == false) {
				bankTransferIn(destNum, amt);
			}
		} else if (input == "4") {
			system("cls");
			system("clear");
			header();
			cout << "Transaction #\t | Account #\t | Datetime\t | Type\t | Amount\n";
			for (pair pr : trs | views::reverse) {
				Transaction tr = pr.second;
				if (tr.accNum != acc.accNum) continue;
				cout << tr.transId << "\t"
					<< tr.accNum << "\t"
					<< tr.datetime << "\t"
					<< tr.type << "\t"
					<< tr.amount << "\n";
			}
			cout << "\nInput any key to continue\n";
			cout << "> ";
			cin >> input;
		} else if (input == "5") {
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag) {
					cout << "Please enter valid month\n";
				}
				cout << "Generate Report/Statement of Account\n";
				cout << "Enter month\n";
				cout << "'000' to cancel\n";
				cout << "> ";
				cin >> input;

				if (input == "000") break;
				if (stoi(input) < 1 || stoi(input) > 12) errFlag = true;
				else break;
			}
			if (input == "000") continue;
			int month = stoi(input);
			errFlag = false;
			while (true) {
				system("cls");
				system("clear");
				header();
				if (errFlag) {
					cout << "Please enter valid year\n";
				}
				cout << "Generate Report/Statement of Account\n";
				cout << "Enter year\n";
				cout << "'000' to cancel\n";
				cout << "> ";
				cin >> input;

				if (input == "000") break;
				if (stoi(input) < 1900) errFlag = true;
				else break;
			}
			if (input == "000") continue;
			int year = stoi(input);
			errFlag = false;
			generateMonthlyReport(month, year, acc.accNum);
		} else if (input == "R") {
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

Customer fetchCustomer(string cusNum) {
	if (css.count(cusNum)) return css[cusNum];
	else return Customer(); 
}

string genCusNum() {
	// customer number format: AC-YYYY-00000000
	while (true) {
		const auto now = chrono::system_clock::now();
		time_t now_c = chrono::system_clock::to_time_t(now);
		tm* ltm = localtime(&now_c);
		string year = to_string(ltm->tm_year + 1900);
		
		ostringstream oss;
		oss << right << setfill('0') << setw(8) << to_string(glbl_ctrs["CS"]);
		
		string num = "CS-" + year + "-" + oss.str();
		
		try {
			css.at(num);
			glbl_ctrs["CS"]++;
		} catch (const out_of_range& e) {
			glbl_ctrs["CS"]++;
			memSave("glbl_ctrs.txt", "glbl_ctrs");
			return num;
		}
	}
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
		
		string num = "AC-" + year + "-" + oss.str();
		
		try {
			acs.at(num);
			glbl_ctrs["AC"]++;
		} catch (const out_of_range& e) {
			glbl_ctrs["AC"]++;
			memSave("glbl_ctrs.txt", "glbl_ctrs");
			return num;
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
		
		string num = "TR-" + year + "-" + oss.str();
		
		try {
			trs.at(num);
			glbl_ctrs["TR"]++;
		} catch (const out_of_range& e) {
			glbl_ctrs["TR"]++;
			memSave("glbl_ctrs.txt", "glbl_ctrs");
			return num;
		}
	}
}

void memSave(string filepath, string tableName) {
    ofstream ofs(filepath);
    string input;

    if (!ofs.is_open()) {
        system("cls");
		system("clear");
        header();
        cout << "System error: Could not open " << filepath << " for writing!\n";
        cout << "Please contact the system administrator to resolve the issue.\n";
        cout << "The program will exit now.\n";
        cout << "Input any key to continue\n";
		cout << "> ";
		cin >> input;
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
		system("clear");
		header();
		cout << "System error!\n";
		cout << "Please contact the system administrator to resolve the issue.\n";
		cout << "The program will exit now.\n";
		cout << "Input any key to continue\n";
		cout << "> ";
		cin >> line;
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
	} else if (mapName == "glbl_ctrs") {
		string code, c;
		while (getline(ifs, code, ',')) {
			getline(ifs, c);
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
		if (i == count+1) break;
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
			if (tr.type == "DEPOSIT" || tr.type == "BANK TRANSFER - IN") bal += tr.amount;
			else if (tr.type == "WITHDRAW" || tr.type == "BANK TRANSFER - OUT") bal -= tr.amount;
		}
	}
	acs[accNum].balance = bal;
	memSave("accounts.txt", "accounts");
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
	memSave("transactions.txt", "transactions");
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
	memSave("transactions.txt", "transactions");
	return;
}

void bankTransferOut(string destNum, float amt) {
	Transaction tr = Transaction();
	tr.transId = genTransId();
	tr.accNum = destNum;
	tr.amount = amt;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ostringstream oss;
	oss << put_time(ltm, "%m/%d/%Y %H:%M:%S");
	tr.datetime = oss.str();
	tr.type = "BANK TRANSFER - OUT";
	trs[tr.transId] = tr;
	memSave("transactions.txt", "transactions");
	return;
}

void bankTransferIn(string origNum, float amt) {
	Transaction tr = Transaction();
	tr.transId = genTransId();
	tr.accNum = origNum;
	tr.amount = amt;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ostringstream oss;
	oss << put_time(ltm, "%m/%d/%Y %H:%M:%S");
	tr.datetime = oss.str();
	tr.type = "BANK TRANSFER - IN";
	trs[tr.transId] = tr;
	memSave("transactions.txt", "transactions");
	return;
}

void openAccount(Customer cs) {
	Account ac = Account();
	ac.accNum = genAccNum();
	ac.cusNum = cs.cusNum;
	acs[ac.accNum] = ac;
	memSave("accounts.txt", "accounts");
}

float calcMonthlyInterest(float monSum) {
	int daysPerMonth = 30;
	float intRate = 0.04;
	float withholdingTax = 0.2;
	float adb = (monSum / daysPerMonth); // avergae daily balance
	float monInt = adb * daysPerMonth * intRate;
	return monInt - (withholdingTax * monInt);
}

void generateMonthlyReport(int month, int year, string accNum) {
	float depositAmt, withdrawAmt, transferInAmt, transferOutAmt, monthlySum, monthlyInterestAmt;
	bool interestFlag = false;

	for (pair pr : trs) {
		Transaction tr = pr.second;

		if (tr.accNum != accNum) continue;

		tm tm = {};
		istringstream ss(tr.datetime);
		ss >> get_time(&tm, "%m/%d/%Y");

		if ((tm.tm_year + 1900) != year || (tm.tm_mon + 1) != month) continue;

		if (tr.type == "DEPOSIT") depositAmt += tr.amount;
		else if (tr.type == "WITHDRAW") withdrawAmt += tr.amount;
		else if (tr.type == "BANK TRANSFER - IN") transferInAmt += tr.amount;
		else if (tr.type == "BANK TRANSFER - OUT") transferOutAmt += tr.amount;
		else if (tr.type == "INTEREST") interestFlag = true;
	}

	monthlySum += depositAmt - (withdrawAmt + transferOutAmt);
	monthlyInterestAmt = calcMonthlyInterest(monthlySum);

	// save to file; format: Report-mm-yyyy-timestamp.txt
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ostringstream oss;
	oss << put_time(ltm, "%m_%d_%Y-%H_%M_%S");
	string filepath = "ACC-STATEMENT-" + months[month-1] + "-" + to_string(year) + "-" + oss.str() + ".txt";
	ofstream ofs(filepath);
	ofs << "Generated Report for " << months[month-1] << " " << year << endl
		<< "---------------\n"
		<< "Deposits | Php " << depositAmt << endl
		<< "Withdrawals | Php " << withdrawAmt << endl
		<< "Bank Transfer - Out | Php " << to_string(transferOutAmt) << endl
		<< "---------------\n"
		<< "Interest Earned | Php " << monthlyInterestAmt << endl
		<< "---------------\n"
		<< "Ending Balance for this month:\n"
		<< "Php " << depositAmt - (withdrawAmt + transferOutAmt) + monthlyInterestAmt << endl;

	system("cls");
	system("clear");
	header();
	cout << "Generated Report for " << months[month-1] << " " << year << endl;
	cout << "---------------\n";
	cout << "Deposits | Php " << depositAmt << endl;
	cout << "Withdrawals | Php " << withdrawAmt << endl;
	cout << "Bank Transfer - Out | Php " << to_string(transferOutAmt) << endl;
	cout << "---------------\n";
	cout << "Interest Earned | Php " << monthlyInterestAmt << endl;
	cout << "---------------\n";
	cout << "Ending Balance for this month:\n";
	cout << "Php " << depositAmt - (withdrawAmt + transferOutAmt) + monthlyInterestAmt << endl;
	cout << "\nInput any key to go back to main menu\n";
	cout << "> ";

	if (interestFlag == false) {
		Transaction tr = Transaction();
		tr.transId = genTransId();
		tr.accNum = accNum;
		tr.type = "INTEREST";
		tr.amount = monthlyInterestAmt;

		ostringstream oss2;
		oss2 << put_time(ltm, "%m/%d/%Y %H:%M:%S");
		tr.datetime = oss.str();

		trs[tr.transId] = tr;
		memSave("transactions.txt", "transactions");
	}

	string input;
	cin >> input;

	return;
}