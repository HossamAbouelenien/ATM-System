#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;


struct sClient {
    string accountNumber;
    string pinCode;
    string name;
    string phone;
    double accountBalance = 0.000000;
    bool markToDelete = false;

};

enum enMainMenuOption {
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposite = 3,
    eCheckBalance = 4,
    eLogout = 5,

};

const string clientFile = "myFile.txt";
sClient currentClient;


void login();
void showMainMenu();

vector<string> splitFunc(string s1, string delimiter = "#//#") {
    vector<string>vWords;
    int pos = 0;
    string word = "";
    while ((pos = s1.find(delimiter)) != string::npos) {
        word = s1.substr(0, pos);
        if (word != "") {
            vWords.push_back(word);
        }
            s1.erase(0, pos + delimiter.length());
    }
    if (s1 != "") {
        vWords.push_back(s1);
    }
    return vWords;
}

sClient convertStringLineToClientRecord(string line) {
    sClient client;
    vector<string>vInfo;
    vInfo = splitFunc(line, "#//#");
    client.accountNumber = vInfo[0];
    client.pinCode = vInfo[1];
    client.name = vInfo[2];
    client.phone = vInfo[3];
    client.accountBalance = stod(vInfo[4]);
   
    return client;
}

string convertClientRecordToLine(sClient client , string delimiter = "#//#") {
    string line = "";
    line += client.accountNumber + delimiter;
    line += client.pinCode + delimiter;
    line += client.name + delimiter;
    line += client.phone + delimiter;
    line += to_string(client.accountBalance);
    return line;
}

vector<sClient> loadDataFromClientFile(string clientFile) {
    vector<sClient>vClients;
    sClient client;
    fstream file;
    file.open(clientFile, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            client = convertStringLineToClientRecord(line);
            vClients.push_back(client);
        }
        file.close();
    }


    return vClients;
}

void saveChangesInClientFile(vector<sClient>&vClients) {
    fstream file;
    file.open(clientFile, ios::out);
    if (file.is_open()) {
        string line;
        for (sClient& c : vClients) {
            if (c.markToDelete == false) {
                line = convertClientRecordToLine(c);
                file << line << endl;
            }


        }
        file.close();
    }
}

bool deposite(string accountNumber, double depositeAmount, vector<sClient>&vClients) {
    char answer = 'n';
    cout << "\nAre you sure you want to perform this transaction ? y/n  ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        for (sClient& c : vClients) {
            if (c.accountNumber == accountNumber) {
                c.accountBalance += depositeAmount;
                saveChangesInClientFile(vClients);
                cout << "\nTransaction has done successfully " << endl;
                return true;
            }
        }
    }
    return false;
  
}

int readOptionOfQuickWithdraw() {
    int option;
    do {
        cout << "\nEnter How much do you want to withdraw [ 1 - 9 ] : ";
        cin >> option;
    } while (option < 0 || option > 9);

    return option;
}

int amountOfWithdraw(int option) {
    switch (option) {
    case(1):
        return 20;
    case(2):
        return 50;
    case(3):
        return 100;
    case(4):
        return 200;
    case(5):
        return 400;
    case(6):
        return 600;
    case(7):
        return 800;
    case(8):
        return 1000;

    }
}

void performQuickWithdraw(int option) {

    cout << "\nYour Balance is : " << currentClient.accountBalance << endl;


    if (option == 9) {
        return;
    }

    int withdrawAmount = amountOfWithdraw(option);
    vector<sClient>vClients = loadDataFromClientFile(clientFile);
    if (deposite(currentClient.accountNumber, withdrawAmount * -1, vClients)) {
    currentClient.accountBalance -= withdrawAmount;
    }
    

}

void showQuickWithdraw() {
    system("cls");
    cout << "========================================" << endl;
    cout << "\t\tNormal Withdraw Screen." << endl;
    cout << "========================================" << endl;
    cout << "\t[1]20\t\t[2]50" << endl;
    cout << "\t[3]100\t\t[4]200" << endl;
    cout << "\t[5]400\t\t[6]600" << endl;
    cout << "\t[7]800\t\t[8]1000" << endl;
    cout << "\t[9]Exit\t\t" << endl;
    performQuickWithdraw(readOptionOfQuickWithdraw());
}


void performNormalWithdraw() {

    int withdrawAmount;
    cout << "Your Balance is : " << currentClient.accountBalance << endl;
    cout << "\nEnter How much do you want to Withdraw : ";
    cin >> withdrawAmount;
    while (withdrawAmount > currentClient.accountBalance) {
        cout << "\nInvalid transaction , you must enter amount less than your balance " << currentClient.accountBalance << endl;
        cout << "\nEnter How much do you want to Withdraw : ";
        cin >> withdrawAmount;
    }
    vector<sClient>vClients = loadDataFromClientFile(clientFile);
    if (deposite(currentClient.accountNumber, withdrawAmount * -1, vClients)) {
        currentClient.accountBalance -= withdrawAmount;
    }

}

