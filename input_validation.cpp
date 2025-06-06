#include "input_validation.h"
#include <set>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <variant>
#include <vector>

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

int get_col_size(const InputData& data, const std::string& col_name) {
    for (const auto& [name, size] : data.cols) {
        if (name == col_name) return size;
    }
    throw std::runtime_error("Column not found: " + col_name);
}

int get_row_size(const InputData& data, const std::string& row_name) {
    for (const auto& [name, size] : data.rows) {
        if (name == row_name) return size;
    }
    throw std::runtime_error("Row not found: " + row_name);
}


bool validate_pairings(const InputData& data) {
    bool valid = true;

    for (const auto& [key, value] : data.pairings) {
        const auto& [row_name, col_name, pattern] = key;

        // Check that column and row exist
        int col_size, row_size;
        try {
            col_size = get_col_size(data, col_name);
            row_size = get_row_size(data, row_name);
        } catch (const std::exception& e) {
            std::cerr << "❌ " << e.what() << '\n'; 
            valid = false;
            continue;
        }

        // 1. Validate pattern length matches column size
        if (static_cast<int>(pattern.size()) != col_size) {
            std::cerr << "❌ Pattern size mismatch for column \"" << col_name 
                      << "\". Expected size: " << col_size 
                      << ", got: " << pattern.size() << "\n";
            valid = false;
        }

        // 2. Validate integers are in range [1, row_size]
        for (const auto& p : pattern) {
            if (std::holds_alternative<int>(p)) {
                int val = std::get<int>(p);
                if (val < 1 || val > row_size) {
                    std::cerr << "❌ Pattern value " << val << " out of range for row \"" 
                              << row_name << "\" (size " << row_size << ")\n";
                    valid = false;
                }
            }
        }
    }

    return valid;
}



bool check_input_validity(const InputData& input_data) {
    if(!validate_orbit_names(input_data) || !validate_names(input_data))
        return false;
    if(!validate_unique_targets(input_data) || !validate_pairings(input_data))
        return false;

    return true;
}
