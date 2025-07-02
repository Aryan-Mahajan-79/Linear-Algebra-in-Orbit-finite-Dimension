#include "input_data.h"
#include<stdexcept>
#include<iostream>
#include <iomanip>

void InputData::addRow(const std::string& name, int value) {
    rows_.emplace_back(name, value);
}

void InputData::addColumn(const std::string& name, int value) {
    columns_.emplace_back(name, value);
}

void InputData::setTarget(const std::string& key, int value) {
    target_[key] = value;
}

void InputData::setDefaultValue(const std::string& row, const std::string& col, int value) {
    auto key = std::make_pair(row, col);
    if (default_values_.count(key)) {
        std::cerr << "⚠️ Warning: Default value for (" << row << ", " << col << ") already defined. Skipping.\n";
        return;
    }
    default_values_[key] = value;
}

void InputData::addPairing(const PatternKey& key, int value) {
    if (pairings_.count(key)) {
        std::cerr << "⚠️ Warning: Duplicate pairing for (" << key.getRow() << ", " << key.getColumn() << "). Skipping.\n";
        return;
    }
    pairings_[key] = value;
}



void InputData::addPairing(const std::string& row, const std::string& col,
                           const std::vector<PatternKey::PatternElement>& pattern, int value) {
    pairings_[PatternKey(row, col, pattern)] = value;
}


const std::vector<InputData::RowOrCol>& InputData::getRows() const { return rows_; }
const std::vector<InputData::RowOrCol>& InputData::getColumns() const { return columns_; }
const std::map<std::string, int>& InputData::getTarget() const { return target_; }
const std::map<std::pair<std::string, std::string>, int>& InputData::getDefaultValues() const { return default_values_; }
const std::map<PatternKey, int>& InputData::getPairings() const { return pairings_; }


const int InputData::getDefaultValue(const std::string& row, const std::string& col) const {
    auto key = std::make_pair(row, col);
    auto it = default_values_.find(key);
    if (it != default_values_.end()) {
        return it->second;
    }
    throw std::runtime_error("Default value not found for (" + row + ", " + col + ")");
}

const int InputData::getPairingValue(const std::string& row,
                               const std::string& col,
                               const std::vector<PatternKey::PatternElement>& pattern) const {
    PatternKey key(row, col, pattern);
    auto it = pairings_.find(key);
    if (it != pairings_.end()) {
        return it->second;
    }

    std::cerr << "Pairing value not found for (" << row << ", " << col << ") with pattern: [";
    for (size_t i = 0; i < pattern.size(); ++i) {
        std::visit([](auto&& arg) {
            std::cerr << arg;
        }, pattern[i]);

        if (i + 1 < pattern.size()) {
            std::cerr << ", ";
        }
    }
    std::cerr << "]" << std::endl;
    throw std::runtime_error("Pairing value not found for (" + row + ", " + col + ")");
}


void InputData::print() const {
    std::cout << "=== Input Data ===\n";

    std::cout << "\nRows:\n";
    for (const auto& [name, size] : rows_) {
        std::cout << "  " << name << ": " << size << '\n';
    }

    std::cout << "\nColumns:\n";
    for (const auto& [name, size] : columns_) {
        std::cout << "  " << name << ": " << size << '\n';
    }

    std::cout << "\nTargets:\n";
    for (const auto& [name, val] : target_) {
        std::cout << "  " << name << ": " << val << '\n';
    }

    std::cout << "\nDefault Values:\n";
    for (const auto& [key, val] : default_values_) {
        std::cout << "  (" << key.first << ", " << key.second << ") = " << val << '\n';
    }

    std::cout << "\nPairings:\n";
    for (const auto& [key, val] : pairings_) {
        std::cout << "  (" << key.getRow() << ", " << key.getColumn() << ", [";

        const auto& pattern = key.getPattern();
        for (size_t i = 0; i < pattern.size(); ++i) {
            if (std::holds_alternative<int>(pattern[i])) {
                std::cout << std::get<int>(pattern[i]);
            } else {
                std::cout << "'" << std::get<char>(pattern[i]) << "'";
            }
            if (i + 1 < pattern.size()) std::cout << ", ";
        }

        std::cout << "]) = " << val << '\n';
    }

    std::cout << "==================\n";
}
