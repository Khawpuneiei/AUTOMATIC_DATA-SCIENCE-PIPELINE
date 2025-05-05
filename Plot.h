#pragma once  
#include <string>
#include <vector>
#include <unordered_map>

// Ask the user to choose the type of chart (line or bar)
std::string ask_chart_type();

// Let the user select a column from a list (used for X or Y axis)
std::string choose_column(const std::vector<std::string>& headers, const std::string& name);

// Extract numeric columns from the CSV-based hash table 
// Converts string values to double for plotting
std::unordered_map<std::string, std::vector<double>> extract_numeric_columns(
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& table,
    const std::vector<std::string>& columns);

// Plot a line graph with given X and Y values
void plot_line(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label);

// Plot a bar chart with given X and Y values
void plot_bar(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label);
