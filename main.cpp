#include <iostream>
#include "input.h"
#include "input_validation.h"

int main() {
    auto input_data = input_data_3;

    if (!check_input_validity(input_data)) {
        std::cerr<<"Invalid Input"<<std::endl;
        return 1;
    }

    // Proceed with the rest of the program
    return 0;
}
