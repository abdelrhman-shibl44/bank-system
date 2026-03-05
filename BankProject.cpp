#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include "./mylib.h"
using namespace std;

enum enOptions {SHOW = 1, ADD, DELETE, UPDATE, FIND, TRANSACTIONS, EXIST};
enum enTransactions {DEPOSIT = 1, WITHDRAW, TOTALBALANCE, MAINMENU};
struct stClientData {
    string accountNumber;
    string pinCode;
    string name;
    string phone;
    double Balance = 0;
    bool markforDeletion = false;
};

struct stUserData {
    string username;
    string password;
    string permissions;
};

const string fileName = "Clients.txt";

void printClientData(stClientData client){
    cout << "| " << left << setw(20) <<  client.accountNumber;
    cout << "| " << left << setw(10) <<  client.pinCode;
    cout << "| " << left << setw(35) <<  client.name;
    cout << "| " << left << setw(15) <<  client.phone;
    cout << "| " << left << setw(15) <<  client.Balance << endl;   
}

void header(string headerTxt){
    cout << "====================================================\n";
    cout << "=================" << headerTxt << "================\n";
}

stClientData convertLineToRecord(string line){
    stClientData clientData;
    vector<string> data = split(line, "#//#");
    clientData.accountNumber = data[0];
    clientData.pinCode = data[1];
    clientData.name = data[2];
    clientData.phone = data[3];
    clientData.Balance = stod(data[4]);
    return clientData;
}

stUserData convertUserLineToRecord(string line){
    stUserData userData;
    vector<string> user = split(line, "#//#");
    userData.username = user[0];
    userData.password = user[1];
    userData.permissions = user[2];
    return userData;   
}
vector<stClientData> readDataFromFile(string fileName){
    fstream file;
    vector<stClientData> vClients;
    file.open(fileName, ios::in);
    if(file.is_open()){
        string line;
        while(getline(file, line)){
            stClientData client =  convertLineToRecord(line);
            vClients.push_back(client);
        }
        file.close();
    }
    return vClients;
}

void printClientsData(){
    header("Show Client Data");
    vector<stClientData> vClients = readDataFromFile(fileName);
        cout << "| " << left << setw(20) << "Account Number";
        cout << "| " << left << setw(10) << "PinCode";
        cout << "| " << left << setw(35) << "Name";
        cout << "| " << left << setw(15) << "Phone";
        cout << "| " << left << setw(15) << "Balance";
        cout << "\n--------------------------------------------------------------------------------------------------\n";
    for(stClientData &client: vClients){
        printClientData(client);
        cout << endl;
    }
}

bool findClientByAccountNumber(string accountNumber,stClientData* clientData = nullptr){
    vector<stClientData> vClients = readDataFromFile(fileName);
    for(stClientData &client: vClients){
        if(convertTextToLower(client.accountNumber) == convertTextToLower(accountNumber)){
            header("Client Data");
            printClientData(client);
            cout << endl;
            if(clientData != nullptr){
                *clientData = client;
            }
            return true;
        }
    }
    return false;
}

void findClientScreen(){
    header("Find Client Screen");
    string accountNumber = readString("Please Enter the Account Number you're looking for? ");
    while(!findClientByAccountNumber(accountNumber)){
        cout << "Account Not Exist! :(" << endl;
        accountNumber = readString("Try Again with another Account Number? ");
    }

}

stClientData readClientData(stClientData &client, bool *isAdd = nullptr){
    if(*isAdd){
        client.accountNumber =  readString("Please Enter client account number? ");
        while(findClientByAccountNumber(client.accountNumber, &client)){
            cout << "The Account number is already exist, Please try another one\n";
            client.accountNumber =  readString("Please Enter client account number? ");
        }
    }
    client.pinCode =  readString("Please Enter client pinCode? ");
    client.name =  readString("Please Enter client name? ");
    client.phone =  readString("Please Enter client phone? ");
    client.Balance = readNumber("Please Enter client balance? ");
    return client;
}

