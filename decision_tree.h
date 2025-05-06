#ifndef DATAPIPELINE_H        
#define DATAPIPELINE_H

#include <iostream>           
#include <fstream>            
#include <sstream>            
#include <unordered_map>      
#include <vector>            
#include <string>             
#include <set>                
#include <cmath>              
#include <cstdlib>           
#include <ctime>           

using namespace std;

inline void printHashTable(
        const unordered_map<string, unordered_map<string,string>>& hashTable)
{
    cout << "Hash Table Contents:\n";
    for (const auto& pair : hashTable) {            
        cout << "Key: " << pair.first << " | Values: ";
        for (const auto& valuePair : pair.second)  
            cout << valuePair.first << "=" << valuePair.second << ' ';
        cout << '\n';
    }
}

inline vector<string> split(const string& line)
{
    vector<string> result;               
    stringstream ss(line);              
    string item;
    while (getline(ss, item, ','))       
        result.push_back(item);          
    return result;                    
}

inline unordered_map<string, unordered_map<string,string>>
csvToHashTable(const string& filename, vector<string>& headers)
{
    unordered_map<string, unordered_map<string,string>> table; 
    ifstream file(filename);                                   
    string line;

    if (!file.is_open()) {            
        cout << "Error opening file.\n";
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

struct TreeNode {
    string feature;     
    string value;       
    string label;       
    double prediction; 
    TreeNode* left  = nullptr;   
    TreeNode* right = nullptr;   
};

inline TreeNode* buildClassificationTree(
        const unordered_map<string, unordered_map<string,string>>& data,
        const vector<string>& features)
{
    string first_label; bool pure = true;      
    for (auto& row : data) {                  
        string lbl = row.second.at("label");
        if (first_label.empty()) first_label = lbl;
        else if (lbl != first_label) { pure = false; break; }
    }
    if (pure)                                
        return new TreeNode{"","",first_label};

    string feat = features[0],
           val  = data.begin()->second.at(feat);

    unordered_map<string, unordered_map<string,string>> left, right;
    for (auto& row : data)                              
        (row.second.at(feat) == val ? left : right)[row.first] = row.second;

    return new TreeNode{feat,val,"",0.0,
                        buildClassificationTree(left,  features),
                        buildClassificationTree(right, features)};
}

inline TreeNode* buildRegressionTree(
        const unordered_map<string, unordered_map<string,string>>& data,
        const vector<string>& features,
        const string& target)
{
    bool pure = true;
    double first_val = -1;
    for (auto& row : data) {
        double v = stod(row.second.at(target));
        if (first_val < 0) first_val = v;
        else if (v != first_val) { pure = false; break; }
    }
    if (pure || data.size() <= 2) {          
        double sum = 0;
        for (auto& row : data) sum += stod(row.second.at(target));
        return new TreeNode{"","", "", sum / data.size()}; 
    }

    string bestFeature, bestValue;
    double bestError = 1e9;

    for (const string& feat : features) {
        for (const auto& rowA : data) {
            string splitVal = rowA.second.at(feat);

            unordered_map<string, unordered_map<string,string>> left,right;
            for (auto& rowB : data)
                (rowB.second.at(feat) == splitVal ? left : right)[rowB.first]=rowB.second;
            if (left.empty() || right.empty()) continue;   

            auto mseSide = [&](const auto& subset){
                double sum=0; for(auto& r:subset) sum+=stod(r.second.at(target));
                double mean = sum / subset.size();
                double mse = 0;
                for(auto& r:subset)
                    mse += pow(stod(r.second.at(target)) - mean, 2);
                return mse;
            };
            double totalError = mseSide(left) + mseSide(right);

            if (totalError < bestError) {
                bestError   = totalError;
                bestFeature = feat;
                bestValue   = splitVal;
            }
        }
    }

    if (bestFeature.empty()) {
        double sum = 0; for (auto& row : data) sum += stod(row.second.at(target));
        return new TreeNode{"","", "", sum / data.size()};
    }

    unordered_map<string, unordered_map<string,string>> left,right;
    for (auto& r : data)
        (r.second.at(bestFeature) == bestValue ? left : right)[r.first] = r.second;

    return new TreeNode{bestFeature,bestValue,"",0.0,
                        buildRegressionTree(left,  features, target),
                        buildRegressionTree(right, features, target)};
}

inline string classify(TreeNode* node, const unordered_map<string,string>& row)
{
    if (!node->left && !node->right) return node->label;           
    return row.at(node->feature) == node->value
           ? classify(node->left,  row) : classify(node->right, row);
}

inline double regress(TreeNode* node, const unordered_map<string,string>& row)
{
    if (!node->left && !node->right) return node->prediction;      
    return row.at(node->feature) == node->value
           ? regress(node->left,  row) : regress(node->right, row);
}

inline void classificationMetrics(const vector<string>& y_true,
                                  const vector<string>& y_pred)
{
    string classA = y_true[0], classB = "";          
    for (const auto& l : y_true) if (l != classA) { classB = l; break; }

    int TP=0, FP=0, FN=0, TN=0;
    for (size_t i=0;i<y_true.size();++i){
        if (y_true[i]==classA && y_pred[i]==classA) TP++;
        else if (y_true[i]==classB && y_pred[i]==classA) FP++;
        else if (y_true[i]==classA && y_pred[i]==classB) FN++;
        else TN++;
    }
    double accuracy = (TP+TN) / double(y_true.size());
    cout << "Classes: " << classA << " vs " << classB << '\n';
    cout << "Accuracy: " << accuracy << '\n';
}

inline void regressionMetrics(const vector<double>& y_true,
                              const vector<double>& y_pred)
{
    double mae = 0;
    for (size_t i=0;i<y_true.size();++i)
        mae += abs(y_true[i]-y_pred[i]);
    cout << "Mean Absolute Error: " << mae / y_true.size() << '\n';
}

inline void AI(
    unordered_map<string, unordered_map<string, string>>& table,
    const vector<string>& headers)
{
    printHashTable(table);

    cout << "Columns available:" << endl;
    for (const auto& h : headers) {
        cout << h << " ";       
    }
    cout << endl;

    string id_column;
    cout << "Please enter the ID column: ";
    cin >> id_column;                 

    string target;
    while (true) {
        cout << "Please enter your target column: ";
        cin >> target;               

        set<string> unique_classes;
        for (const auto& row : table) {
            unique_classes.insert(row.second.at(target));
        }

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
    for (const auto& h : headers) {
        if (h != target && h != id_column)
            features.push_back(h); 
    }

    for (auto& row : table) {
        row.second["label"] = row.second[target];
    }

    bool is_regression = true;
    try {
        stod(table.begin()->second.at(target));
    } catch (...) {
        is_regression = false;   
    }

    vector<string> keys;
    for (const auto& row : table) {
        keys.push_back(row.first);
    }
    srand(time(0));             

    int split = static_cast<int>(keys.size() * 0.8);
    unordered_map<string, unordered_map<string, string>> train, test;
    for (int i = 0; i < split; ++i) {
        train[keys[i]] = table[keys[i]];
    }
    for (int i = split; i < static_cast<int>(keys.size()); ++i) {
        test[keys[i]] = table[keys[i]];
    }

    TreeNode* tree = nullptr;
    if (!is_regression) {
        cout << "Detected task: Classification" << endl;
        tree = buildClassificationTree(train, features);
    } else {
        cout << "Detected task: Regression" << endl;
        tree = buildRegressionTree(train, features, target);
    }

    if (!is_regression) {
        vector<string> y_true, y_pred;
        cout << "\n=== Predictions (Classification) ===" << endl;
        for (const auto& row : test) {
            string true_label = row.second.at(target);
            string pred_label = classify(tree, row.second);
            y_true.push_back(true_label);
            y_pred.push_back(pred_label);
            cout << "True Label: " << true_label << ", Predicted: " << pred_label << endl;
        }
        classificationMetrics(y_true, y_pred); 
    } else {
        vector<double> y_true, y_pred;
        cout << "\n=== Predictions (Regression) ===" << endl;
        for (const auto& row : test) {
            double true_val = stod(row.second.at(target));
            double pred_val = regress(tree, row.second);
            y_true.push_back(true_val);
            y_pred.push_back(pred_val);
            cout << "True Value: " << true_val << ", Predicted: " << pred_val << endl;
        }
        regressionMetrics(y_true, y_pred); 
    }
}
#endif 
