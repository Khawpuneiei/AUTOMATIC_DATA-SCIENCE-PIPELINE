#include "Plot.h"
#include "matplotlibcpp.h"
#include <iostream>

namespace plt = matplotlibcpp;

// Ask user to select chart type
std::string ask_chart_type() {
    int choice;
    std::cout << "\nChoose type of graph:\n";
    std::cout << "1. Line Chart\n";
    std::cout << "2. Bar Chart\n";
    std::cout << "3. Categorical Bar Chart (e.g., by name)\n";
    std::cout << "Enter (1-3): ";
    std::cin >> choice;

    switch (choice) {
        case 1: return "line";
        case 2: return "bar";
        case 3: return "cat_bar";
        default:
            std::cout << "Invalid choice, defaulting to line chart.\n";
            return "line";
    }
}

// Let user choose column
std::string choose_column(const std::vector<std::string>& headers, const std::string& name) {
    std::cout << "\nAvailable columns for " << name << ":\n";
    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << i + 1 << ". " << headers[i] << "\n";
    }

    int choice;
    std::cout << "Choose column " << name << " (number): ";
    std::cin >> choice;
    return headers[choice - 1];
}

// Extract numeric columns from hash table
std::unordered_map<std::string, std::vector<double>> extract_numeric_columns(
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& table,
    const std::vector<std::string>& columns) {

    std::unordered_map<std::string, std::vector<double>> result;
    for (const auto& col : columns) {
        result[col] = {};
    }

    for (const auto& row : table) {
        for (const auto& col : columns) {
            try {
                result[col].push_back(std::stod(row.second.at(col)));
            } catch (...) {
                result[col].push_back(0); 
            }
        }
    }
    return result;
}

// Line chart
void plot_line(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label) {
    plt::plot(x, y);
    plt::title(y_label + " vs " + x_label);
    plt::xlabel(x_label);
    plt::ylabel(y_label);
    plt::grid(true);
    plt::show();
}

// Bar chart
void plot_bar(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label) {
    plt::bar(x, y);
    plt::title(y_label + " vs " + x_label);
    plt::xlabel(x_label);
    plt::ylabel(y_label);
    plt::grid(true);
    plt::show();
}

// Bar chart for categorical (string) X-axis
void plot_categorical_bar(const std::vector<std::string>& categories, const std::vector<double>& values, std::string x_label, std::string y_label) {
    std::vector<double> x;
    for (size_t i = 0; i < categories.size(); ++i) {
        x.push_back(i);
    }

    plt::bar(x, values);               
    plt::xticks(x, categories);        
    plt::title(y_label + " by " + x_label);
    plt::xlabel(x_label);
    plt::ylabel(y_label);
    plt::grid(true);
    plt::show();
}
