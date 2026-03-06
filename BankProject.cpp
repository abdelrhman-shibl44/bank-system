#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include "./mylib.h"
using namespace std;

enum enOptions {SHOW = 1, ADD, DELETE, UPDATE, FIND, TRANSACTIONS, MANAGE_USERS, EXIST};
enum enTransactions {DEPOSIT = 1, WITHDRAW, TOTALBALANCE, MAINMENU};
enum enUsersOptions {SHOW_USER = 1, ADD_USER, DELETE_USER, UPDATE_USER, FIND_USER, USERS_EXIST};
enum enUserPermissions {
    ALL_P = -1, P_LIST_CLIENTS = 1, P_ADD_CLIENTS = 2, P_DELETE_CLIENTS = 4,
    P_UPDATE_CLIENTS = 8, P_FIND_CLIENTS = 16 , P_TRANSACTIONS = 32, P_MANAGE_USERS = 64 };
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
    int permissions;
    bool markForDeletion = false;
};

stUserData currUser;

const string clientFile = "Clients.txt";
const string UserFile = "Users.txt";

bool checkIfHasPermissions(enUserPermissions permission);

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
    userData.permissions = stoi(user[2]);
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
    vector<stClientData> vClients = readDataFromFile(clientFile);
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

void showAccessDeniedMsg(){
    system("cls");
    cout << "\n==========================================\n";
    cout << "============== Access Denied ===============";
    cout << "\n==========================================\n";
}

