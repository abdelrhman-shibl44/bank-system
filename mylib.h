#ifndef MYLIB_H
#define MYLIB_H
#include <iostream>
#include <limits>
#include <math.h>
#include <numbers>
#include <vector>
using namespace std;

struct sDate {
    short Year;
    short Month;
    short Day;
};

int getMonthDays(int year, int month); 

int readNumber(string message){
    int num = 0;
    cout << message;
    cin >> num;
    
    while(num < 0){
        cout << "Please Enter a positive number: ";
        cin >> num;
    }

    while (cin.fail())
    {       
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid number, Enter a valid one: ";
    }
    
    return num;
}


bool checkIfLeapYear(int year){
    if(year % 400 == 0 || year % 4 == 0 && year % 100 != 0){
        return true;
    }else{
        return false;
    }
};


short getDayIndex(int year, int month, short day){
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + (12 * a) - 2;
    short d = ((day + y) + (y / 4) - (y / 100) + (y / 400) + ((31 * m) /12)) % 7;
    return d;
}
short getDayIndex(sDate Date){
    return getDayIndex(Date.Year,Date.Month, Date.Day);
}

bool isLeapYear(int year){
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) ? true : false;
}


short getDaysFromDate(short year, short month,short day){
    short totalDays = 0;
    for(int i = 1; i < month ; i++){
        short monthDays =  getMonthDays(year, i);
        totalDays += monthDays;
    }
    totalDays += day;
    return totalDays;
}

sDate getDateFromDays(short days, short year){
    sDate Date;
    Date.Year = year;
    Date.Month = 1;
    short monthDays = 0;

    for(short i = 1; i <= 12; i++){
        monthDays =  getMonthDays(year, Date.Month);
        if(days > monthDays){
            days-= monthDays;
            Date.Month++;
        }
    }
    Date.Day = days;
    return Date;
}

int days31Arr[12] = {31, 30, 31,30, 31,30, 31, 31, 30, 31, 30, 31};
int getMonthDays(int year, int month){
    return month == 2 ? isLeapYear(year) ? 29 : 28 : days31Arr[month - 1];
}


bool isLastDayInMonth(short year, short month, short day){
    short monthDays =  getMonthDays(year, month);
    return day == monthDays ? true : false;
}

bool isFirstDayInMonth(short year, short month, short day){
    return day == 1 ? true : false;
}

bool isLastMonthInYear(short month){
    return month == 12 ? true : false;
}

bool isFirstMonthInYear(short month){
    return month == 1 ? true : false;
}



bool isDateOneBeforeDate2(sDate date1, sDate date2){
    return (date1.Year < date2.Year) 
            ? true
            : ((date1.Year == date2.Year)
                ? (date1.Month < date2.Month
                    ? true 
                    : (date1.Month == date2.Month
                        ? date1.Day < date2.Day
                        : false
            )):false);
}

sDate increaseDateByOne(sDate Date){
    int total = getDaysFromDate(Date.Year, Date.Month, Date.Day);   
    if(isLastDayInMonth(Date.Year, Date.Month, Date.Day)) {
        if(isLastMonthInYear(Date.Month)) {
            Date.Day = 1;
            Date.Month = 1;
            Date.Year++;
        }else {
            Date.Month++;
            Date.Day = 1;
        }
    } else Date.Day++;
    return Date;
}

sDate decreaseByOne(sDate Date){
    if(isFirstDayInMonth(Date.Year, Date.Month, Date.Day)) {
        if(isFirstMonthInYear(Date.Month)) {
            Date.Month = 12;
            Date.Day = 31;
            Date.Year--;
        }else {
            Date.Month--;
            Date.Day = getMonthDays(Date.Year, Date.Month);
        }
    } else Date.Day--;
    return Date;
}

