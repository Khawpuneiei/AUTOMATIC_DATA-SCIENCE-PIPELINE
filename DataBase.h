#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

class Table {
private:
    unordered_map<string, unordered_map<string, string>> data;

public:
    void insert(const string& key, const unordered_map<string, string>& row);

    void search(const string& key) const;

    void remove(const string& key);

    void printAll() const;

    static Table loadFromCSV(const string& filename);

    const unordered_map<string, unordered_map<string, string>>& getData() const;
};

vector<string> split(const string& line);

#endif
