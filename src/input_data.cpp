#include "input_data.h"
#include<stdexcept>
#include<iostream>

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