int getDifferentDaysNumber(sDate date1, sDate date2){
    cout << date1.Year << "/" << date2.Year << endl;
    cout << date1.Month << "/" << date2.Month << endl;
    cout << date1.Day << "/" << date2.Day << endl;
    int dateOneDays = getDaysFromDate(date1.Year, date1.Month, date1.Day);
    int dateTwoDays = getDaysFromDate(date2.Year, date2.Month, date2.Day);
    cout << dateOneDays << endl;
    cout << dateTwoDays << endl;
    return dateTwoDays - dateOneDays;
}
void swapDates(sDate &date1, sDate &date2){
    sDate temp;
    temp.Day = date1.Day;
    temp.Month = date1.Month;
    temp.Year = date1.Year;

    date1.Day = date2.Day;
    date1.Month = date2.Month;
    date1.Year = date2.Year;
    
    date2.Day = temp.Day;
    date2.Month = temp.Month;
    date2.Year = temp.Year;
}

short getDifferentDaysNumber2(sDate date1, sDate date2, bool includeLastDay = false){
    short Day = 0;
    short flagValue = 1;
    if(!isDateOneBeforeDate2(date1, date2)){
        swapDates(date1, date2);
        flagValue = -1;
    }
    while(isDateOneBeforeDate2(date1, date2)){
        Day++;
        date1 = increaseDateByOne(date1);
    }
    return includeLastDay ? ++Day * flagValue : Day  * flagValue;
}

string getDayString(short dayIndex){
    string Days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return Days[dayIndex];
}

#endif


bool isPrimeNum(int number){
  int divideNum =  number / 2;
  for(int i =  2; i <= divideNum ; i++){
    if(number % i == 0 )
    return false;
}
    return true;
}

bool isPerfectNumber(int num){
    int sum = 0;
    for(int i= 1; i < num ; i++){
        if(num % i == 0 ){
            sum += i;
        }
    }
    return sum == num;
}

string readString(string message){
    string input = "";
    cout << message;
    getline(cin  >> ws, input);

    while (cin.fail())
    {       
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid number, Enter a valid string: ";
    }
    
    return input;
}

char readChar(string message){
    char input;
    cout << message;
    cin >> input;

    while (cin.fail())
    {       
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid number, Enter a valid string: ";
    }
    
    return input;
}

void readArr(int arr[100],int length){
    for(int i = 0; i < length; i++){
    cout << arr[i] << " ";
  }
  cout << endl;
} 
void readArr(string arr[100],int length){
    for(int i = 0; i < length; i++){
        cout << "array [" << i << "] " << arr[i] << " " << endl;;
    }
} 

void readArr(vector<int>& arr){
    for(int i = 0; i < arr.size(); i++){
            cout << arr[i] << " ";   
    }
} 

void readArr(vector<string>& arr){
    for(int i = 0; i < arr.size(); i++){
            cout << arr[i] << " ";   
    }
} 

string reverseNumber(string userInput){
    int length = userInput.length();
    string arr[length];
    for(int i = 0; i < length; i++){
        arr[length - i] =  userInput[i]; 
    }
    return userInput;

}

string printReverseNumber(int userInput){
    while(userInput > 0){
        int remainder = userInput % 10;
        cout << "rest num is" << remainder << endl;
        userInput = userInput / 10;
        cout << userInput << endl;
    }
}

int reverseNumber(int userInput){
    string name = "";
    while(userInput > 0){
        int remainder = userInput % 10;
        name += to_string(remainder);
        userInput = userInput / 10;
    }
    return stoi(name);
}

int reverseNumber2(int userInput){
    int num2 = 0;
    while(userInput > 0){
        int remainder = userInput % 10;
        num2 =  num2 * 10 + remainder;
        userInput = userInput / 10;
    }
    return num2;
}

int printNumbersInSameOrder(int userInput){
    int num2 = 0;
    while(userInput > 0){
        int remainder = userInput % 10;
        num2 =  num2 * 10 + remainder;
        userInput = userInput / 10;
    }
    return reverseNumber2(num2);
}

