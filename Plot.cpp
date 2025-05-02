#include "Plot.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

void auto_plot(const std::unordered_map<std::string, std::vector<double>>& data) {
    if (data.size() < 2) return;

 
    auto it = data.begin();
    std::string x_label = it->first;
    std::vector<double> x = it->second;
    ++it;

    for (; it != data.end(); ++it) {
        const std::string& y_label = it->first;
        const std::vector<double>& y = it->second;

        plt::figure(); 
        plt::plot(x, y); 
        plt::title(y_label + " vs " + x_label);
        plt::xlabel(x_label);
        plt::ylabel(y_label);
        plt::show();
    }
}
