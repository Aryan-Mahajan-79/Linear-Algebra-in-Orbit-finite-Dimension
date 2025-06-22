#include <iostream>
#include "input_data.h"
#include "input_validation.h"
#include "matrix_reduction.h"

InputData input_data = [] {
    InputData data;
    data.addRow("A", 2);
    data.addRow("B", 1);
    data.addColumn("X", 1);
    data.addColumn("Y", 2);

    data.setTarget("A", 4);
    data.setTarget("B", 7);

    data.setDefaultValue("A", "X", 0);
    data.setDefaultValue("A", "Y", 0);
    data.setDefaultValue("B", "X", 0);
    data.setDefaultValue("B", "Y", 0);

    data.addPairing("A", "X", {1}, 6);
    data.addPairing("A", "X", {2}, 3);
    data.addPairing("A", "X", {'*'}, 1);

    data.addPairing("A", "Y", {1, 2}, 4);
    data.addPairing("A", "Y", {'*', 2}, 2);
    data.addPairing("A", "Y", {'*', '*'}, 0);

    data.addPairing("B", "X", {1}, 7);
    data.addPairing("B", "X", {'*'}, 3);

    data.addPairing("B", "Y", {1, '*'}, 5);
    data.addPairing("B", "Y", {'*', 1}, 2);
    data.addPairing("B", "Y", {'*', '*'}, 0);

    return data;
}();

int main() {
    
    if (!InputValidation::check_input_validity(input_data)) {
        std::cerr<<"Invalid Input"<<std::endl;
        return 1;
    }
    MatrixReduction::fill_missing_default_values_with_zero(input_data);
    MatrixReduction::fill_missing_pairing_with_the_default_values(input_data);
    input_data.print();
    if (!InputValidation::check_input_validity(input_data)) {
        std::cerr<<"Invalid after filling"<<std::endl;
        return 1;
    }
    // input_data.print();
    // InputData reduced_input_data = MatrixReduction::reduce_the_matrix_completely(input_data);
    return 0;
}
