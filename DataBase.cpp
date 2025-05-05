#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

// ---------- Table Class ----------
class Table {
private:
    unordered_map<string, unordered_map<string, string>> data;   // hashTable {key -> {column -> value}}

public:
    // Insert or update a row by key
    void insert(const string& key, const unordered_map<string, string>& row) {
        data[key] = row;   // Insert or update the key
        cout << "Inserted/Updated key '" << key << "'." << endl;
    }

    // Search by key and print row values
    void search(const string& key) const {
        auto it = data.find(key);   // Try to find the key in the table
        if (it != data.end()) {
            cout << "Found key: " << key << " | Values: ";
            for (const auto& pair : it->second) {
                cout << pair.first << "=" << pair.second << " ";
            }
            cout << endl;
        } else {
            cout << "Key '" << key << "' not found." << endl;
        }
    }

    // Delete a row by key
    void remove(const string& key) {
        if (data.erase(key)) {   // erase returns 1 if key was found and deleted
            cout << "Key '" << key << "' deleted." << endl;
        } else {
            cout << "Key '" << key << "' not found." << endl;
        }
    }

    // Print the entire table
    void printAll() const {
        cout << "=== Hash Table Contents ===" << endl;
        for (const auto& pair : data) {
            cout << "Key: " << pair.first << " | Values: ";
            for (const auto& valuePair : pair.second) {
                cout << valuePair.first << "=" << valuePair.second << " ";
            }
            cout << endl;
        }
    }

    // Get reference to data (if needed externally)
    const unordered_map<string, unordered_map<string, string>>& getData() const {
        return data;
    }
};

// ---------- Utility: Split CSV Line ----------
vector<string> split(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

// ---------- Load CSV into Table ----------
Table csvToTable(const string& filename) {
    Table table;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return table;
    }

    getline(file, line);
    vector<string> headers = split(line);

    while (getline(file, line)) {
        vector<string> values = split(line);
        if (values.size() == headers.size()) {
            string key = values[0];
            unordered_map<string, string> row;
            for (size_t i = 1; i < headers.size(); ++i) {
                row[headers[i]] = values[i];
            }
            table.insert(key, row);
        }
    }

    file.close();
    return table;
}
