#include <gtest/gtest.h>
#include "input_data.h"
#include "solving_linear_equations.h"

TEST(PrintMatrix, ZeroDimensionMatrix) {
    InputData data;
    data.addRow("R1", 0);
    data.addRow("R2", 0);
    data.addColumn("C1", 0);
    data.addColumn("C2", 0);

    data.addPairing("R1", "C1", {}, 5);
    data.addPairing("R1", "C2", {}, 3);
    data.addPairing("R2", "C1", {}, 1);
    data.addPairing("R2", "C2", {}, 0);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());  // Redirect cout

    SolveLinearEquations::print_matrix(data);

    std::cout.rdbuf(old);  // Reset cout

    std::string expected =
        "      C1 C2 \n"
        "R1    5   3\n"
        "R2    1   0\n";

    std::string output = buffer.str();

    // Trim whitespace if needed or normalize output
    ASSERT_NE(output.find("C1"), std::string::npos);
    ASSERT_NE(output.find("R1"), std::string::npos);
    ASSERT_NE(output.find("5"), std::string::npos);
    ASSERT_NE(output.find("0"), std::string::npos);
}

TEST(PrintMatrix, NonZeroRowOrbit){
    InputData data;
    data.addRow("R1", 0);
    data.addRow("R2", 7);
    data.addColumn("C1", 0);

    EXPECT_THROW({
        SolveLinearEquations::print_matrix(data);
    }, std::runtime_error);
}

TEST(PrintMatrix, NonZeroRowColumn){
    InputData data;
    data.addRow("A", 0);
    data.addRow("B", 0);
    data.addColumn("X", 0);
    data.addColumn("Y", 3);

    EXPECT_THROW({
        SolveLinearEquations::print_matrix(data);
    }, std::runtime_error);
}