string convertRecordToLine(stClientData clientData, string delim = "#//#"){
    return(
        clientData.accountNumber + delim +
        clientData.pinCode + delim +
        clientData.name + delim + 
        clientData.phone + delim +
        to_string(clientData.Balance)
    );
}

void saveLineToFile(string line){
    fstream file;
    file.open(fileName, ios::out | ios::app);
    if(file.is_open()){
        file << line << endl;
        file.close();
    }
}

void addNewClient(){
    bool isAdd = true;
    header("Add New Client :)");
    stClientData client;
    readClientData(client, &isAdd);
    saveLineToFile(convertRecordToLine(client));
    cout << "Client has been added successfully\n";
}

void saveClientsToFile(vector<stClientData> vClients){
    fstream file; 
    file.open(fileName, ios::out);
    if(file.is_open()){
        for(stClientData &client: vClients){
            if(!client.markforDeletion){
                string line = convertRecordToLine(client);
                file << line << endl;
            }
        }
        file.close();
    }
}

void markClientForDeletion(string accountNumber, vector<stClientData> &vClients){
    for(stClientData &client: vClients){
        if(convertTextToLower(client.accountNumber) == convertTextToLower(accountNumber)){
            client.markforDeletion = true;
        }
    }
}

void deleteClient(){
    string accountNumber = readString("please Enter Account number you want to delete? ");
    vector<stClientData> vClients = readDataFromFile(fileName); 
    stClientData client;
    if(findClientByAccountNumber(accountNumber, &client)){
        char answer = readChar("Are you sure you want to delete this account? \n");
        if(tolower(answer) == 'y'){
            markClientForDeletion(accountNumber,vClients);
            saveClientsToFile(vClients);
            cout << "Account Deleted successfully :)" << endl;
            vClients = readDataFromFile(fileName);
        }
    }else{
        cout << "The Account you're looking for not exist or already deleted";
    }
}

void updateClientData(string accountNumber, vector<stClientData> &vClients){
    bool isAdd = false;
    for(stClientData &client: vClients){
        if(convertTextToLower(client.accountNumber) == convertTextToLower(accountNumber)){
            client = readClientData(client, &isAdd);
        }
    }
}

void updateClient(){
    header("Update Client Data");
    string accountNumber = readString("please Enter Account number you want to update? ");
    vector<stClientData> vClients = readDataFromFile(fileName); 
    stClientData client;
    if(findClientByAccountNumber(accountNumber, &client)){
        char answer = readChar("Are you sure you want to update this account? \n");
        if(tolower(answer) == 'y'){
            updateClientData(accountNumber,vClients);
            saveClientsToFile(vClients);
            cout << "Account Updated successfully :)";
            vClients = readDataFromFile(fileName);
        }
    }else{
        cout << "The Account you're looking for not exist or deleted" << endl;
    }
}

void showMenuScreen();
bool showLoginScreen();

void backToMenu(){
    cout << "Press any key to go back to the Menu :)" << endl;
    system("pause>0");
    showMenuScreen();
}

void transaction(string accountNumber, vector<stClientData> &vClients, double amount){
    char answer = readChar("Are you sure you want to perform this transaction? ");
    if(tolower(answer) == 'y'){
        for(stClientData &client: vClients){
            if(convertTextToLower(client.accountNumber) == convertTextToLower(accountNumber)){  
                client.Balance = client.Balance + amount;
            }
        }
    }
}

void deposit(){
    header("Deposit Screen");
    vector<stClientData> vClients = readDataFromFile(fileName);
    string accountNumber = readString("please enter account number? ");
    stClientData client;
    if(findClientByAccountNumber(accountNumber, &client)){
        double depositAmount =  readNumber("Please Enter Deposit Amount? ");
        transaction(accountNumber, vClients,  depositAmount);
        saveClientsToFile(vClients);
        cout << "Deposit made successfully :)" << endl;
    } else{
        cout << "Account not exist try again :(" << endl;
    }
}

