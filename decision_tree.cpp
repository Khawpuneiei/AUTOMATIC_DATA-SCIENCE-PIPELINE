#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

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

// ---------- CSV Split ------------
vector<string> split(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) result.push_back(item);
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

// ---------- Decision Tree Node ------------
struct TreeNode {
    string feature, value, label;
    double prediction = 0.0;
    TreeNode *left = nullptr, *right = nullptr;
};

// ---------- Classification Tree ------------
TreeNode* buildClassificationTree(const unordered_map<string, unordered_map<string, string>>& data, const vector<string>& features) {
    string first_label; bool pure = true;
    for (auto& row : data) {
        string lbl = row.second.at("label");
        if (first_label.empty()) first_label = lbl;
        else if (lbl != first_label) { pure = false; break; }
    }
    if (pure) return new TreeNode{ "", "", first_label };

    string feat = features[0], val = data.begin()->second.at(feat);
    unordered_map<string, unordered_map<string, string>> left, right;
    for (auto& row : data)
        (row.second.at(feat) == val ? left : right)[row.first] = row.second;

    return new TreeNode{ feat, val, "", 0.0, buildClassificationTree(left, features), buildClassificationTree(right, features) };
}

// ---------- Regression Tree ------------
TreeNode* buildRegressionTree(const unordered_map<string, unordered_map<string, string>>& data, const vector<string>& features, const string& target) {
    double first_val = -1; bool pure = true;
    for (auto& row : data) {
        double val = stod(row.second.at(target));
        if (first_val < 0) first_val = val;
        else if (val != first_val) { pure = false; break; }
    }
    if (pure) return new TreeNode{ "", "", "", first_val };

    string feat = features[0], val = data.begin()->second.at(feat);
    unordered_map<string, unordered_map<string, string>> left, right;
    for (auto& row : data)
        (row.second.at(feat) == val ? left : right)[row.first] = row.second;

    return new TreeNode{ feat, val, "", 0.0, buildRegressionTree(left, features, target), buildRegressionTree(right, features, target) };
}

// ---------- Predict Classification ------------
string classify(TreeNode* node, const unordered_map<string, string>& row) {
    if (!node->left && !node->right) return node->label;
    return row.at(node->feature) == node->value ? classify(node->left, row) : classify(node->right, row);
}

// ---------- Predict Regression ------------
double regress(TreeNode* node, const unordered_map<string, string>& row) {
    if (!node->left && !node->right) return node->prediction;
    return row.at(node->feature) == node->value ? regress(node->left, row) : regress(node->right, row);
}

// ---------- Classification Metrics ------------
void classificationMetrics(const vector<string>& y_true, const vector<string>& y_pred) {
    int TP = 0, FP = 0, FN = 0, TN = 0;
    for (size_t i = 0; i < y_true.size(); ++i) {
        if (y_true[i] == "Yes" && y_pred[i] == "Yes") TP++;
        else if (y_true[i] == "No" && y_pred[i] == "Yes") FP++;
        else if (y_true[i] == "Yes" && y_pred[i] == "No") FN++;
        else TN++;
    }
    double accuracy = (TP + TN) / double(y_true.size());
    double precision = TP / double(TP + FP + 1e-9);
    double recall = TP / double(TP + FN + 1e-9);
    double f1 = 2 * precision * recall / (precision + recall + 1e-9);

    cout << "Accuracy: " << accuracy << endl;
    cout << "Precision: " << precision << endl;
    cout << "Recall: " << recall << endl;
    cout << "F1 Score: " << f1 << endl;
}

// ---------- Regression Metrics ------------
void regressionMetrics(const vector<double>& y_true, const vector<double>& y_pred) {
    double mae = 0;
    for (size_t i = 0; i < y_true.size(); ++i)
        mae += abs(y_true[i] - y_pred[i]);
    mae /= y_true.size();
    cout << "Mean Absolute Error: " << mae << endl;
}
