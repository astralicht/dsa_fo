#include <iostream>
#include <map>
#include <cstring>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

struct Account {
	string num, fname, mname, lname;
	float bal;
};

string progLoop(); // main program loop; put in main func
string genAccNum ();
void memLoad(map<string, Account>* accs);
void memSave(map<string, Account>* accs);
void readCSVToMap(string filename, map<string, Account>* accs);

int main() {
	map<string, Account> accs;
	
	memLoad(&accs);
	
	Account acc = accs["111"];
	
	cout << acc.fname << endl;
}

void memLoad(map<string, Account>* accs) {
    // read from file code here
	readCSVToMap("accounts.csv", accs);
}

void memSave(map<string, Account>* accs) {
    // write to file code here
    
    // use this for loop iterator
    for (map<string, Account>::iterator itr = accs->begin(); itr != accs->end(); itr++) {
        cout << "Saving " << itr->first << "...\n";
    }
}

void readCSVToMap(string filename, map<string, Account>* accs) {
	string line, str, num;
	ifstream CSV(filename);
	Account acc;
	string bal;
	
	while (getline(CSV, num, ',')) {
		acc = Account();
		acc.num = num;
		getline(CSV, acc.fname, ',');
		getline(CSV, acc.mname, ',');
		getline(CSV, acc.lname, ',');
		getline(CSV, bal);
		acc.bal = stof(bal);
		accs->insert(make_pair(num, acc));
		num = "";
		bal = "";
	}
	
	CSV.close();
}
