#include "input_validation.h"
#include "logger.h"
#include <set>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <cctype>
#include <variant>
#include <vector>

using PatternElement = PatternKey::PatternElement;


bool InputValidation::check_input_validity(const InputData& input_data) {

    const auto& rows = input_data.getRows();
    const auto& columns = input_data.getColumns();
    const auto& target = input_data.getTarget();
    try {
        if (!validate_if_orbit_names_are_unique(rows, columns)) {
            Logger::instance().log("❌ Orbit names are not unique.\n");
            return false;
        }
    } catch (const std::exception& e) {
        Logger::instance().log("❌ Exception during orbit name uniqueness validation: " + std::string( e.what()) + "\n");
        return false;
    }

    try {
        if (!validate_if_number_of_orbits_are_non_negative(rows, columns)) {
            Logger::instance().log("❌ Number of Orbits is Negative.\n");
            return false;
        }
    } catch (const std::exception& e) {
        Logger::instance().log("❌ Exception during orbit number validation: " + std::string(e.what()) +  "\n");
        return false;
    }

    try {
        if (!validate_if_names_in_descriptions_of_target_and_matrix_are_defined(input_data)) {
            Logger::instance().log("❌ Some names in targets or matrix descriptions are undefined.\n");
            return false;
        }
    } catch (const std::exception& e) {
        Logger::instance().log("❌ Exception during name definition validation: " + std::string(e.what()) +  "\n");
        return false;
    }

    try {
        if (!validate_unique_targets(rows, target)) {
            Logger::instance().log("❌ Target definitions are not unique.\n");
            return false;
        }
    } catch (const std::exception& e) {
        Logger::instance().log("❌ Exception during target uniqueness validation: " + std::string(e.what()) +  "\n");
        return false;
    }

    try {
        if (!validate_if_all_matrix_entry_are_correct(input_data)) {
            Logger::instance().log("❌ Matrix entries are incorrect or malformed.\n");
            return false;
        }
    } catch (const std::exception& e) {
        Logger::instance().log("❌ Exception during matrix entry validation: " + std::string(e.what()) +  "\n");
        return false;
    }

    return true;
}


static bool is_only_spaces(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); });
}

bool InputValidation::validate_if_orbit_names_are_unique(const std::vector<InputData::RowOrCol> rows, const std::vector<InputData::RowOrCol> columns) {

    std::set<std::string> row_names;
    std::set<std::string> column_names;
    bool duplicate_found = false;
    bool empty_name = false;
    bool name_overlap = false;

    if (rows.empty() || columns.empty()) return false;

    for (const auto& [name, _] : rows) {
        if (name.empty() || is_only_spaces(name)) empty_name = true;
        if (!row_names.insert(name).second) duplicate_found = true;
    }

    for (const auto& [name, _] : columns) {
        if (name.empty() || is_only_spaces(name)) empty_name = true;
        if (!column_names.insert(name).second) duplicate_found = true;
    }

    // Check for name overlap between rows and columns
    for (const auto& name : row_names) {
        if (column_names.count(name)) {
            name_overlap = true;
            break;
        }
    }

    if (duplicate_found)
        Logger::instance().log("❌ Duplicate names found in rows/columns!\n" );
    if (empty_name)
        Logger::instance().log("❌ Empty name found in rows/columns!\n" );
    if (name_overlap)
        Logger::instance().log("❌ Row and column share the same name!\n" );

    return !(duplicate_found || empty_name || name_overlap);
}

bool InputValidation::validate_if_number_of_orbits_are_non_negative(const std::vector<InputData::RowOrCol> rows, const std::vector<InputData::RowOrCol> columns){
    bool negative_found = false;

    for (const auto& [name, count] : rows) {
        if (count < 0) {
            Logger::instance().log("❌ Row \"" + std::string (name) + "\" has a negative number of orbits: " + std::to_string (count) + "\n" );
            negative_found = true;
        }
    }

    for (const auto& [name, count] : columns) {
        if (count < 0) {
            Logger::instance().log("❌ Column \"" + std::string (name) + "\" has a negative number of orbits: " + std::to_string (count) + "\n" );
            negative_found = true;
        }
    }

    return !negative_found;
}

