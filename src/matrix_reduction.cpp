#include "matrix_reduction.h"
#include<iostream>

using PatternElement = PatternKey::PatternKey::PatternElement;
std::vector<bool> MatrixReduction::newly_created_rows;


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

std::vector<std::vector<PatternKey::PatternElement>> MatrixReduction::generate_patterns(int col_size, int row_size) {
    std::vector<std::vector<PatternKey::PatternElement>> all_patterns;
    // number of subsets of columns
    int total_masks = 1 << col_size;

    //mask describes a pattern of wildcards in the column data
    for (int mask = 0; mask < total_masks; ++mask) {

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

        // probably to remove
        //if (all_wildcard) break; // Handle (*,*,...,*) only once at end
    }

    // Add (*,*,...,*)
    std::vector<PatternKey::PatternElement> all_stars(col_size, '*');
    all_patterns.push_back(all_stars);
    return all_patterns;
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
            all_patterns = MatrixReduction::generate_patterns(col_size, row_size);

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

void MatrixReduction::add_new_rows_for_a_sigle_input_row_in_the_reduction_process(InputData& input_data,const InputData::RowOrCol& row, InputData& output_data){
    const std::string& row_name = row.first;
    int row_size = row.second;

    int max_dimension = MatrixReduction::get_maximum_dimension_of_the_matrix(input_data.getRows());

    if (row_size < 1) {
        std::cerr << "⚠️ Cannot decompose row '" << row_name << "' of size " << row_size << ". Skipping.\n";
        return;
    }
    if(row.second < max_dimension){
        output_data.addRow(row.first, row.second);
        newly_created_rows.push_back(false);
    }
    else{
        for (int i = 0; i < row_size - 1; ++i) {
            std::string new_row_name = row_name + "_" + std::to_string(i + 1);
            output_data.addRow(new_row_name, row_size - 1);
            newly_created_rows.push_back(true);
        }
    }
}

int MatrixReduction::get_arity_of_column(const InputData& input_data, const std::string col_name){
    for (const auto& col : input_data.getColumns()) {
        if (col.first == col_name) {
            return col.second;
        }
    }
    throw std::invalid_argument("Column name not found: " + col_name);
}

std::vector<PatternElement> create_a_new_pattern_element(int index, std::vector<PatternKey::PatternElement> new_pattern, int i) {
    std::vector<PatternElement> pattern;

    for (int j =0; j< index; j++){
        pattern.push_back(new_pattern[j]);

    }
    pattern.push_back(i);
    for (std::size_t j = index + 1; j < new_pattern.size(); j++){
        pattern.push_back(new_pattern[j]);
    }
    return pattern;
}

int MatrixReduction::calculate_f_function(InputData& input_data, std::string old_row_name, int index , std::string old_col_name, std::vector<PatternKey::PatternElement> new_pattern){
    std::string new_row_name = old_row_name + "_" + std::to_string(index);
    std::list<std::vector<PatternElement> >  index_extensions_of_the_new_pattern;
    // we need to create a list of all possibe index_extensions of the new_pattern to know which old orbits we have to sum when we calculate the function f.
    // first we determine indices of column data that bound where then index can be matched
    int lowerbound_for_index = (index == 0) ? -1 : PatternKey::convert_to_int(new_pattern[index -1]);

    int maximal_index_of_data_supporting_column = MatrixReduction::get_arity_of_column(input_data, old_col_name);

    int upperbound_for_index = (index == (int)new_pattern.size()) ? maximal_index_of_data_supporting_column +1  : PatternKey::convert_to_int(new_pattern[index + 1]);

    // now we generate all possible index_extensions of the new_pattern
    for (int i = lowerbound_for_index + 1; i < upperbound_for_index; i++){
        if(i==0)continue;
        std::vector<PatternElement> new_element = create_a_new_pattern_element(index, new_pattern, i);
        index_extensions_of_the_new_pattern.push_back(new_element);
    }

    int sum = 0;
    for (auto extension : index_extensions_of_the_new_pattern){
        int value = input_data.getPairingValue(old_row_name, old_col_name, extension);
        sum += value;
    }
    return sum;
}

InputData MatrixReduction::reduce_the_matrix_by_one_dimension(InputData& input_data){
    InputData new_data;

    const auto& rows = input_data.getRows();
    const auto& cols = input_data.getColumns();

    // New columns
    for (const auto& c : cols){
        new_data.addColumn(c.first, c.second);
    }

    // New rows

    //But first we have to clear vector newly_created_rows;
    newly_created_rows.clear();

    // first I copy the rows in the input_data, next I process them using add_new_rows_for_a_sigle_input_row_in_the_reduction_process
    // function, that changes them to the proper new set of rows 
    for (const auto& row : rows){
        MatrixReduction::add_new_rows_for_a_sigle_input_row_in_the_reduction_process(input_data, row, new_data);
    }

    // New Default
        // We dont need to add them as we don't use dafault data on this stage of the reduction, everything is explicit

    // New Target
    for (std::size_t i=0; i<rows.size(); i++) {
        auto row= rows[i];
        const std::string& row_name = row.first;

        if (!newly_created_rows[i]) {
            new_data.setTarget(row_name, input_data.getTarget().at(row_name));
        } else {
            new_data.setTarget(row_name, 0);
        }
    }
    // New Matrix

    for (auto column : new_data.getColumns()){
        const std::string& col_name = column.first;
        const int col_size = column.second;
        for(std::size_t i=0; i<new_data.getRows().size(); i++){
            const auto& row = new_data.getRows()[i];
            const std::string& row_name = row.first;
            const int row_size = row.second;
            std::vector<std::vector<PatternKey::PatternElement>> all_patterns = MatrixReduction::generate_patterns(col_size, row_size);
            if (!(newly_created_rows[i])){
                
                for (const auto& pattern : all_patterns) {
                    // Copy value from the original input_data
                    int value = input_data.getPairingValue(row_name, col_name, pattern);
                    new_data.addPairing(row_name, col_name, pattern, value);
                }      
            }
            else{
                // here we calculate the f function look to the thesis page 75
                std::size_t pos = row_name.rfind('_');
                std::string old_row_name = row_name.substr(0, pos);
                int index = std::stoi(row_name.substr(pos + 1));

                for (const auto& pattern : all_patterns) {
                    // Calculate value using the function f
                    int value = MatrixReduction::calculate_f_function(input_data, old_row_name, index, col_name, pattern);
                    new_data.addPairing(row_name, col_name, pattern, value);
                }
            }
        }
    }
    return new_data;
}

InputData MatrixReduction::reduce_the_matrix_completely(InputData& input_data){
    InputData data;
    while(MatrixReduction::get_maximum_dimension_of_the_matrix(input_data.getRows()) > 0){
        MatrixReduction::fill_missing_default_values_with_zero(input_data);
        MatrixReduction::fill_missing_pairing_with_the_default_values(input_data);
        std::cout<<"reducing by one"<<std::endl;
        data = MatrixReduction::reduce_the_matrix_by_one_dimension(input_data);
    }
    return data;
}