int sumOfDigits(int userInput){
    int sum = 0;
    while(userInput > 0){
        int remainder = userInput % 10;
        sum += remainder;
        userInput = userInput / 10; 
    }   
    return sum;
}

int repeatedNum(int userInput, int numToCheck){
    int counter = 0;
    int remainder = 0;

    while(userInput >  0){
        int value =  userInput / 10;
        remainder = userInput % 10;
        userInput = value;
        if(numToCheck == remainder){
            counter++;
        }
    }

    return counter;
}

int repeatedNum(int arr[100], int elementLength, int numToCheck){
    int counter = 0;
    for(int i = 0; i < elementLength; i++){
        if(arr[i] == numToCheck){
            counter++;
        }
    }

    return counter;
}
bool isRepeated(int arr[100], int elementLength, int numToCheck){
    for(int i = 0; i < elementLength; i++){
        if(arr[i] == numToCheck){
            return true;
        }
    }

    return false;
}

void PrintAllDigitFrequency(int userInput,int length){
    int digitFrequency = 0;
    for(int i = 0; i < length; i++){
        digitFrequency =  repeatedNum(userInput, i);
        if(digitFrequency > 0){
            cout << "number " << i << " repeated " << digitFrequency  << " times\n";
        }
    }
}

bool isPalindrom(int userInput){ 
    int reversedDigit = reverseNumber2(userInput);
    if(userInput != reversedDigit){
        return false;
    }
    
    return true;
}

bool isPalindrom(int arr[100], int arrayLength){
    for(int i = 0; i < arrayLength / 2; i++){
        if(arr[i] != arr[arrayLength - 1 - i]){
            return false;
        }
    }
    return true;
}


void InvertedNumber(int userInput){
    for(int i = userInput; i > 0; i--){
        for(int j = 0; j < i ; j++){
            cout << i ;
        }
        cout << endl;
    }
}

void InvertedLetters(int userInput){
    for(int i = userInput; i > 0; i--){
        for(int j = 0; j < i ; j++){
            cout << char(i + 64) ;
        }
        cout << endl;
    }
}

void normalNumberPattern(int userInput){
    for(int i = 1; i <= userInput; i++){
        for(int j = 0; j < i ; j++){
            cout << i ;
        }
        cout << endl;
    }
}

void lettersPattern(int userInput){
    for(int i = 1; i <= userInput; i++){
        for(int j = 0; j < i ; j++){
            cout <<  char(i + 64) ;
        }
        cout << endl;
    }
}

bool lettersPattern2(string passletters){
    string word = "";
    int counter = 0;
    for(int i = 65; i <= 90; i++){
        for(int j = 65 ; j <= 90; j++){
            for(int k = 65 ; k <= 90; k++){
                word += char(i);
                word += char(j);
                word += char(k);
                counter++;

                cout << "Trial [" << counter << "] : " << word << endl;
                if(passletters == word){
                    cout << "pass letters here " << word;
                    return true;
                }
                word = "";
            }
        }
    }
}


string encryptText(string text, short keyEncryption){
    cout << "encryptedText before encryption " << text << endl;
    for(int i =0; i <= text.length(); i++){
        text[i] = char((int)text[i] + keyEncryption);
    }
    return text;
}

string DecryptText(string encryptedText, short keyEncryption){
    cout << "text after Encrypted " << encryptedText << endl;
    for(int i =0; i < encryptedText.length(); i++){
        encryptedText[i] = char((int)encryptedText[i] - keyEncryption);
    }
    cout << "text after Decryption " << encryptedText << endl;
    return encryptedText;
}

int randomNum(int From, int To){
    return rand() % (To - From + 1) + From;
}

enum enRandom {CAPITAL_LETTER, SMALL_LETTER, SPECIAL_CHAR, DIGIT};