bool InputValidation::validate_if_names_in_descriptions_of_target_and_matrix_are_defined(const InputData& input_data) {
    std::unordered_set<std::string> valid_rows, valid_columns;

    for (const auto& [row_name, _] : input_data.getRows())
        valid_rows.insert(row_name);

    for (const auto& [col_name, _] : input_data.getColumns())
        valid_columns.insert(col_name);

    bool all_valid = true;

    for (const auto& [row_name, _] : input_data.getTarget()) {
        if (!valid_rows.count(row_name)) {
            Logger::instance().log("❌ Invalid row name in target: " + std::string(row_name) + "\n" );
            all_valid = false;
        }
    }

    for (const auto& [key, _] : input_data.getDefaultValues()) {
        const auto& [row_name, col_name] = key;
        if (!valid_rows.count(row_name)) {
            Logger::instance().log("❌ Invalid row name in default_values: " + std::string(row_name) + "\n" );
            all_valid = false;
        }
        if (!valid_columns.count(col_name)) {
            Logger::instance().log("❌ Invalid column name in default_values: " + std::string(col_name) + "\n" );
            all_valid = false;
        }
    }

    for (const auto& [key, _] : input_data.getPairings()) {
        const auto& row_name = key.getRow();
        const auto& col_name = key.getColumn();
        if (!valid_rows.count(row_name)) {
            Logger::instance().log("❌ Invalid row name in pairings: " + std::string(row_name) + "\n" );
            all_valid = false;
        }
        if (!valid_columns.count(col_name)) {
            Logger::instance().log("❌ Invalid column name in pairings: " + std::string(col_name) + "\n" );
            all_valid = false;
        }
    }

    return all_valid;
}

bool InputValidation::validate_if_for_every_row_target_is_uniquely_defined(const std::vector<InputData::RowOrCol> rows, const std::map<std::string, int> target){
    std::unordered_set<std::string> seen_names;
    bool duplicate_found = false;

    for (const auto& [row_name, _] : target) {
        if (!seen_names.insert(row_name).second) {
            Logger::instance().log("❌ Duplicate target found for row: " + std::string(row_name) + "\n" );
            duplicate_found = true;
        }
    }
    return !duplicate_found;
}

bool InputValidation::validate_unique_targets(const std::vector<InputData::RowOrCol> rows, const std::map<std::string, int> target) {

    if(rows.size() != target.size()) {
        Logger::instance().log("❌ Target count does not match row count!\n" );
        return false;
    }
    if(!validate_if_for_every_row_target_is_uniquely_defined(rows, target)){
        return false;
    }

    return true;
}

int get_col_size(const InputData& data, const std::string& col_name) {
    for (const auto& [name, size] : data.getColumns()) {
        if (name == col_name) return size;
    }
    throw std::runtime_error("Column not found: " + col_name);
}

int get_row_size(const InputData& data, const std::string& row_name) {
    for (const auto& [name, size] : data.getRows()) {
        if (name == row_name) return size;
    }
    throw std::runtime_error("Row not found: " + row_name);
}

bool InputValidation::validate_if_single_matrix_entry_is_correct_column_size(int col_size, std::vector<PatternElement>pattern, const std::string& col_name){
    
    if (static_cast<int>(pattern.size()) != col_size) {
        Logger::instance().log("❌ Pattern size mismatch for column \"" + std::string(col_name) + 
                "\". Expected: " + std::to_string(col_size) + 
                ", got: " + std::to_string(pattern.size()) + "\n");
        return false;
    }
    return true;

}

bool InputValidation::validate_if_single_matrix_entry_is_correct_row_range(int row_size, std::vector<PatternElement>pattern, const std::string& row_name){

    for (const auto& p : pattern) {
        if (std::holds_alternative<int>(p)) {
            int val = std::get<int>(p);
            if (val < 1 || val > row_size) {
                Logger::instance().log("❌ Pattern value " + std::to_string(val) + " out of range for row \""
                            + std::string(row_name) + "\" (size " + std::to_string(row_size) + ")\n");
                return false;
            }
        }
    }
    return true;
}

bool InputValidation::validate_if_all_matrix_entry_are_correct(const InputData& data) {

    for (const auto& [key, value] : data.getPairings()) {
        const std::string& row_name = key.getRow();
        const std::string& col_name = key.getColumn();
        const auto& pattern = key.getPattern();

        int col_size, row_size;
        try {
            col_size = get_col_size(data, col_name);
            row_size = get_row_size(data, row_name);
        } catch (const std::exception& e) {
            Logger::instance().log("❌ " + std::string(e.what()) +  '\n');
            return false;
        }
        if(!validate_if_single_matrix_entry_is_correct_column_size(col_size, pattern, col_name)){
            return false;
        }
        if(!validate_if_single_matrix_entry_is_correct_row_range(row_size, pattern, row_name)){
            return false;
        }
    }

    return true;
}

