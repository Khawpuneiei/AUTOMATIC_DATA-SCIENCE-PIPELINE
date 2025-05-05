#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <unordered_map>

using namespace std;

// Node structure for the Decision Tree
struct TreeNode {
    string feature;  // which feature to split on
    string value;    // to compare for the split
    string label;    // classification output
    double prediction = 0.0; // for regression leaves

    TreeNode* left = nullptr;   // left child
    TreeNode* right = nullptr;  // right child
};

// simple decision tree for classification
TreeNode* buildClassificationTree(
    const unordered_map<string, unordered_map<string, string>>& data,
    const vector<string>& features
) {
    
    string first_label;
    bool pure = true;
    for (const auto& row : data) {
        string label = row.second.at("label"); 
        if (first_label.empty()) first_label = label;
        else if (label != first_label) {
            pure = false;
            break;
        }
    }

    if (pure) {
        TreeNode* leaf = new TreeNode;
        leaf->label = first_label;
        return leaf;
    }

    
    string bestFeature = features[0];
    string bestValue = "";

    for (const auto& row : data) {
        bestValue = row.second.at(bestFeature);
        break;  
    }

    // Split data
    unordered_map<string, unordered_map<string, string>> left_data;
    unordered_map<string, unordered_map<string, string>> right_data;

    for (const auto& row : data) {
        if (row.second.at(bestFeature) == bestValue)
            left_data[row.first] = row.second;
        else
            right_data[row.first] = row.second;
    }

    TreeNode* node = new TreeNode;
    node->feature = bestFeature;
    node->value = bestValue;
    node->left = buildClassificationTree(left_data, features);
    node->right = buildClassificationTree(right_data, features);

    return node;
}

// simple decision tree for regression
TreeNode* buildRegressionTree(
    const unordered_map<string, unordered_map<string, string>>& data,
    const vector<string>& features,
    const string& target_column
) {
    
    double first_value = -1;
    bool pure = true;
    for (const auto& row : data) {
        double value = stod(row.second.at(target_column));
        if (first_value < 0) first_value = value;
        else if (value != first_value) {
            pure = false;
            break;
        }
    }

    if (pure) {
        TreeNode* leaf = new TreeNode;
        leaf->prediction = first_value;
        return leaf;
    }

    
    string bestFeature = features[0];
    string bestValue = "";

    for (const auto& row : data) {
        bestValue = row.second.at(bestFeature);
        break;
    }

    // Split data
    unordered_map<string, unordered_map<string, string>> left_data;
    unordered_map<string, unordered_map<string, string>> right_data;

    for (const auto& row : data) {
        if (row.second.at(bestFeature) == bestValue)
            left_data[row.first] = row.second;
        else
            right_data[row.first] = row.second;
    }

    TreeNode* node = new TreeNode;
    node->feature = bestFeature;
    node->value = bestValue;
    node->left = buildRegressionTree(left_data, features, target_column);
    node->right = buildRegressionTree(right_data, features, target_column);

    return node;
}