char getRandom(enRandom CharType){
    switch (CharType) {
        case(enRandom::CAPITAL_LETTER):
            return char(randomNum(65,90));
        break;
        case(enRandom::SMALL_LETTER):
            return char(randomNum(97,122));
        break;
            case(enRandom::SPECIAL_CHAR):
            return char(randomNum(33,47));
        break;
    
    default:
        return randomNum(48,57);
        break;
    }
}


string generateWord(enRandom CharType, int wordLength){
    string randomWord;
        for(int i = 0; i < wordLength; i++){
            randomWord += getRandom(CharType);
        }
    return randomWord;
}
string generateKey(enRandom CharType, int wordLength, int wordsLength){
    string randomKey ;
    for(int i = 0; i < wordsLength; i++) {
        randomKey += generateWord(CharType, wordLength);
        randomKey += (i < wordsLength - 1 ? "-" :"");
    }
    return randomKey;
}

string generateKeys(int randomKey, int wordLength, int wordsLength){ 
    string key = "";
    for(int i = 0; i < randomKey; i++) {
        // cout << "Key [" << i + 1 << "]: " <<  generateKey(enRandom::CAPITAL_LETTER, wordLength, wordsLength) << endl;
        key = generateKey(enRandom::CAPITAL_LETTER, wordLength, wordsLength);;
    }
    return key;
}
void fillInOrderNums(int arr[100], int maxNum){
    for(int i = 0; i <= maxNum; i++){
        arr[i] = i + 1;
    }
}

void fillWithRandomNums(int arr[100], int elementsLength){
        for(int i = 0; i <= elementsLength; i++){
            arr[i] = randomNum(1, 100);
        }
}

void fillArray(int arr[100], int elementLength){
    cout << "Enter array elements:" << endl;
    for(int i = 0; i < elementLength; i++){
        cout << "Element [" << i + 1 << "]: ";
        arr[i] = readNumber("");
    }
}


void fillArrayWithRandomNumbers(int arr[100], int elementsLength,bool isMix = false){
        for(int i = 0; i < elementsLength; i++){
            arr[i] = isMix ? randomNum(-100, 100) : randomNum(1, 100);
        }
}

int getMaxNum(int arr[100], int elementLength){
    int temp = arr[0];
    for(int i = 0; i < elementLength; i++){
        if(temp < arr[i] ){
            temp = arr[i];
        }
    }
    return temp;
}
int getMinNum(int arr[100], int elementLength){
    int temp = arr[0];
    for(int i = 1; i < elementLength; i++){
        if(temp > arr[i] ){
            temp = arr[i];
        }
    }
    return temp;
}

int sumOfDigits(int arr[100], int elementsLength){
    int sum = 0;
    for(int i = 0; i < elementsLength; i++){
        sum += arr[i];
    }
    return sum;
}

float avergeOfNumbers(int arr[10], int elementsLength){
    return (float)sumOfDigits(arr, elementsLength) / elementsLength;
}

void copyArray(int arr[100],int elementsLength,int arr2[100]){
    for(int i = 0; i < elementsLength; i++){
        arr2[i] = arr[i];
    }
}

void AddArrayElement(int num, int arr[100], short &arrayLength){
    arrayLength++;
    arr[arrayLength - 1] = num;
}

void copyArray(int arr[100], int arr2[100], int arrayLength, short arrayLength2){
    for(int i = 0; i < arrayLength; i++){
        AddArrayElement(arr[i], arr2, arrayLength2);
    }
}

void copyPrimeNumsFromArray(int arr[100],int elementsLength, vector<int>& arr2){
    for(int i = 0; i < elementsLength; i++){
        if(isPrimeNum(arr[i])){
            arr2.push_back(arr[i]);
        }
    }
}

void sumDigitsOfTwoArrays(int arr[100], int arr2[100],int arr3[100], int elementsLength){
        for(int i = 0; i < elementsLength; i++){
            arr3[i] = arr[i] + arr2[i];
        }   
}

