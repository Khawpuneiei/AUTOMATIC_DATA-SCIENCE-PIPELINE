#pragma once
#include <string>
#include <vector>

std::string ask_chart_type();
std::string choose_column(const std::vector<std::string>& headers, const std::string& name);
void plot_line(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label);
void plot_bar(const std::vector<double>& x, const std::vector<double>& y, std::string x_label, std::string y_label);
