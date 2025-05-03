#pragma once
#include <string>
#include <vector>
#include <unordered_map>

std::string ask_chart_type();
std::string choose_column(const std::vector<std::string>& headers, const std::string& name);

std::unordered_map<std::string, std::vector<double>> extract_numeric_columns(
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& table,
    const std::vector<std::string>& columns);

void plot_line(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label);
void plot_bar(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label);
