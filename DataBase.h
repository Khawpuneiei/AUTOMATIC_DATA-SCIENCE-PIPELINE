#include <iostream>  
#include <fstream>               
#include <sstream>               
#include <unordered_map>         
#include <vector>                
#include <string>
using namespace std;
void searchKey(const unordered_map<string, unordered_map<string, string>>& hashTable, const string& key);
void deleteKey(unordered_map<string, unordered_map<string, string>>& hashTable, const string& key);
void insertKey(unordered_map<string, unordered_map<string, string>>& hashTable,
               const string& key,
               const unordered_map<string, string>& values);

void printHashTable(const unordered_map<string, unordered_map<string, string>>& hashTable);
