#ifndef PIPELINE_H
#define PIPELINE_H

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

struct TreeNode {
    string feature, value, label;
    double prediction = 0.0;
    TreeNode *left = nullptr, *right = nullptr;
};

TreeNode* buildClassificationTree(const unordered_map<string, unordered_map<string, string>>& data, const vector<string>& features);
TreeNode* buildRegressionTree(const unordered_map<string, unordered_map<string, string>>& data, const vector<string>& features, const string& target);

string classify(TreeNode* node, const unordered_map<string, string>& row);
double regress(TreeNode* node, const unordered_map<string, string>& row);

void classificationMetrics(const vector<string>& y_true, const vector<string>& y_pred);
void regressionMetrics(const vector<double>& y_true, const vector<double>& y_pred);

#endif
