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

// Tree node structure
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
TreeNode* buildRegressionTree(
    const unordered_map<string, unordered_map<string, string>>& data,
    const vector<string>& features,
    const string& target
) {
    bool pure = true;
    double first_val = -1;
    for (auto& row : data) {
        double val = stod(row.second.at(target));
        if (first_val < 0) first_val = val;
        else if (val != first_val) { pure = false; break; }
    }
    if (pure || data.size() <= 2) {
        double sum = 0;
        for (auto& row : data)
            sum += stod(row.second.at(target));
        double avg = sum / data.size();
        return new TreeNode{ "", "", "", avg };
    }

    string bestFeature;
    string bestValue;
    double bestDifference = 1e9;

    for (auto& feat : features) {
        for (auto& row : data) {
            string val = row.second.at(feat);

            unordered_map<string, unordered_map<string, string>> left, right;
            for (auto& r : data) {
                if (r.second.at(feat) == val)
                    left[r.first] = r.second;
                else
                    right[r.first] = r.second;
            }
            if (left.empty() || right.empty()) continue;

            double left_sum = 0;
            for (auto& l : left)
                left_sum += stod(l.second.at(target));
            double left_avg = left_sum / left.size();

            double right_sum = 0;
            for (auto& r_ : right)
                right_sum += stod(r_.second.at(target));
            double right_avg = right_sum / right.size();

            double total_diff = 0;
            for (auto& l : left)
                total_diff += pow(stod(l.second.at(target)) - left_avg, 2);
            for (auto& r_ : right)
                total_diff += pow(stod(r_.second.at(target)) - right_avg, 2);

            if (total_diff < bestDifference) {
                bestDifference = total_diff;
                bestFeature = feat;
                bestValue = val;
            }
        }
    }

    if (bestFeature.empty()) {
        double sum = 0;
        for (auto& row : data)
            sum += stod(row.second.at(target));
        double avg = sum / data.size();
        return new TreeNode{ "", "", "", avg };
    }

    unordered_map<string, unordered_map<string, string>> left, right;
    for (auto& r : data) {
        if (r.second.at(bestFeature) == bestValue)
            left[r.first] = r.second;
        else
            right[r.first] = r.second;
    }

    return new TreeNode{
        bestFeature,
        bestValue,
        "",
        0.0,
        buildRegressionTree(left, features, target),
        buildRegressionTree(right, features, target)
    };
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
    string classA = y_true[0];
    string classB = "";
    for (auto label : y_true) {
        if (label != classA) {
            classB = label;
            break;
        }
    }

    int TP = 0, FP = 0, FN = 0, TN = 0;
    for (size_t i = 0; i < y_true.size(); ++i) {
        if (y_true[i] == classA && y_pred[i] == classA) TP++;
        else if (y_true[i] == classB && y_pred[i] == classA) FP++;
        else if (y_true[i] == classA && y_pred[i] == classB) FN++;
        else TN++;
    }

    double accuracy = (TP + TN) / double(y_true.size());

    cout << "Classes: " << classA << " and " << classB << endl;
    cout << "Accuracy: " << accuracy << endl;
}

// ---------- Regression Metrics ------------
void regressionMetrics(const vector<double>& y_true, const vector<double>& y_pred) {
    double mae = 0;
    for (size_t i = 0; i < y_true.size(); ++i)
        mae += abs(y_true[i] - y_pred[i]);
    mae /= y_true.size();
    cout << "Mean Absolute Error: " << mae << endl;
}
