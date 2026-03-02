#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include "./mylib.h"
using namespace std;

enum enOptions {SHOW = 1, ADD, DELETE, UPDATE, FIND, EXIST};
struct stClientData {
    string accountNumber;
    string pinCode;
    string name;
    string phone;
    double Balance = 0;
    bool markforDeletion = false;
};
const string fileName = "Clients.txt";


void printClientData(stClientData client){
    cout << "| " << left << setw(20) <<  client.accountNumber;
    cout << "| " << left << setw(10) <<  client.pinCode;
    cout << "| " << left << setw(35) <<  client.name;
    cout << "| " << left << setw(15) <<  client.phone;
    cout << "| " << left << setw(15) <<  client.Balance;   
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

bool findClient(string accountNumber = readString("please Enter Account number you're looking for? "), stClientData client = {}){
    vector<stClientData> vClients = readDataFromFile(fileName);
    for(stClientData &client: vClients){
        if(convertTextToLower(client.accountNumber) == convertTextToLower(accountNumber)){
            header("Client Data");
            printClientData(client);
            cout << endl;
            client = client;
            return true;
        }
    }
    return false;
}

stClientData readClientData(stClientData &client){
    client.accountNumber =  readString("Please Enter client account number? ");
    while(findClient(client.accountNumber, client)){
        cout << "The Account number is already exist, Please try another one\n";
        client.accountNumber =  readString("Please Enter client account number? ");
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
    header("Add New Client :)");
    stClientData client;
    readClientData(client);
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
    if(findClient(accountNumber, client)){
        char answer = readChar("Are you sure you want to delete this account? \n");
        if(tolower(answer) == 'y'){
            markClientForDeletion(accountNumber,vClients);
            saveClientsToFile(vClients);
            cout << "Account Deleted successfully :)";
            vClients = readDataFromFile(fileName);
        }
    }else{
        cout << "The Account you're looking for not exist or already deleted";
    }
}

void updateClientData(string accountNumber, vector<stClientData> &vClients){
    for(stClientData &client: vClients){
        if(convertTextToLower(client.accountNumber) == convertTextToLower(accountNumber)){
            client = readClientData(client);
        }
    }
}

void updateClient(){
    header("Update Client Data");
    string accountNumber = readString("please Enter Account number you want to update? ");
    vector<stClientData> vClients = readDataFromFile(fileName); 
    stClientData client;
    if(findClient(accountNumber, client)){
        char answer = readChar("Are you sure you want to update this account? \n");
        if(tolower(answer) == 'y'){
            updateClientData(accountNumber,vClients);
            saveClientsToFile(vClients);
            cout << "Account Updated successfully :)";
            vClients = readDataFromFile(fileName);
        }
    }else{
        cout << "The Account you're looking for not exist or already deleted";
    }
}

void showMenuScreen();

void backToMenu(){
    cout << "Press any key to go back to the Menu :)";
    system("pause>0");
    showMenuScreen();
}

short chooseFromMenu(short number){
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
            findClient();
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
        case enOptions::EXIST: 
            cout << "Good Bye :(" << endl;
        default:
            break;
    }
}

void showMenuScreen(){
    cout <<"==================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout <<"==================================================\n";
    cout << "\t[1] Show Client List.\n"
            "\t[2] Add New Client.\n"
            "\t[3] Delete Client.\n"
            "\t[4] Update Client Info.\n"
            "\t[5] Find Client.\n"
            "\t[6] Exist." << endl;
    cout <<"==================================================\n";
    short chosenOption = readNumber("Choose what do you want to do? [1 to 6]?");
    chooseFromMenu(chosenOption);
}

int main(){
    showMenuScreen();
    return 0;
}