void Swap(int& A, int& B){
    int temp = A;
    A = B;
    B = temp;
}

void shuffleArray(int arr[100], int elementsLength){
    for(int i = 0; i < elementsLength; i++){
        Swap( arr[randomNum(1, elementsLength - 1)], arr[randomNum(1, elementsLength - 1)]);
    }  
}

void copyInReverseOrder(int arr[100],int arr2[100], int elementsLength){
        for(int i = 0; i < elementsLength; i++){
        arr2[i] = arr[elementsLength - 1 - i];
    }
}

void createArraysWithKeys(string arr[100],int elementsLength){
    for(int i = 0; i < elementsLength; i++){
        arr[i] = generateKey(enRandom::CAPITAL_LETTER, 4, 5);
    }
}

short findNumPosition(int arr[100],int elementLength, short num){
    for(int i = 0; i < elementLength; i++){
        cout << "array to look for nubmers >> " << arr[i] << endl;
        if(num == arr[i]){
            return arr[i];
        }
    }
    return -1;
}

short findNumPositionInMatrix(int arr[3][3],int rows, int cols, short num){
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(num == arr[i][k]){
                return k; 
                break;
            };
        }
    }
    return -1;
}
bool isNumInArray(int arr[3][3],int rows, int cols, short num){
    return findNumPositionInMatrix(arr, rows, cols, num) != -1;
}
bool isNumInArray(int arr[100],int elementLength, short num){
    return findNumPosition(arr, elementLength, num) != -1;
}



void fillArrWithOddNums(int arr[100], int arrayLength, int arr2[100], short &arrayLength2){
    for(int i = 0; i < arrayLength; i++){
        if(arr[i] % 2 != 0){
            AddArrayElement(arr[i], arr2, arrayLength2);
        }
    }
}

void fillArrWithPrimeNums(int arr[100], int arrayLength, int arr2[100], short &arrayLength2){
    for(int i = 0; i < arrayLength; i++){
        if(isPrimeNum(arr[i])){
            AddArrayElement(arr[i], arr2, arrayLength2);
        }
    }
}

void fillDistinctElements(int arr[100], int arrayLength, int arr2[100], short &arrayLength2){
    for(int i = 0; i < arrayLength; i++){
        if(!isNumInArray(arr2, arrayLength2, arr[i])){
            AddArrayElement(arr[i], arr2, arrayLength2);
        }
    }
}

bool isOddNumber(int num){
    return num % 2 != 0;
}
bool isEvenNumber(int num){
    return num % 2 == 0;
}
bool isPositiveNum(int num){
    return num > 0;
}
bool isNegativeNum(int num){
    return !isPositiveNum(num);
}

enum enCounttype {ALL = 1, ODD, EVEN, Pos, Neg};
int countNums(int arr[100], int arrLength, enCounttype countType){
    short counter = 0;
    for(int i = 0; i < arrLength; i++){
        switch (countType){
            case enCounttype::ODD:
                    isOddNumber(arr[i]) ? counter++ : counter;
                break;
            case enCounttype::EVEN:
                    isEvenNumber(arr[i]) ? counter++ : counter;
                break;
            case enCounttype::Pos:
                    isPositiveNum(arr[i]) ? counter++ : counter;
                break;
            case enCounttype::Neg:
                    isNegativeNum(arr[i]) ? counter++ : counter;
                break;

            default:
                counter++;
                break;
            }
    }
    return counter;
}

int myAbs(int num){
    return num > 0 ? num : num * -1;
}

// problem solving number => 3 
void fillMatrixArrWithOrderNums(int arr[3][3], short arrLength){
    int counter = 1;
    for(int i = 0; i < arrLength; i++ ){
        for(int k = 0; k < arrLength; k++ ){
            arr[i][k] = counter++;
        }
        cout << endl;
    }
}