bool findClientByAccountNumber(string accountNumber,stClientData* clientData = nullptr){
    vector<stClientData> vClients = readDataFromFile(clientFile);
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
    if(!checkIfHasPermissions(enUserPermissions::P_LIST_CLIENTS)){
        showAccessDeniedMsg();
        return;
    }; 
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

void saveLineToFile(string line, string fileName){
    fstream file;
    file.open(fileName, ios::out | ios::app);
    if(file.is_open()){
        file << line << endl;
        file.close();
    }
}

void addNewClient(){
    if(!checkIfHasPermissions(enUserPermissions::P_ADD_CLIENTS)){
        showAccessDeniedMsg();
        return;
    }; 
    bool isAdd = true;
    header("Add New Client :)");
    stClientData client;
    readClientData(client, &isAdd);
    saveLineToFile(convertRecordToLine(client), clientFile);
    cout << "Client has been added successfully\n";
}

void saveClientsToFile(vector<stClientData> vClients){
    fstream file; 
    file.open(clientFile, ios::out);
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
    if(!checkIfHasPermissions(enUserPermissions::P_DELETE_CLIENTS)){
        showAccessDeniedMsg();
        return;
    }; 
    string accountNumber = readString("please Enter Account number you want to delete? ");
    vector<stClientData> vClients = readDataFromFile(clientFile); 
    stClientData client;
    if(findClientByAccountNumber(accountNumber, &client)){
        char answer = readChar("Are you sure you want to delete this account? \n");
        if(tolower(answer) == 'y'){
            markClientForDeletion(accountNumber,vClients);
            saveClientsToFile(vClients);
            cout << "Account Deleted successfully :)" << endl;
            vClients = readDataFromFile(clientFile);
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
    if(!checkIfHasPermissions(enUserPermissions::P_UPDATE_CLIENTS)){
        showAccessDeniedMsg();
        return;
    }; 
    header("Update Client Data");
    string accountNumber = readString("please Enter Account number you want to update? ");
    vector<stClientData> vClients = readDataFromFile(clientFile); 
    stClientData client;
    if(findClientByAccountNumber(accountNumber, &client)){
        char answer = readChar("Are you sure you want to update this account? \n");
        if(tolower(answer) == 'y'){
            updateClientData(accountNumber,vClients);
            saveClientsToFile(vClients);
            cout << "Account Updated successfully :)";
            vClients = readDataFromFile(clientFile);
        }
    }else{
        cout << "The Account you're looking for not exist or deleted" << endl;
    }
}

void showMenuScreen();
void showLoginScreen();
void showManageUsersMenu();
vector<stUserData> getUsers();

void backToMenu(){
    cout << "Press any key to go back to the Menu :)" << endl;
    system("pause>0");
    showMenuScreen();
}

void backToUsersMenue(){
    cout << "Press any key to go back to the user Menu :)" << endl;
    system("pause>0");
    showManageUsersMenu();
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
    vector<stClientData> vClients = readDataFromFile(clientFile);
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
    vector<stClientData> vClients = readDataFromFile(clientFile);
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
    vector<stClientData> vClients = readDataFromFile(clientFile);
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
    if(!checkIfHasPermissions(enUserPermissions::P_TRANSACTIONS)){
        showAccessDeniedMsg();
        return;
    }; 
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

void printUserData(stUserData user){
    cout << "| " << left << setw(20) <<  user.username;
    cout << "| " << left << setw(10) <<  user.password;
    cout << "| " << left << setw(35) <<  user.permissions;
}

void showUsersList(){
    header("Show User Data");
    vector<stUserData> vUsers = getUsers();
        cout << "| " << left << setw(20) << "userName";
        cout << "| " << left << setw(10) << "Password";
        cout << "| " << left << setw(35) << "Permissions";
        cout << "\n--------------------------------------------------------------------------------------------------\n";
    for(stUserData &user: vUsers){
        printUserData(user);
        cout << endl;
    }
}

bool findUserByUserName(string username, stUserData* userData = nullptr){
    vector<stUserData> vUsers = getUsers();
    for(stUserData &user: vUsers){
        if(convertTextToLower(user.username) == convertTextToLower(username)){
            header("User Data");
            printUserData(user);
            cout << endl;
            if(userData != nullptr){
                *userData = user;
            }
            return true;
        }
    }
    return false;
}

int addNewPermission(char answer,int &permissions, int perm){
    if(tolower(answer) == 'y'){
        permissions += perm;
    }
    return permissions;
}

bool checkIfHasPermissions(enUserPermissions permissoin){
    if(currUser.permissions == enUserPermissions::ALL_P) return true;
    if((permissoin&currUser.permissions) == permissoin) return true;
    return false;
}

int setUserPermissions(){
    char answer = 'y';
    int permissions = 0;

    answer = readChar("Do you want to give this User all permissions Y/N? ");
    if(tolower(answer) == 'y'){
        permissions = -1;
        return permissions;
    }

    cout << "\nDo you want to give access to: \n ";
    answer = readChar("\n Show Client List y/n? ");
        addNewPermission(answer, permissions, enUserPermissions::P_LIST_CLIENTS);
    answer = readChar("\n Add new Client y/n? ");
        addNewPermission(answer, permissions, enUserPermissions::P_ADD_CLIENTS);
    answer = readChar("\n Delete Client y/n? ");
        addNewPermission(answer, permissions, enUserPermissions::P_DELETE_CLIENTS);
    answer = readChar("\n Update Client y/n? ");
        addNewPermission(answer, permissions, enUserPermissions::P_UPDATE_CLIENTS);
    answer = readChar("\n Find Client y/n? ");
        addNewPermission(answer, permissions, enUserPermissions::P_FIND_CLIENTS);
    answer = readChar("\n Transactions y/n? ");
        addNewPermission(answer, permissions, enUserPermissions::P_TRANSACTIONS);
    answer = readChar("\n Manage Users y/n? ");
        addNewPermission(answer, permissions, enUserPermissions::P_MANAGE_USERS);
    return permissions;
}
stUserData readUserData(stUserData user, bool *isAdd = nullptr){
    if(*isAdd){
    user.username =  readString("Please Enter username? ");
        while(findUserByUserName(user.username, &user)){
            cout << "The User is already exist, Please try another one\n";
            user.username =  readString("Please Enter user name? ");
        }
    }
    user.password =  readString("Please Enter user password? ");
    user.permissions = setUserPermissions();
    
    return user;
}

string converUserRecordToLine(stUserData user, string delim = "#//#"){
    return(
        user.username + delim +
        user.password + delim +
        to_string(user.permissions)
    );
}

void addNewUser(){
    bool isAdd = true;
    header("Add New User :)");
    stUserData user;
    user = readUserData(user, &isAdd);
    saveLineToFile(converUserRecordToLine(user, "#//#"), UserFile);
    cout << "User has been added successfully\n";
}

void findUser(){
    header("Find User Screen");
    string username = readString("Please Enter the user name you're looking for? ");
    while(!findUserByUserName(username)){
        cout << "UserName Not Exist! :(" << endl;
        username = readString("Try Again with another user name? ");
    }
}

void markUserForDeletion(string username, vector<stUserData> &vUsers){
    for(stUserData &user: vUsers){
        if(convertTextToLower(user.username) == convertTextToLower(username)){
            user.markForDeletion = true;
        }
    }
}

void saveUsersToFile(vector<stUserData> vUsers){
    fstream file; 
    file.open(UserFile, ios::out);
    if(file.is_open()){
        for(stUserData &user: vUsers){
            if(!user.markForDeletion){
                string line = converUserRecordToLine(user);
                file << line << endl;
            }
        }
        file.close();
    }
}

void deleteUser(){
    string username = readString("please Enter User Name you want to delete? ");
    vector<stUserData> vUsers = getUsers();
    stUserData user;
    if(findUserByUserName(username, &user)){
        char answer = readChar("Are you sure you want to delete this user? \n");
        if(tolower(answer) == 'y'){
            markUserForDeletion(username,vUsers);
            saveUsersToFile(vUsers);
            cout << "User Deleted successfully :)" << endl;
            vUsers = getUsers();
        }
    }else{
        cout << "The User you're looking for not exist or already deleted";
    }
}

void updateUserData(string username, vector<stUserData> &vUsers){
    bool isAdd = false;
    for(stUserData &user: vUsers){
        if(convertTextToLower(username) == convertTextToLower(username)){
            user = readUserData(user, &isAdd);
        }
    }
}

void updateUser(){
    header("Update User Data");
    string username = readString("please Enter username you want to update? ");
    vector<stUserData> vUsers = getUsers(); 
    stUserData user;
    if(findUserByUserName(username, &user)){
        char answer = readChar("Are you sure you want to update this account? \n");
        if(tolower(answer) == 'y'){
            updateUserData(username, vUsers);
            saveUsersToFile(vUsers);
            cout << "Account Updated successfully :)";
            vUsers = getUsers();
        }
    }else{
        cout << "The Account you're looking for not exist or deleted" << endl;
    }
}

short usersMenue(short number){
    switch ((enUsersOptions) number){
        case enUsersOptions::SHOW_USER:
            showUsersList();
            backToUsersMenue();
            break;  
            case enUsersOptions::ADD_USER:
            addNewUser();
            backToUsersMenue();
            break;
            case enUsersOptions::FIND_USER:
            findUser();
            backToUsersMenue();
            break;
            case enUsersOptions::DELETE_USER:
            deleteUser();
            backToUsersMenue();
            break;
            case enUsersOptions::UPDATE_USER:
            updateUser();
            backToUsersMenue();
            break;
            case enUsersOptions::USERS_EXIST :
            showMenuScreen();
            backToUsersMenue();
            break;
        default:
            break;
    }
}

void showManageUsersMenu(){
    if(!checkIfHasPermissions(enUserPermissions::P_MANAGE_USERS)){
        showAccessDeniedMsg();
        return;
    }; 
    system("cls");
    cout <<"==================================================\n";
    cout << "\t\tUsers Menu Screen\n";
    cout <<"==================================================\n";
    cout << "\t[1] Users List.\n"
            "\t[2] Add New User.\n"
            "\t[3] Delete User.\n"
            "\t[4] Update User Info.\n"
            "\t[5] Find User.\n"
            "\t[6] Main Menu." << endl;
    cout <<"==================================================\n";
    short chosenOption = readNumber("Choose what do you want to do? [1 to 6]?");
    usersMenue(chosenOption);
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
            case enOptions::MANAGE_USERS :
            showManageUsersMenu();
            backToMenu();
            break;
            case enOptions::EXIST: 
            showLoginScreen();
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
            "\t[7] Manage Users.\n"
            "\t[8] Logout." << endl;
    cout <<"==================================================\n";
    short chosenOption = readNumber("Choose what do you want to do? [1 to 6]? ");
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
    vector<stUserData> users = getUsers();
    for(stUserData &user: users){
        if(user.username == username && user.password == password){
            currUser = user;
            return true;
        }
    }
    return false;
}

void showLoginScreen(){
    system("cls");
    header("Login Screen");
    bool userExist = false;
    do{
        string username = readString("Enter Username? ");
        string password = readString("Enter Password? ");
        if(isUserExist(username, password)){
            userExist = true;
        }else{
            userExist = false;
            cout << "Invalid Credential :(\n";
        }
    }while(!userExist);
    showMenuScreen();
}

int main(){
    showLoginScreen();
    return 0;
}