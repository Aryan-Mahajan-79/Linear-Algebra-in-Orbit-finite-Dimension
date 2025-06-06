#include "input_validation.h"
#include <set>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <cctype>

bool is_only_spaces(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); });
}

bool validate_orbit_names(const InputData& input_data) {
    std::set<std::string> seen;
    bool duplicate_found = false;
    bool empty_name = false;
    if(input_data.rows.size() == 0 || input_data.cols.size() == 0) return false;
    // Check rows
    for (const auto& [name, _] : input_data.rows) {
        if(name.empty() || is_only_spaces(name)) {
            empty_name = true;
        }
        if (!seen.insert(name).second) {
            duplicate_found = true;
        }
    }
    seen.clear();
    // Check cols
    for (const auto& [name, _] : input_data.cols) {
        if(name.empty() || is_only_spaces(name)) {
            empty_name = true;
        }
        if (!seen.insert(name).second) {
            duplicate_found = true;
        }
    }

    if (duplicate_found) {
        std::cerr << "❌ Duplicate names found in rows/cols!" << std::endl;
    }
    if(empty_name){
        std::cerr << "❌ Empty name found in rows/cols!" << std::endl;
    }

    return !(duplicate_found || empty_name);
}


bool validate_names(const InputData& input_data) {
    std::unordered_set<std::string> valid_rows, valid_cols;

    for (const auto& [row_name, _] : input_data.rows) {
        valid_rows.insert(row_name);
    }
    for (const auto& [col_name, _] : input_data.cols) {
        valid_cols.insert(col_name);
    }

    bool all_valid = true;

    // Check target row names
    for (const auto& [row_name, _] : input_data.target) {
        if (!valid_rows.count(row_name)) {
            std::cerr << "❌ Invalid row name in target: " << row_name << std::endl;
            all_valid = false;
        }
    }

    // Check default_values (row, col) pairs
    for (const auto& [key, _] : input_data.default_values) {
        const auto& [row_name, col_name] = key;
        if (!valid_rows.count(row_name)) {
            std::cerr << "❌ Invalid row name in default_values: " << row_name << std::endl;
            all_valid = false;
        }
        if (!valid_cols.count(col_name)) {
            std::cerr << "❌ Invalid column name in default_values: " << col_name << std::endl;
            all_valid = false;
        }
    }

    // Check pairings (row, col) pairs
    for (const auto& [key, _] : input_data.pairings) {
        const auto& [row_name, col_name, _path] = key;
        if (!valid_rows.count(row_name)) {
            std::cerr << "❌ Invalid row name in pairings: " << row_name << std::endl;
            all_valid = false;
        }
        if (!valid_cols.count(col_name)) {
            std::cerr << "❌ Invalid column name in pairings: " << col_name << std::endl;
            all_valid = false;
        }
    }

    return all_valid;
}

bool validate_unique_targets(const InputData& input_data) {
    std::unordered_set<std::string> seen_names;
    bool duplicate_found = false;

    if(input_data.rows.size() != input_data.target.size()){
        return false;
    }
    for (const auto& [row_name, target_value] : input_data.target) {
        if (!seen_names.insert(row_name).second) {
            std::cerr << "❌ Duplicate target found for row : " << row_name << std::endl;
            duplicate_found = true;
        }
    }

    return !duplicate_found;
}


bool check_input_validity(const InputData& input_data) {
    if(!validate_orbit_names(input_data) || !validate_names(input_data))
        return false;
    if(!validate_unique_targets(input_data))
        return false;

    return true;
}
