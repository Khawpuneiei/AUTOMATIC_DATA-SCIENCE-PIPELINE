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

// ---------- CSV to Hash Table ------------
unordered_map<string, unordered_map<string, string>> csvToHashTable(const string& filename, vector<string>& headers) {
    unordered_map<string, unordered_map<string, string>> table;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        exit(1);
    }

    getline(file, line);
    headers = split(line);

    while (getline(file, line)) {
        vector<string> values = split(line);
        if (values.size() == headers.size()) {
            string key = values[0];
            unordered_map<string, string> row;
            for (size_t i = 1; i < headers.size(); ++i)
                row[headers[i]] = values[i];
            table[key] = row;
        }
    }

    return table;
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
    do {
        printHashTable(table);

        cout << "Columns available:\n";
        for (auto& h : headers) cout << h << " ";
        cout << endl;

        string id_column;
        cout << "Please enter the ID column: ";
        cin >> id_column;

        string target;
        while (true) {
            cout << "Please enter your target column: ";
            cin >> target;

            set<string> unique_classes;
            for (auto& row : table)
                unique_classes.insert(row.second.at(target));

            if (unique_classes.size() == 2) {
                cout << "This column can be used for classification." << endl;
                break;
            } else {
                try {
                    stod(table.begin()->second.at(target));
                    cout << "This is a regression target." << endl;
                    break;
                } catch (...) {
                    cout << "This column has more than two classes and is not numeric. Please choose another column." << endl;
                }
            }
        }

        vector<string> features;
        for (auto& h : headers)
            if (h != target && h != id_column)
                features.push_back(h);

        for (auto& row : table)
            row.second["label"] = row.second[target];

        bool is_regression = true;
        try { stod(table.begin()->second.at(target)); }
        catch (...) { is_regression = false; }

        vector<string> keys;
        for (auto& row : table) keys.push_back(row.first);
        srand(time(0));

        int split = int(keys.size() * 0.8);
        unordered_map<string, unordered_map<string, string>> train, test;
        for (int i = 0; i < split; ++i) train[keys[i]] = table[keys[i]];
        for (int i = split; i < keys.size(); ++i) test[keys[i]] = table[keys[i]];

        TreeNode* tree = nullptr;
        if (!is_regression) {
            cout << "Detected task: Classification\n";
            tree = buildClassificationTree(train, features);
        } else {
            cout << "Detected task: Regression\n";
            tree = buildRegressionTree(train, features, target);
        }

        if (!is_regression) {
            vector<string> y_true, y_pred;
            cout << "\n=== Predictions (Classification) ===\n";
            for (auto& row : test) {
                string true_label = row.second.at(target);
                string pred_label = classify(tree, row.second);
                y_true.push_back(true_label);
                y_pred.push_back(pred_label);
                cout << "True Label: " << true_label << ", Predicted: " << pred_label << endl;
            }
            classificationMetrics(y_true, y_pred);
        } else {
            vector<double> y_true, y_pred;
            cout << "\n=== Predictions (Regression) ===\n";
            for (auto& row : test) {
                double true_val = stod(row.second.at(target));
                double pred_val = regress(tree, row.second);
                y_true.push_back(true_val);
                y_pred.push_back(pred_val);
                cout << "True Value: " << true_val << ", Predicted: " << pred_val << endl;
            }
            regressionMetrics(y_true, y_pred);
        }

    } while (false);

    return 0;
}
