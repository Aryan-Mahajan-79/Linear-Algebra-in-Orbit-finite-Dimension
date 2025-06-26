#pragma once
#include "input_data.h"

class MatrixReduction{
    public:
        static InputData reduce_the_matrix_completely(InputData& input_data);
        static int get_maximum_dimension_of_the_matrix(const std::vector<InputData::RowOrCol>& rows);
    
    private:
        static std::vector<bool> newly_created_rows;
        static void fill_missing_default_values_with_zero(InputData& input_data);
        static void fill_missing_pairing_with_the_default_values(InputData& input_data);
        static std::vector<std::vector<PatternKey::PatternElement>> generate_patterns(int col_size, int row_size);
        static InputData reduce_the_matrix_by_one_dimension(InputData& input_data);
        static void add_new_rows_for_a_sigle_input_row_in_the_reduction_process(InputData& input_data,const InputData::RowOrCol& row, InputData& output_data);



    // Friend Classes for testing
    friend class FillMissingDefaultValuesWithZero_SomeDefaultValuesDefined_Test;
    friend class FillMissingDefaultValuesWithZero_AllDefaultValuesDefined_Test;
    friend class FillMissingDefaultValuesWithZero_NoDefaultValuesDefined_Test;
    friend class FillMissingDefaultValuesWithZero_LargeMatrix_Test;

    friend class FillMissingPairingWithDefaultValues_SomePairingsDefined_Test;
    friend class FillMissingPairingWithDefaultsValues_NoPairingsDefined_Test;
    friend class FillMissingPairingWithDefaultsValues_PartiallyFilledPairings_Test;
    friend class FillMissingPairingWithDefaultsValues_MultipleRowsAndColumns_Test;

    friend class GetMaxDimensionOfTheMatrix_SingleElement_Test;
    friend class GetMaxDimensionOfTheMatrix_AllEqualSizes_Test;
    friend class GetMaxDimensionOfTheMatrix_VaryingSizes_Test;


    friend class ReplaceRowByNMinus1Rows_BasicReplacement_Test;

};