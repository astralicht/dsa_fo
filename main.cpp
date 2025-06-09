#include <iostream>
#include <vector>
#include <map>
#include <cstring>

using namespace std;

struct Account {
	string num;
	string fname;
	string mname;
	string lname;
	float bal;
};

string progLoop(); // main program loop; put in main func
string genAccNum ();
void memLoad(map<string, Account>* accs);
void memSave(map<string, Account>* accs);

int main() {
	map<string, Account> accs;
	
	memLoad(&accs);
	
	cout << accs["101132857923"].fname << "\n";
	
	memSave(&accs);
}

void memLoad(map<string, Account>* accs) {
    // read from file code here
    
    accs->insert({"101132857922", {"101132857922", "bubu"}});
	accs->insert({"101132857923", {"101132857922", "baba"}});
	accs->insert({"101132857924", {"101132857922", "bebe"}});
}

void memSave(map<string, Account>* accs) {
    // write to file code here
    
    for (const auto& accPair : *accs) {
        cout << "Saving " << accPair.first << "...\n";
    }
}