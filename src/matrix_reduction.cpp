#include "matrix_reduction.h"

using PatternElement = PatternKey::PatternElement;

void MatrixReduction::fill_missing_default_values_with_zero(InputData& data) {
    const auto& rows = data.getRows();
    const auto& cols = data.getColumns();
    const auto& defaults = data.getDefaultValues();

    for (const auto& [rowName, _] : rows) {
        for (const auto& [colName, _] : cols) {
            std::pair<std::string, std::string> key = {rowName, colName};
            if (defaults.find(key) == defaults.end()) {
                data.setDefaultValue(rowName, colName, 0);
            }
        }
    }
}

void MatrixReduction::generate_patterns(int col_size, int row_size, std::vector<PatternElement>& current, std::vector<std::vector<PatternElement>>& all) {
    if (static_cast<int>(current.size()) == col_size) {
        all.push_back(current);
        return;
    }

    for (int i = 1; i <= row_size; ++i) {
        current.push_back(i);
        generate_patterns(col_size, row_size, current, all);
        current.pop_back();
    }

    current.push_back('*');
    generate_patterns(col_size, row_size, current, all);
    current.pop_back();
}

namespace {

void expandAndInsertDefaults(const std::vector<PatternElement>& pattern,
                             const std::string& row_name,
                             const std::string& col_name,
                             int row_size,
                             std::unordered_set<PatternKey, PatternKey::Hasher>& existing_keys,
                             InputData& data) {
    std::function<void(size_t, std::vector<PatternElement>&)> expand;
    expand = [&](size_t index, std::vector<PatternElement>& partial) {
        if (index == pattern.size()) {
            PatternKey key(row_name, col_name, partial);
            if (existing_keys.find(key) == existing_keys.end()) {
                int default_val = data.getDefaultValue(row_name, col_name);
                data.addPairing(row_name, col_name, partial, default_val);
                existing_keys.insert(key);
            }
            return;
        }

        const auto& elem = pattern[index];
        if (std::holds_alternative<char>(elem) && std::get<char>(elem) == '*') {
            for (int i = 1; i < row_size; ++i) {
                partial.push_back(i);
                expand(index + 1, partial);
                partial.pop_back();
            }
        } else {
            partial.push_back(elem);
            expand(index + 1, partial);
            partial.pop_back();
        }
    };

    std::vector<PatternElement> partial;
    expand(0, partial);
}

bool containsWildcard(const std::vector<PatternElement>& pattern) {
    for (const auto& elem : pattern) {
        if (std::holds_alternative<char>(elem) && std::get<char>(elem) == '*') {
            return true;
        }
    }
    return false;
}

} 


void MatrixReduction::fill_missing_pairing_with_the_default_values(InputData& data) {
    const auto& rows = data.getRows();
    const auto& cols = data.getColumns();
    const auto& pairings = data.getPairings();

    std::unordered_set<PatternKey, PatternKey::Hasher> existing_keys;
    for (const auto& [key, _] : pairings) {
        existing_keys.insert(key);
    }

    for (const auto& [row_name, row_size] : rows) {
        for (const auto& [col_name, col_size] : cols) {

            std::vector<std::vector<PatternElement>> all_patterns;
            std::vector<PatternElement> current;
            generate_patterns(col_size, row_size, current, all_patterns);

            for (const auto& pattern : all_patterns) {
                if (containsWildcard(pattern)) {
                    expandAndInsertDefaults(pattern, row_name, col_name, row_size, existing_keys, data);
                } else {
                    PatternKey key(row_name, col_name, pattern);
                    if (existing_keys.find(key) == existing_keys.end()) {
                        int default_val = data.getDefaultValue(row_name, col_name);
                        data.addPairing(row_name, col_name, pattern, default_val);
                        existing_keys.insert(key);
                    }
                }
            }
        }
    }
}

int get_maximum_dimension_of_the_matrix(const std::vector<InputData::RowOrCol>& rows) {
    int max_size = 0;
    for (const auto& [name, size] : rows) {
        if (size > max_size) {
            max_size = size;
        }
    }
    return max_size;
}

void replace_a_row_of_n_dimension_by_n_rows_of_dimension_n_minus_1(InputData& input_data,const InputData::RowOrCol& row){
    const std::string& row_name = row.first;
    int row_size = row.second;

    if (row_size <= 1) {
        std::cerr << "⚠️ Cannot decompose row '" << row_name << "' of size " << row_size << ". Skipping.\n";
        return;
    }

    auto& rows = const_cast<std::vector<InputData::RowOrCol>&>(input_data.getRows());
    rows.erase(std::remove_if(rows.begin(), rows.end(), [&](const InputData::RowOrCol& r) { return r.first == row_name; }), rows.end());

    for (int i = 0; i < row_size - 1; ++i) {
        std::string new_row_name = row_name + "_" + std::to_string(i + 1);
        input_data.addRow(new_row_name, row_size - 1);
    }

    std::cout << "✅ Replaced row '" << row_name << "' with " << (row_size - 1)
              << " rows of size " << (row_size - 1) << ".\n";
}

InputData& reduce_the_matrix_by_one_dimension(InputData& input_data){
    InputData new_data;

    int max_dimension = get_maximum_dimension_of_the_matrix(input_data.getRows());

    // Rows
    for (const auto& row : input_data.getRows()) {
        const std::string& name = row.first;
        int size = row.second;

        if (size == max_dimension && size > 1) {
            for (int i = 0; i < size; ++i) {
                std::string new_row_name = name + "_" + std::to_string(i + 1);
                new_data.addRow(new_row_name, size - 1);
            }
        } else {
            new_data.addRow(name, size);
        }
    }

    // Columns
    for (const auto& col : input_data.getColumns()) {
        new_data.addColumn(col.first, col.second);
    }

    // Target


    // Pairings

    return new_data;
}