void withdraw(){
    header("Withdraw Screen");
    vector<stClientData> vClients = readDataFromFile(fileName);
    string accountNumber = readString("please enter account number? ");
    stClientData client;
    if(findClientByAccountNumber(accountNumber, &client)){
        double withdrawAmount =  readNumber("Please Enter Withdraw Amount? ");
        while(client.Balance < withdrawAmount){
            cout << "Exceed Amount you can withdraw up to " << client.Balance << endl;
            withdrawAmount = readNumber("Please Enter Withdraw Amount? ");
        }
        transaction(accountNumber, vClients,  withdrawAmount * - 1);
        saveClientsToFile(vClients);
        cout << "Withdraw made successfully :)" << endl;
    }else{
        cout << "Account not exist try again :(" << endl;
    }
}

void totalBalance(){
    header("Total Balance Screen");
    vector<stClientData> vClients = readDataFromFile(fileName);
    double totalBalance = 0;

    for(stClientData &client: vClients){
        totalBalance += client.Balance;
    }
    
    printClientsData();
    cout << "\n\t\t\t\tTotal Balance is: " << totalBalance << endl;;

}

void chooseTransaction(short number){
    switch ((enTransactions) number){
        case enTransactions::DEPOSIT :
            deposit();
            break;
        case enTransactions::WITHDRAW :
            withdraw();
            break;
        case enTransactions::TOTALBALANCE :
            totalBalance();
            break;
        case enTransactions::MAINMENU :
            showMenuScreen();
            break;
        
        default:
            break;
    }
}

void showTransactionMenu(){
    system("cls");
    cout <<"==================================================\n";
    cout << "\t\tTransactions Menu Screen\n";
    cout <<"==================================================\n";
    cout << "\t[1] Deposit.\n"
            "\t[2] Withdraw.\n"
            "\t[3] Total Balances.\n"
            "\t[4] Main Menu.\n";
    cout <<"==================================================\n";
    short chosenOption = readNumber("Choose what do you want to do? [1 to 4]?");
    chooseTransaction(chosenOption);
}

short chooseFromMenu(short number){
    stClientData clientData;
    switch ((enOptions) number){
        case enOptions::SHOW: 
            printClientsData();
            backToMenu();
            break;  
            case enOptions::ADD:
            addNewClient();
            backToMenu();
            break;
            case enOptions::FIND:
            findClientScreen();
            backToMenu();
            break;
            case enOptions::DELETE:
            deleteClient();
            backToMenu();
            break;
            case enOptions::UPDATE:
            updateClient();
            backToMenu();
            break;
            case enOptions::TRANSACTIONS:
            showTransactionMenu();
            backToMenu();
            break;
        default:
            break;
    }
}

void showMenuScreen(){
    system("cls");
    cout <<"==================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout <<"==================================================\n";
    cout << "\t[1] Show Client List.\n"
            "\t[2] Add New Client.\n"
            "\t[3] Delete Client.\n"
            "\t[4] Update Client Info.\n"
            "\t[5] Find Client.\n"
            "\t[6] Transactions.\n"
            "\t[7] Logout." << endl;
    cout <<"==================================================\n";
    short chosenOption = readNumber("Choose what do you want to do? [1 to 6]? ");
    if(chosenOption == enOptions::EXIST) return;
    chooseFromMenu(chosenOption);
}

vector<stUserData> getUsers(){
    fstream file;
    vector<stUserData> vUsers;
    file.open("users.txt", ios::in);
    if(file.is_open()){
        string line;
        while(getline(file, line)){
            stUserData userData =  convertUserLineToRecord(line);
            vUsers.push_back(userData);
        }
        file.close();
    }
    return vUsers;
}

bool isUserExist(string username, string password){
    vector<stUserData> users =  getUsers();
    for(stUserData &user: users){
        if(user.username == username && user.password == password){
            return true;
        }
    }
    return false;
}

bool showLoginScreen(){
    system("cls");
    header("Login Screen");
    do{
        string username = readString("Enter Username? ");
        string password = readString("Enter Password? ");
        if(isUserExist(username, password)){
            return true; 
        }else{
            cout << "Invalid Credential :(\n";
        }
    }while(true);
    return true;
}

int main(){
    while (showLoginScreen()){   
        showMenuScreen();
    }
    return 0;
}