#include <iostream>              
#include <fstream>               
#include <sstream>               
#include <unordered_map>         
#include <vector>                
#include <string>
#include "DataBase.h"
using namespace std;

// Function to split a CSV 
vector<string> split(const string& line) {
    vector<string> result;            
    stringstream ss(line);            
    string item;

    while (getline(ss, item, ',')) {  
        result.push_back(item);       
    }

    return result;                    
}

// FUNCTION CSV to Hash Table
unordered_map<string, unordered_map<string, string>> csvToHashTable(const string& filename) {

    unordered_map<string, unordered_map<string, string>> hashTable;

    ifstream file(filename);           
    string line;

    if (!file.is_open()) {             
        cout << "Error opening file." << endl;
        return hashTable;              
    }

    
    getline(file, line);  
    vector<string> headers = split(line);  


    while (getline(file, line)) {
        vector<string> values = split(line);    

        if (values.size() == headers.size()) {   
            string key = values[0];  

            unordered_map<string, string> rowData;

          
            for (size_t i = 1; i < headers.size(); ++i) {
                rowData[headers[i]] = values[i];
            }

            hashTable[key] = rowData; 
        }
    }

    file.close(); 
    return hashTable;  
}



int main() {

    int choice;
    string csvname;

    cout << "WELCOME TO \n";
    cout << "AUTOMATE \n";
    cout << "DATASCI \n";
    cout << "PIPELINE";

    //step 1
    cout << "=== Upload CSV === \n"; 
    cout << "Please enter your CSV name : ";
    cin >> csvname;
    //input csv name (string)

    auto myTable = csvToHashTable("data.csv");


    printHashTable(myTable);


    cout << endl << "Search for key '2':" << endl;
    searchKey(myTable, "2");  


    cout << endl << "Insert new key '4':" << endl;
    unordered_map<string, string> newRow = { {"name", "David"}, {"age", "28"} };
    insertKey(myTable, "4", newRow);

    printHashTable(myTable);

    cout << endl << "Delete key '1':" << endl;
    deleteKey(myTable, "1");


    printHashTable(myTable);

    //step 2
    do {
        cout << "=== Do you want to use Database? === \n"; 
        cout << "1.Yes \n"; //Insert / Delete / Search
        cout << "2.No \n"; //go to step 3
        cout << "Please enter your choice : ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                //Database function
                break;
            case 2:
                //go to step 3
                break;
            default:
                cout << "Invalid choice \n" ;
        }
    } while (choice != 2);

    //step 3
    do {
        cout << "=== Do you want to use Automate? ===\n";
        cout << "1.Yes \n"; //go to graph plotting
        cout << "2.No \n"; //use only AI / graph plotting
        cout << "Please enter your choice : \n\n";
        cin >> choice;
        
        switch (choice) {
            case 1:
                //go to graph plotting
                break;
            case 2:
                //use only AI / graph plotting
                break;
            default:
                cout << "Invalid choice \n" ;
        }
    } while (choice != 1);

    return 0;
}