void readMatrixArr(int arr[3][3], short arrLength){
    for(int i = 0; i < arrLength; i++ ){
        for(int k = 0; k < arrLength; k++ ){
            printf("%0*d ", 2 , arr[i][k]);
        }
        cout << endl;
    }
}

// int sumRows(int arr[3][3] ,int rowNum,int colNum){
//         int rowSum = 0;
//         for(int k = 0; k < colNum; k++ ){
//             rowSum += arr[rowNum][k];
//         }
//         return rowSum;
// }

// int sumCols(int arr[3][3] ,int rowNum,int colNum){
//         int rowSum = 0;
//         for(int k = 0; k < colNum; k++ ){
//             rowSum += arr[k][rowNum];
//         }
//         return rowSum;
// }

// void sumMatrixRows(int arr[3][3], short rows, short cols, int arr2[3]){
//     int hi = 0;
//     for(int i = 0; i < rows; i++ ){
//         // cout <<   sumRows(arr, i, cols) << " ";
//         arr2[i] = sumCols(arr, i, cols);
//     }
//     // cout << hi;
//     // return hi;
// }

// void transposeMatrixArr(int arr[3][3],int arr2[3][3], int arrLength){
//     for(int i = 0; i < 3; i++ ){
//         for(int k = 0; k < 3; k++){
//             arr2[i][k] = arr[k][i];
//         }
//     }
// }
void fillMatrixWithRandomNums(int arr[3][3], int rows, int cols){
     for(int i = 0; i < rows; i++ ){
        for(int k = 0; k < cols; k++){
            arr[i][k] =  randomNum(1,100);                 
        }
    }
}

void multiplyMatrix(int arr[3][3], int arr2[3][3], int arr3[3][3], int rows, int cols){
    for(int i = 0; i < rows; i++ ){
        for(int k = 0; k < cols; k++){
            arr3[i][k] =  arr[i][k] * arr2[i][k];                 
        }
    }
}

void printMiddleRow(int arr[3][3],int rows, int cols){
    short shortmiddleRow = rows  / 2;
        for(int k = 0; k < cols; k++){
            cout << arr[k][1] << " "; 
        }
        cout << endl;
}
// void printMiddleCol(int arr[3][3],int rows, int cols){
//     for(int i = 0; i < rows; i++ ){
//         for(int k = 0; k < cols; k++){
//             cout << arr[k][1] << " ";   
//         }
//     }
// }
int sumMatrixArr(int arr[3][3],int rows, int cols){
    int sum = 0;
    for(int i = 0; i < cols; i++){
        for(int k = 0; k < cols; k++){
            sum += arr[i][k];
        }
        cout << endl;
    }
    return sum;
}


// bool isMatrixEqual(int arr[3][3], int arr2[3][3],int rows, int cols){
//     for(int i = 0; i < cols; i++){
//         for(int k = 0; k < cols; k++){
//             cout << "(numbers to check arr2) " << arr2[i][k] << endl;
//             cout << "(find number in arr1) " <<  findNumPositionInMatrix(arr, rows, cols, arr2[i][k]) << endl ;
//             if(findNumPositionInMatrix(arr, rows, cols, arr2[i][k]) == -1){
//                 return false; 
//                 break;
//             };
//         }
//     }
//     return true;
// }
bool isMatrixEqual(int arr[3][3], int arr2[3][3],int rows, int cols){
    for(int i = 0; i < cols; i++){
        for(int k = 0; k < cols; k++){
            if(sumMatrixArr(arr, rows, cols) != sumMatrixArr(arr2, rows, cols)){
                return false; 
                break;
            };
        }
    }
    return true;
}

bool isIdentityMatrix(int arr[3][3], int rows, int cols){
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(i == k && arr[i][k] != 1 || i != k && arr[i][k] !=0 ){
                return false;
            }
        }
    }
    return true;
}

