#include "Plot.h"                  
#include "matplotlibcpp.h"        
#include <iostream>

namespace plt = matplotlibcpp;    

// Ask user to choose the type of chart to plot
std::string ask_chart_type() {
    int choice;
    std::cout << "\nChoose type of graph:\n";
    std::cout << "1. Line Chart\n";
    std::cout << "2. Bar Chart\n";
    std::cout << "Enter (1-2): ";
    std::cin >> choice;

    switch (choice) {
        case 1: return "line";
        case 2: return "bar";
        default:
            std::cout << "Invalid input. Defaulting to line chart.\n";
            return "line";
    }
}

// Let the user choose which column to use for X or Y axis
std::string choose_column(const std::vector<std::string>& headers, const std::string& name) {
    std::cout << "\nAvailable columns:\n";
    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << i + 1 << ". " << headers[i] << "\n";
    }

    int choice;
    std::cout << "Choose column for " << name << " (number): ";
    std::cin >> choice;

    return headers[choice - 1];
}

// Plot a line chart with given x and y values and labels
void plot_line(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label) {
    plt::plot(x, y);
    plt::title(y_label + " vs " + x_label);
    plt::xlabel(x_label);
    plt::ylabel(y_label);
    plt::grid(true);
    plt::show();
}

// Plot a bar chart with given x and y values and labels
void plot_bar(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label) {
    plt::bar(x, y);
    plt::title(y_label + " vs " + x_label);
    plt::xlabel(x_label);
    plt::ylabel(y_label);
    plt::grid(true);
    plt::show();
}

// Extract numeric columns from a hash table and convert them into vectors of doubles
std::unordered_map<std::string, std::vector<double>> extract_numeric_columns(
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& table,
    const std::vector<std::string>& columns) {

    std::unordered_map<std::string, std::vector<double>> result;

    // Initialize result vectors
    for (const auto& col : columns) {
        result[col] = {};
    }

    // Iterate through rows and extract numeric values
    for (const auto& row : table) {
        for (const auto& col : columns) {
            try {
                result[col].push_back(std::stod(row.second.at(col)));  // Convert string to double
            } catch (...) {
                result[col].push_back(0);  // Fallback if conversion fails
            }
        }
    }

    return result;
}