void showNormalWithdrawScreen() {
    system("cls");
    cout << "========================================" << endl;
    cout << "\tNormal Withdraw Screen." << endl;
    cout << "========================================" << endl;
    performNormalWithdraw();


}

void performDeposite() {
    int depositeAmount;
    cout << "Your Balance is : " << currentClient.accountBalance << endl;
    cout << "\nEnter Positive number to deposite : ";
    cin >> depositeAmount;
    while (depositeAmount < 0) {
        cout << "\nEnter Positive number to deposite : ";
        cin >> depositeAmount;
    }
    vector<sClient>vClients = loadDataFromClientFile(clientFile);
    if (deposite(currentClient.accountNumber, depositeAmount, vClients)) {
    currentClient.accountBalance += depositeAmount;
    }
    
}

void showDepositeScreen() {
    system("cls");
    cout << "========================================" << endl;
    cout << "\tDeposite Screen." << endl;
    cout << "========================================" << endl;
    performDeposite();
}

void showCheckBalanceScreen() {
    system("cls");
    cout << "========================================" << endl;
    cout << "\tCheck Balance Screen." << endl;
    cout << "========================================" << endl;
    cout << "\nFull Balance is : " << currentClient.accountBalance << endl;
    cout << "\n";
}

int readMainMenuOption() {
    int choice;

    cout << "\nChoose What do you want to do [ 1 - 5 ] : ";
    cin >> choice;

    while (choice < 0 || choice > 5) {
        cout << "\nChoose What do you want to do [ 1 - 5 ] : ";
        cin >> choice;
    }

    return choice;
}


void backToMainMenu() {
    cout << "\npress key to back to main menu ....." << endl;
    system("pause > 0");
    showMainMenu();
}

void performMainMenuOption(enMainMenuOption mainMenuOption) {
    switch (mainMenuOption) {

    case(enMainMenuOption::eQuickWithdraw): {
        showQuickWithdraw();
        backToMainMenu();
        break;
    }
    case(enMainMenuOption::eNormalWithdraw): {
        showNormalWithdrawScreen();
        backToMainMenu();
        break;
    }

    case(enMainMenuOption::eDeposite): {
        showDepositeScreen();
        backToMainMenu();
        break;
    }

    case(enMainMenuOption::eCheckBalance): {
        showCheckBalanceScreen();
        backToMainMenu();
        break;
    }

    case(enMainMenuOption::eLogout): {
        login();
        break;
    }




    }
}

void showMainMenu() {
    system("cls");
    cout << "========================================" << endl;
    cout << "\tATM Main Menu Screen." << endl;
    cout << "========================================" << endl;
    cout << "\n\t[1]Quick Withdraw. " << endl;
    cout << "\t[2]Normal Withdraw. " << endl;
    cout << "\t[3]Deposite. " << endl;
    cout << "\t[4]Check Balance. " << endl;
    cout << "\t[5]Logout. " << endl;
    performMainMenuOption((enMainMenuOption)readMainMenuOption());
}


bool isClientFoundedByAccountNumberAndPinCode(string accountNumber, string pinCode, sClient& client) {
    vector<sClient>vClients = loadDataFromClientFile(clientFile);
    for (sClient& c : vClients) {
        if (c.accountNumber == accountNumber && c.pinCode == pinCode) {
            client = c;
            return true;
        }
    }
    return false;
}

bool ckeckClientInfo(string accountNumber, string pinCode) {
    if (isClientFoundedByAccountNumberAndPinCode(accountNumber, pinCode, currentClient)) {
        return true;
    }
    else {
        return false;
    }


}

void login() {
    system("cls");
    cout << "========================================" << endl;
    cout << "\tLogin Screen." << endl;
    cout << "========================================" << endl;
    bool loginFailed = false;
    string accountNumber, pinCode;
    do {
        if (loginFailed) {
            cout << "\nInvailed accountNumber / PinCode .\n";
        }
        cout << "\nEnter accountNumber : ";
        cin >> accountNumber;
        cout << "\nEnter pinCode : ";
        cin >> pinCode;

        loginFailed = !ckeckClientInfo(accountNumber, pinCode);

    } while (loginFailed);
    showMainMenu();
}

int main(){

    login();
    system("pause > 0");
    return 0;
}

