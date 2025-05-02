#include <iostream>              
#include <fstream>               
#include <sstream>               
#include <unordered_map>         
#include <vector>                
#include <string>

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

    return 0;
}

