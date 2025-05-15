#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "DataBase.h"
#include "Plot.h"
#include "decision_tree.h"

using namespace std;

int main() {
    int choice;
    string csvname;

    cout << "WELCOME TO\n";
    cout << "AUTOMATE DATASCI PIPELINE\n\n";

    // Step 1: Upload CSV
    cout << "=== Upload CSV ===\n";
    cout << "Please enter your CSV file name (e.g., candy.csv): ";
    cin >> csvname;

    vector<string> headers;
    auto table = csvToHashTable(csvname, headers);
    printHashTable(table);

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
                        insertKey(table, key, newRow);
                        break;
                    }
                    case 2: {
                        string key;
                        cout << "Enter key to delete: ";
                        cin >> key;
                        deleteKey(table, key);
                        break;
                    }
                    case 3: {
                        string key;
                        cout << "Enter key to search: ";
                        cin >> key;
                        searchKey(table, key);
                        break;
                    }
                    case 4:
                        printHashTable(table);
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
            if (!table.empty()) {
                const auto& firstRow = table.begin()->second;
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

            string chart_type = ask_chart_type();

            if (chart_type == "cat_bar") {
                vector<string> categories;
                vector<double> values;
                for (const auto& row : table) {
                    categories.push_back(row.second.at(x_col));
                    try {
                        values.push_back(stod(row.second.at(y_col)));
                    } catch (...) {
                        values.push_back(0);
                    }
                }
                plot_categorical_bar(categories, values, x_col, y_col);
            } else if (chart_type == "cat_line") {
                vector<string> categories;
                vector<double> values;
                for (const auto& row : table) {
                    categories.push_back(row.second.at(x_col));
                    try {
                        values.push_back(stod(row.second.at(y_col)));
                    } catch (...) {
                        values.push_back(0);
                    }
                }
                plot_categorical_line(categories, values, x_col, y_col);
            } else {
                auto data = extract_numeric_columns(table, {x_col, y_col});
                vector<double> x = data[x_col];
                vector<double> y = data[y_col];
                if (chart_type == "line") {
                    plot_line(x, y, x_col, y_col);
                } else {
                    plot_bar(x, y, x_col, y_col);
                }
            }
        }
    } while (choice != 2);

    // Step 4: Decision Tree
    AI(table, headers);

    return 0;
}
