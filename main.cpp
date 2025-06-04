#include <iostream>
#include "input_example.h"
#include "input_validity.h"

int main() {
    auto input_data = input_data_2;

    if (!check_input_validity(input_data)) {
        std::cerr<<"Invalid Input"<<std::endl;
        return 1;
    }

    // Proceed with the rest of the program
    return 0;
}
