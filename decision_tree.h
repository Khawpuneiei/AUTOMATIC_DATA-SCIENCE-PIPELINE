#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

vector<string> split(const string& line);

unordered_map<string, unordered_map<string, string>> csvToHashTable(const string& filename);

struct TreeNode {
    string feature;
    string value;
    string label;
    double prediction = 0.0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

TreeNode* buildClassificationTree(
    const unordered_map<string, unordered_map<string, string>>& data,
    const vector<string>& features
);

TreeNode* buildRegressionTree(
    const unordered_map<string, unordered_map<string, string>>& data,
    const vector<string>& features,
    const string& target_column
);

string classify(TreeNode* node, const unordered_map<string, string>& row);
double regress(TreeNode* node, const unordered_map<string, string>& row);

#endif
