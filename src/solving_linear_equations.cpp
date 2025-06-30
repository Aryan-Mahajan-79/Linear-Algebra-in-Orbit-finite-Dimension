#include "solving_linear_equations.h"
#include <iostream>

void SolveLinearEquations::print_matrix(const InputData& reduced_matrix) {
    const auto& rows = reduced_matrix.getRows();
    const auto& cols = reduced_matrix.getColumns();

    for (const auto& row : rows) {
        if (row.second != 0) {
            throw std::runtime_error("Non-zero row target: " + row.first + " -> " + std::to_string(row.second));
        }
    }
    for (const auto& col : cols) {
        if (col.second != 0) {
            throw std::runtime_error("Non-zero column target: " + col.first + " -> " + std::to_string(col.second));
        }
    }

    std::cout << "      ";
    for (const auto& col : cols) {
        std::cout << col.first << " ";
    }
    std::cout << "\n";

    for (const auto& row : rows) {
        std::cout << row.first << " ";
        for (const auto& col : cols) {
            int val = reduced_matrix.getPairingValue(row.first, col.first, {});
            std::cout << "   " << val;
        }
        std::cout << "\n";
    }
}
