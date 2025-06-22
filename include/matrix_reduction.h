#pragma once
#include "input_data.h"

class MatrixReduction{
    public:
        static void fill_missing_default_values_with_zero(InputData& input_data);
        static void fill_missing_pairing_with_the_default_values(InputData& input_data);
        static InputData& reduce_the_matrix_completely(InputData& input_data);
        
    private:
        static InputData& reduce_the_matrix_by_one_dimension(InputData& input_data);
        static void generate_patterns(int col_size, int row_size, std::vector<PatternKey::PatternElement>& current, std::vector<std::vector<PatternKey::PatternElement>>& all);
        static int get_maximum_dimension_of_the_matrix(const std::vector<InputData::RowOrCol> rows);
        static void replace_a_row_of_n_dimension_by_n_rows_of_dimension_n_minus_1(InputData& input_data,const InputData::RowOrCol& row)
};