bool isScalarMatrix(int arr[3][3], int rows, int cols){
    short firstDiagonal = arr[0][0];
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if((i == k && arr[i][k] != firstDiagonal) || i != k && arr[i][k] !=0 ){
                return false;
            }
        }
    }
    return true;
}

short countNumberInMatrix(int arr[3][3], int rows, int cols, short numToCheck){
    short counter = 0;
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(arr[i][k] == numToCheck){
                counter++;
            }
        }
    }
    return counter;
}

bool isSparceMatrix(int arr[3][3], int rows, int cols){
        short arrSize = rows * cols;
        return countNumberInMatrix(arr, 3, 3, 0) > ( arrSize / 2);
}

void printIntersectedNumbersInMatrixes(int arr[3][3], int arr2[3][3],short rows, short cols){
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(isNumInArray(arr2, 3, 3, arr[i][k])){
                cout << arr[i][k] << " ";
            }
        }
    }
}

short minNumInMatrix(int arr[3][3], short rows, short cols){
    short num = arr[0][0];
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(num > arr[i][k]){
                num = arr[i][k];
            }
        }
    }
    return num;
}

short maxNumInMatrix(int arr[3][3], short rows, short cols){
    short num = arr[0][0];
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols; k++){
            if(num < arr[i][k]){
                num = arr[i][k];
            }
        }
    }
    return num;
}

bool isPalindromMatrix(int arr[3][3],short rows, short cols){
    for(int i = 0; i < rows; i++){
        for(int k = 0; k < cols/2; k++){
            if(arr[i][k] != arr[i][cols - 1 - k]){
                return false;
            }
        }
    }
    return true;
}

void fibonacciSeries(short n, short prev1,short prev2){
    if(n == 0) return;
    short res = 0;
    res = prev1 + prev2;
    cout << res << " ";
    prev1 = prev2;
    prev2 = res;
    fibonacciSeries(n - 1, prev1, prev2);
}


void getFirstLetterFromString(string name){
    bool isFirstLetter = true;
    string newName = "";
    for(short i = 0; i < name.length(); i++){
        if(name[i] != ' ' && isFirstLetter){
            name[i] = tolower(name[i]);
        }
        isFirstLetter = (name[i] == ' ' ? true : false);
    }
    cout << name;
}
string convetTextToUpper(string text){
    for(short i = 0; i < text.length(); i++){
        text[i] = toupper(text[i]);
    }
    return text;
}

string convetTextToLower(string text){
    for(short i = 0; i < text.length(); i++){
        text[i] = tolower(text[i]);
    }
    return text;
}

char invertChar(char character){
    return isupper(character) ? tolower(character) : toupper(character);
}

string invertAllLetters(string chars){
    for(short i = 0; i < chars.length(); i++){
        chars[i] = invertChar(chars[i]);
    }
    return chars;
}

short countCapitalLetters(string text){
    short lettersCount = 0;
    for(short i = 0; i < text.length(); i++){
        if(isupper(text[i])){
            lettersCount++;
        }
        
    }
    return lettersCount;
}

short countSmallLetters(string text){
    short lettersCount = 0;
    for(short i = 0; i < text.length(); i++){
        if(islower(text[i])){
            lettersCount++;
        }
    }
    return lettersCount;
}

short countCharInString(string name, char character, bool matchCase = false){
    short counter = 0;
    for(short i = 0; i < name.length(); i++){
        if(matchCase && toupper(name[i]) == toupper(character)){
            counter++;
        }else if(name[i] == character){
            counter++;
        }
    } 
    return counter;
}

bool isVowel(char character){
    char vowels[] = {'a','e','i','o','u'};
    for(char &vowel:  vowels){
        if(toupper(character) == toupper(vowel)){
            return true;
        }
    }
    return false;
}

short countVowels(string text){
    short counter = 0;
    for(short i = 0; i < text.length(); i++){
        counter += isVowel(text[i]);
    }  
    return counter;
};

void VowelsInString(string text){
    for(short i = 0; i < text.length(); i++){
        if(isVowel(text[i])){
            cout << text[i] << " ";
        };
    }  
};

