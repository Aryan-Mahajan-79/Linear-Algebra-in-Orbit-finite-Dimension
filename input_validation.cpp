#include "input_validation.h"
#include <set>
#include <iostream>

bool validate_orbit_names(const InputData& input_data) {
    std::set<std::string> seen;
    bool duplicate_found = false;

    // Check rows
    for (const auto& [name, _] : input_data.rows) {
        if (!seen.insert(name).second) {
            duplicate_found = true;
        }
    }

    // Check cols
    for (const auto& [name, _] : input_data.cols) {
        if (!seen.insert(name).second) {
            duplicate_found = true;
        }
    }

    if (duplicate_found) {
        std::cerr << "❌ Duplicate names found in rows/cols!" << std::endl;
    }

    return !duplicate_found;
}

bool check_input_validity(const InputData& input_data) {
    return validate_orbit_names(input_data);
}
