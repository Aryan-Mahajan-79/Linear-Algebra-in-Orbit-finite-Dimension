#include "matrix_reduction.h"
#include<iostream>

using PatternElement = PatternKey::PatternKey::PatternElement;

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

void MatrixReduction::generate_patterns(int col_size, int row_size, std::vector<std::vector<PatternKey::PatternElement>>& all_patterns) {
    // 2^col_size
    int total_masks = 1 << col_size;

    for (int mask = 0; mask < total_masks; ++mask) {
        bool all_wildcard = (mask == total_masks - 1);

        std::function<void(int, std::vector<PatternKey::PatternElement>&)> generate;
        generate = [&](int pos, std::vector<PatternKey::PatternElement>& current) {
            if (pos == col_size) {
                all_patterns.push_back(current);
                return;
            }

            if ((mask >> pos) & 1) {
                // Add Wildcard and call generate
                current.push_back('*');
                generate(pos + 1, current);
                current.pop_back();
            } 
            else {
                // Iterate over numeric values and call generate
                for (int i = 1; i <= row_size; ++i) {
                    current.push_back(i);
                    generate(pos + 1, current);
                    current.pop_back();
                }
            }
        };

        std::vector<PatternKey::PatternElement> current;
        generate(0, current);

        if (all_wildcard) break; // Handle (*,*,...,*) only once at end
    }

    // Add (*,*,...,*)
    std::vector<PatternKey::PatternElement> all_stars(col_size, '*');
    all_patterns.push_back(all_stars);
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

            std::vector<std::vector<PatternKey::PatternElement>> all_patterns;
            MatrixReduction::generate_patterns(col_size, row_size, all_patterns);

            for (const auto& pattern : all_patterns) {
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


int MatrixReduction::get_maximum_dimension_of_the_matrix(const std::vector<InputData::RowOrCol>& rows) {
    int max_size = 0;
    for (const auto& [name, size] : rows) {
        if (size > max_size) {
            max_size = size;
        }
    }
    return max_size;
}

void MatrixReduction::replace_a_row_of_n_dimension_by_n_rows_of_dimension_n_minus_1(InputData& input_data,const InputData::RowOrCol& row){
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

InputData MatrixReduction::reduce_the_matrix_by_one_dimension(InputData& input_data){
    InputData new_data;

    int max_dimension = MatrixReduction::get_maximum_dimension_of_the_matrix(input_data.getRows());

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

InputData MatrixReduction::reduce_the_matrix_completely(InputData& input_data){
    InputData data;
    while(MatrixReduction::get_maximum_dimension_of_the_matrix(input_data.getRows()) > 0){
        data = MatrixReduction::reduce_the_matrix_by_one_dimension(input_data);
    }
    return data;
}