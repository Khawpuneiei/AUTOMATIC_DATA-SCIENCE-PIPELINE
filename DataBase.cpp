#include <iostream>  
#include <fstream>               
#include <sstream>               
#include <unordered_map>         
#include <vector>                
#include <string>
using namespace std;
// FUNCTION  Search Key 
void searchKey(const unordered_map<string, unordered_map<string, string>>& hashTable, const string& key) {

    auto it = hashTable.find(key);     

    if (it != hashTable.end()) {
        cout << "Found key: " << key << " | Values: ";
        for (const auto& pair : it->second) {
            cout << pair.first << "=" << pair.second << " ";
        }
        cout << endl;
    } else {
        cout << "Key '" << key << "' not found." << endl;
    }
}

// FUNCTION  Delete Key 
void deleteKey(unordered_map<string, unordered_map<string, string>>& hashTable, const string& key) {
    if (hashTable.erase(key)) {        
        cout << "Key '" << key << "' deleted." << endl;
    } else {
        cout << "Key '" << key << "' not found." << endl;
    }
}

// FUNCTION Insert Key 
void insertKey(unordered_map<string, unordered_map<string, string>>& hashTable,
               const string& key,
               const unordered_map<string, string>& values) {

    hashTable[key] = values;           
    cout << "Inserted/Updated key '" << key << "'." << endl;
}

// FUNCTION  Print Hash Table 
void printHashTable(const unordered_map<string, unordered_map<string, string>>& hashTable) {
    cout << "Hash Table Contents:" << endl;
    for (const auto& pair : hashTable) {
        cout << "Key: " << pair.first << " | Values: ";
        for (const auto& valuePair : pair.second) {
            cout << valuePair.first << "=" << valuePair.second << " ";
        }
        cout << endl;
    }
}
    file.close();
    return table;
}
