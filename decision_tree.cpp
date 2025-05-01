#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_ROW = 100;
const int MAX_COL = 50;

// CSV Reader feature
void readCSV(const string &filename, string arr[MAX_ROW][MAX_COL], int &rows, int &cols) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        return;
    }

    string line, cell;
    rows = 0;
    cols = 0;

    while (getline(file, line) && rows < MAX_ROW) {
        stringstream lineStream(line);
        int col = 0;

        while (getline(lineStream, cell, ',') && col < MAX_COL) {
            arr[rows][col] = cell;
            col++;
        }

        if (col > cols) cols = col;
        rows++;
    }

    file.close();
}

// Decision Tree with Tree Structure 
struct TreeNode {
    string feature;
    string label;
    map<string, TreeNode*> children;

    bool isLeaf() const {
        return children.empty();
    }
};

// Utility Functions
int getFeatureIndex(const string arr[MAX_ROW][MAX_COL], int cols, const string &featureName) {
    for (int i = 0; i < cols; ++i) {
        if (arr[0][i] == featureName) return i;
    }
    return -1;
}

vector<int> getUniqueValues(const string arr[MAX_ROW][MAX_COL], int rows, int colIndex) {
    map<string, bool> unique;
    for (int i = 1; i < rows; ++i) {
        unique[arr[i][colIndex]] = true;
    }
    return vector<int>(unique.size(), 0);
}

// Count how many times each label appears
map<string, int> countLabels(const string arr[MAX_ROW][MAX_COL], int rows, int targetCol) {
    map<string, int> counts;
    for (int i = 1; i < rows; ++i) {
        counts[arr[i][targetCol]]++;
    }
    return counts;
}

// Entropy 
double entropy(const string arr[MAX_ROW][MAX_COL], int rows, int targetCol) {
    map<string, int> counts = countLabels(arr, rows, targetCol);
    double entropy = 0.0;
    int total = rows - 1;

    for (const auto &pair : counts) {
        double p = (double)pair.second / total;
        entropy -= p * log2(p);
    }

    return entropy;
}

// Split data
void splitDataset(const string arr[MAX_ROW][MAX_COL], int rows, int cols, int featureIndex, const string &value,
                  string subArr[MAX_ROW][MAX_COL], int &subRows) {
    subRows = 1; 
    for (int i = 0; i < cols; ++i) {
        subArr[0][i] = arr[0][i];
    }

    for (int i = 1; i < rows; ++i) {
        if (arr[i][featureIndex] == value) {
            for (int j = 0; j < cols; ++j) {
                subArr[subRows][j] = arr[i][j];
            }
            subRows++;
        }
    }
}

//Building Tree
TreeNode* buildTree(string arr[MAX_ROW][MAX_COL], int rows, int cols, int targetCol) {
    // Check if all labels are the same
    bool pure = true;
    for (int i = 2; i < rows; ++i) {
        if (arr[i][targetCol] != arr[1][targetCol]) {
            pure = false;
            break;
        }
    }

    if (pure) {
        TreeNode *leaf = new TreeNode;
        leaf->label = arr[1][targetCol];
        return leaf;
    }

    double baseEntropy = entropy(arr, rows, targetCol);
    double bestInfoGain = 0.0;
    int bestFeature = -1;

    for (int i = 0; i < cols; ++i) {
        if (i == targetCol) continue;

        map<string, int> featureValues;
        for (int j = 1; j < rows; ++j) {
            featureValues[arr[j][i]]++;
        }

        double newEntropy = 0.0;
        for (auto &pair : featureValues) {
            string subArr[MAX_ROW][MAX_COL];
            int subRows;
            splitDataset(arr, rows, cols, i, pair.first, subArr, subRows);
            double p = (double)(subRows - 1) / (rows - 1);
            newEntropy += p * entropy(subArr, subRows, targetCol);
        }

        double infoGain = baseEntropy - newEntropy;

        if (infoGain > bestInfoGain) {
            bestInfoGain = infoGain;
            bestFeature = i;
        }
    }

    if (bestFeature == -1) {
        TreeNode *leaf = new TreeNode;
        leaf->label = arr[1][targetCol];
        return leaf;
    }

    TreeNode *node = new TreeNode;
    node->feature = arr[0][bestFeature];

    map<string, int> bestFeatureValues;
    for (int j = 1; j < rows; ++j) {
        bestFeatureValues[arr[j][bestFeature]]++;
    }

    for (auto &pair : bestFeatureValues) {
        string subArr[MAX_ROW][MAX_COL];
        int subRows;
        splitDataset(arr, rows, cols, bestFeature, pair.first, subArr, subRows);
        node->children[pair.first] = buildTree(subArr, subRows, cols, targetCol);
    }

    return node;
}

void printTree(TreeNode *node, string indent = "") {
    if (node->isLeaf()) {
        cout << indent << "Label: " << node->label << endl;
        return;
    }

    cout << indent << "Feature: " << node->feature << endl;
    for (auto &pair : node->children) {
        cout << indent << "- Value: " << pair.first << endl;
        printTree(pair.second, indent + "  ");
    }
}

int main() {
    string filename = "fruit.csv";
    string arr[MAX_ROW][MAX_COL];
    int rows = 0, cols = 0;

    readCSV(filename, arr, rows, cols);

    int targetCol = cols - 1; 
    TreeNode *tree = buildTree(arr, rows, cols, targetCol);

    cout << "\n=== Decision Tree ===\n";
    printTree(tree);

    return 0;
}
