#include "Plot.h"
#include "matplotlibcpp.h"
#include <iostream>

namespace plt = matplotlibcpp;

std::string ask_chart_type() {
    int choice;
    std::cout << "\nChoose type of graph:\n";
    std::cout << "1.(Line Chart)\n";
    std::cout << "2.(Bar Chart)\n";
    std::cout << "Enter (1-2): ";
    std::cin >> choice;

    switch (choice) {
        case 1: return "line";
        case 2: return "bar";
        default:
            std::cout << "wrong number will use line bar in defualt\n";
            return "line";
    }
}

std::string choose_column(const std::vector<std::string>& headers, const std::string& name) {
    std::cout << "\n column:\n";
    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << i + 1 << ". " << headers[i] << "\n";
    }

    int choice;
    std::cout << " Choose column " << name << " (number): ";
    std::cin >> choice;
    return headers[choice - 1];
}

void plot_line(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label) {
    plt::plot(x, y);
    plt::title(y_label + " vs " + x_label);
    plt::xlabel(x_label);
    plt::ylabel(y_label);
    plt::grid(true);
    plt::show();
}

void plot_bar(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label) {
    plt::bar(x, y);
    plt::title(y_label + " vs " + x_label);
    plt::xlabel(x_label);
    plt::ylabel(y_label);
    plt::grid(true);
    plt::show();
}
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
