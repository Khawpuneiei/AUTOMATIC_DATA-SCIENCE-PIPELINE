#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

std::unordered_map<std::string, std::vector<double>> read_csv(const std::string& filename) {
    std::unordered_map<std::string, std::vector<double>> data;
    std::ifstream file(filename);
    std::string line, cell;

    std::vector<std::string> headers;
    if (file.good()) {
        std::getline(file, line);
        std::stringstream ss(line);
        while (std::getline(ss, cell, ',')) {
            headers.push_back(cell);
            data[cell] = {};
        }
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int i = 0;
        while (std::getline(ss, cell, ',')) {
            try {
                data[headers[i++]].push_back(std::stod(cell));
            } catch (...) {
                data[headers[i - 1]].push_back(0.0); // fallback
            }
        }
    }

    return data;
}
