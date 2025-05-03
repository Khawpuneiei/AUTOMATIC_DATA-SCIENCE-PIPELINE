#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "DataBase.h"
#include "Plot.h"

using namespace std;

// CSV Split Function
vector<string> split(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

// CSV to Hash Table
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

    cout << "WELCOME TO\n";
    cout << "AUTOMATE DATASCI PIPELINE\n\n";

    // Step 1: Upload CSV
    cout << "=== Upload CSV ===\n";
    cout << "Please enter your CSV file name (e.g., candy.csv): ";
    cin >> csvname;

    auto myTable = csvToHashTable(csvname);
    printHashTable(myTable);

    // Step 2: Database Function
    do {
        cout << "\n=== Do you want to use Database? ===\n";
        cout << "1. Yes\n";
        cout << "2. No\n";
        cout << "Please enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int dbChoice;
            do {
                cout << "\n=== Database Menu ===\n";
                cout << "1. Insert\n";
                cout << "2. Delete\n";
                cout << "3. Search\n";
                cout << "4. Print\n";
                cout << "5. Back\n";
                cout << "Enter your choice: ";
                cin >> dbChoice;

                switch (dbChoice) {
                    case 1: {
                        string key, field, value;
                        cout << "Enter key: ";
                        cin >> key;
                        unordered_map<string, string> newRow;
                        cout << "How many fields? ";
                        int n;
                        cin >> n;
                        for (int i = 0; i < n; ++i) {
                            cout << "Field name: ";
                            cin >> field;
                            cout << "Value: ";
                            cin >> value;
                            newRow[field] = value;
                        }
                        insertKey(myTable, key, newRow);
                        break;
                    }
                    case 2: {
                        string key;
                        cout << "Enter key to delete: ";
                        cin >> key;
                        deleteKey(myTable, key);
                        break;
                    }
                    case 3: {
                        string key;
                        cout << "Enter key to search: ";
                        cin >> key;
                        searchKey(myTable, key);
                        break;
                    }
                    case 4:
                        printHashTable(myTable);
                        break;
                    case 5:
                        break;
                    default:
                        cout << "Invalid choice.\n";
                }
            } while (dbChoice != 5);
        }
    } while (choice != 2);

    // Step 3: Automate Plot
    do {
        cout << "\n=== Do you want to use Automate (Plot Chart)? ===\n";
        cout << "1. Yes\n";
        cout << "2. No\n";
        cout << "Please enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            vector<string> headers;
            if (!myTable.empty()) {
                const auto& firstRow = myTable.begin()->second;
                for (const auto& pair : firstRow) {
                    headers.push_back(pair.first);
                }
            }

            cout << "\nAvailable columns:\n";
            for (size_t i = 0; i < headers.size(); ++i) {
                cout << i << ": " << headers[i] << "\n";
            }

            int x_index, y_index;
            cout << "Enter index for X axis: ";
            cin >> x_index;
            cout << "Enter index for Y axis: ";
            cin >> y_index;

            string x_col = headers[x_index];
            string y_col = headers[y_index];

            auto data = extract_numeric_columns(myTable, {x_col, y_col});
            vector<double> x = data[x_col];
            vector<double> y = data[y_col];

            string chart_type = ask_chart_type();
            if (chart_type == "line") {
                plot_line(x, y, x_col, y_col);
            } else {
                plot_bar(x, y, x_col, y_col);
            }
        }
    } while (choice != 2);

    return 0;
}