short countWordsInString(string text){
    short pos = 0;
    string word = "";
    short counter = 0;
    while((pos = text.find(' ')) != string::npos){
        word = text.substr(0, pos);
        if(word != ""){
            counter++;
        }
        text.erase(0, pos + 1);
    }
    if(text !=""){
        counter++;
    }
    return counter;
}

vector<string>split(string text, string delim){
    cout << "Tokens =" << countWordsInString(text) << endl;
    vector<string> words;
    short pos = 0;
    string word = "";
    short counter = 0;

        while((pos = text.find(delim)) != string::npos){
        word = text.substr(0, pos);
        if(word != ""){
            words.push_back(word);
        }
        text.erase(0, pos + delim.length());
    }
    if(text !=""){
        words.push_back(text);
    }
    return words;
}

void printWordsInString(string text){
    short pos = 0;
    string word = "";

    while((pos = text.find(' ')) != string::npos){
        word = text.substr(0, pos);
        if(word != ""){
            cout << word << endl;
        }
        text.erase(0, pos + 1);
    }
    cout << text << endl;
}

string trimLeft(string text){
    for(short i = 0; i < text.length(); i++){
        if(text[i] != ' '){
            return  text.substr(i, text.length() - i);
        }
    }
    return "";
}

string trimRight(string text){
    for(short i = text.length() - 1; i > 0; i--){
        if(text[i] != ' '){
            return text.substr(0, i + 1);
        }
    }
    return "";
}

string trim(string text){
    return trimRight(trimLeft(text));
    
}

string join(vector<string> arr,string delim = " "){
    string text = "";
    for(string &t: arr){
        text += t + delim;
    }
    return text.substr(0, text.length() - 1);;
}

string join(string arr[100],int arrLength,string delim = " "){
    string text = "";
    for(int i = 0; i < arrLength; i++){
        text += arr[i] + delim;
    }
    return text.substr(0, text.length() - 1);;
}

// string reverseString(string text){
//     vector<string> arr = split(text, " ");
//     vector<string> reversedArr;

//     for(int i = arr.size() ; i >= 0; i--){
//         reversedArr.push_back(arr[i]) ;
//     }
    
//     return join(reversedArr, " ");
// }
string reverseString(string text){
    vector<string> arr = split(text, " ");
    string string2 = "";

    vector<string>::iterator iter = arr.end();

    while (iter != arr.begin()){   
        --iter;
        string2 += *iter + " ";
    }
    return string2.substr(0, string2.length() - 1);
    // return join(reversedArr, " ");
}

// string replaceString(string text, string toReplace, string replaceTo){
//     vector<string> arr =  split(text, " ");
//         for(int i = 0; i < arr.size(); i++){
//         if(convetTextToLower(arr[i]) == convetTextToLower(toReplace)){
//             arr[i] = replaceTo;
//         }
//     }
//     return join(arr);
// }


bool isPunc(char character){
    char puncs[] = {',', '%', '#', '@', '*' , '!' , '(' , ')', '*'};
    for(int i = 0; i < sizeof(puncs); i++){
        if(puncs[i] == character){
            return true;
        }
    }
    return false;
}

string removePunc(string text){
    short pos = 0;
    string s2 = "";
    for(int i = 0; i < text.length(); i++){
        if(!isPunc(text[i])){
            s2 += text[i];
        };
    }

    return s2;
}

// string removePunc(string text = "abdo,hi#,*()sss"){
//     for(int i = 0; i < text.length(); i++){
//         if(ispunct(text[i])){
//             text[i] = ' ';
//         };
//     }
//     return text;
// }
string replaceString(string text, string toReplace, string replaceTo){
    short pos = 0;
    while((pos = text.find(toReplace))  != string::npos){
        text.replace(pos, toReplace.length(), replaceTo);
    }
    return text;
}