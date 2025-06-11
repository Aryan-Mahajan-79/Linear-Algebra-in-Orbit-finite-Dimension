#include <gtest/gtest.h>
#include "input_data.h"
#include "input_validation.h"
#include "data_examples.h"


bool check_input_validity(const InputData& input_data);

// All valid inputs from input.cpp are true
TEST(ValidateOrbitNamesTest, AllInputsValid) {
    for (size_t i = 0; i < input_data_set.size(); ++i) {
        EXPECT_TRUE(InputValidation::check_input_validity(input_data_set[i])) << "InputData " << (i + 1) << " failed";
    }
}

// Orbit of Row is zero
TEST(BasicSyntax, ZeroOrbitRow){
    InputData data;
    data.addRow("A", 0);
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
    
    
    EXPECT_FALSE(InputValidation::check_input_validity(data));
}

// Orbit of Column is zero
TEST(BasicSyntax, ZeroOrbitColumn){
    InputData data;
    data.addRow("A", 1);
    data.addRow("B", 0);
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
    
    
    EXPECT_FALSE(InputValidation::check_input_validity(data));
}

// Row name or Columnumn name is blank
TEST(ValidateOrbitNamesTest, BlankName){
    InputData data;
    data.addRow("A", 2);
    data.addRow("", 1);
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
    
    
    EXPECT_FALSE(InputValidation::validate_orbit_names(data));
}

// Input does not have any Row
TEST(ValidateOrbitNamesTest, NoRow){
    InputData data;
    data.addColumn("X", 2);
    data.setTarget("A", 4);
    data.setDefaultValue("A", "X", 4);

    data.addPairing("A", "X", {1, 2}, 5);
    data.addPairing("A", "X", {1, '*'}, 2);
    data.addPairing("A", "X", {2, 1}, 3);
    data.addPairing("A", "X", {'*', 2}, 1);
    data.addPairing("A", "X", {'*', '*'}, 8);
    
    EXPECT_FALSE(InputValidation::validate_orbit_names(data));
}

// Input does not have any Column
TEST(ValidateOrbitNamesTest, NoColumn){
    InputData data;
    data.addRow("A", 2);
    data.setTarget("A", 4);
    data.setDefaultValue("A", "X", 4);

    data.addPairing("A", "X", {1, 2}, 5);
    data.addPairing("A", "X", {1, '*'}, 2);
    data.addPairing("A", "X", {2, 1}, 3);
    data.addPairing("A", "X", {'*', 2}, 1);
    data.addPairing("A", "X", {'*', '*'}, 8);   
    
    EXPECT_FALSE(InputValidation::validate_orbit_names(data));
}

// Duplicate Row name
TEST(ValidateOrbitNamesTest, DetectsDuplicateRows) {
    InputData data;
    data.addRow("A", 2);
    data.addRow("A", 1);
    data.addColumn("X", 1);
    data.addColumn("Y", 2);

    data.setTarget("A", 4);
    data.setTarget("A", 7);

    data.setDefaultValue("A", "X", 0);
    data.setDefaultValue("A", "Y", 0);
    data.setDefaultValue("A", "X", 0);
    data.setDefaultValue("A", "Y", 0);

    data.addPairing("A", "X", {1}, 6);
    data.addPairing("A", "X", {2}, 3);
    data.addPairing("A", "X", {'*'}, 1);

    data.addPairing("A", "Y", {1, 2}, 4);
    data.addPairing("A", "Y", {'*', 2}, 2);
    data.addPairing("A", "Y", {'*', '*'}, 0);

    data.addPairing("A", "X", {1}, 7);
    data.addPairing("A", "X", {'*'}, 3);

    data.addPairing("A", "Y", {1, '*'}, 5);
    data.addPairing("A", "Y", {'*', 1}, 2);
    data.addPairing("A", "Y", {'*', '*'}, 0);
    
    
    EXPECT_FALSE(InputValidation::validate_orbit_names(data));
}

// Duplicate Column name
TEST(ValidateOrbitNamesTest, DetectsDuplicateColumn) {
    InputData data;
    data.addRow("A", 2);
    data.addRow("B", 1);
    data.addColumn("X", 1);
    data.addColumn("X", 2);

    data.setTarget("A", 4);
    data.setTarget("B", 7);

    data.setDefaultValue("A", "X", 0);
    data.setDefaultValue("A", "X", 0);
    data.setDefaultValue("B", "X", 0);
    data.setDefaultValue("B", "X", 0);

    data.addPairing("A", "X", {1}, 6);
    data.addPairing("A", "X", {2}, 3);
    data.addPairing("A", "X", {'*'}, 1);

    data.addPairing("A", "X", {1, 2}, 4);
    data.addPairing("A", "X", {'*', 2}, 2);
    data.addPairing("A", "X", {'*', '*'}, 0);

    data.addPairing("B", "X", {1}, 7);
    data.addPairing("B", "X", {'*'}, 3);

    data.addPairing("B", "X", {1, '*'}, 5);
    data.addPairing("B", "X", {'*', 1}, 2);
    data.addPairing("B", "X", {'*', '*'}, 0);

    EXPECT_FALSE(InputValidation::validate_orbit_names(data));
}

// For some matrices it makes sense to have same row and column name eg. symmetric matrices
TEST(ValidateOrbitNamesTest, DetectsOverlapBetweenRowsAndColumns) {
    InputData data;
    data.addRow("A", 2);
    data.addRow("X", 1);
    data.addColumn("X", 1);
    data.addColumn("Y", 2);

    data.setTarget("A", 4);
    data.setTarget("X", 7);

    data.setDefaultValue("A", "X", 0);
    data.setDefaultValue("A", "Y", 0);
    data.setDefaultValue("X", "X", 0);
    data.setDefaultValue("X", "Y", 0);

    data.addPairing("A", "X", {1}, 6);
    data.addPairing("A", "X", {2}, 3);
    data.addPairing("A", "X", {'*'}, 1);

    data.addPairing("A", "Y", {1, 2}, 4);
    data.addPairing("A", "Y", {'*', 2}, 2);
    data.addPairing("A", "Y", {'*', '*'}, 0);

    data.addPairing("X", "X", {1}, 7);
    data.addPairing("X", "X", {'*'}, 3);

    data.addPairing("X", "Y", {1, '*'}, 5);
    data.addPairing("X", "Y", {'*', 1}, 2);
    data.addPairing("X", "Y", {'*', '*'}, 0);
    
    EXPECT_TRUE(InputValidation::validate_orbit_names(data));
}

// The name of a row or a column in the target does not match with any row or column name
// Hence is invalid.
TEST(ValidateAllNames, NotMatchingNamesinTarget){
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);
    data.addColumn("0", 1);
    data.addColumn("1", 2);

    data.setTarget("8", 10);
    data.setTarget("1", 8);

    data.setDefaultValue("0", "0", 1);
    data.setDefaultValue("0", "1", 2);
    data.setDefaultValue("1", "0", 3);
    data.setDefaultValue("1", "1", 4);

    data.addPairing("0", "0", {1}, 9);
    data.addPairing("0", "0", {2}, 5);
    data.addPairing("0", "0", {'*'}, 2);

    data.addPairing("0", "1", {1, 2}, 7);
    data.addPairing("0", "1", {'*', 2}, 3);
    data.addPairing("0", "1", {'*', '*'}, 0);

    data.addPairing("1", "0", {1}, 6);
    data.addPairing("1", "0", {'*'}, 1);

    data.addPairing("1", "1", {1, '*'}, 8);
    data.addPairing("1", "1", {'*', 1}, 4);
    data.addPairing("1", "1", {'*', '*'}, 0);
  

    EXPECT_FALSE(InputValidation::validate_names(data));

}

// The name of a row or a column in the default value does not match with any row or column name
// Hence is invalid.
TEST(ValidateAllNames, NotMatchingNamesinDefaultValue){
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);
    data.addColumn("0", 1);
    data.addColumn("1", 2);

    data.setTarget("0", 10);
    data.setTarget("1", 8);

    data.setDefaultValue("0", "0", 1);
    data.setDefaultValue("9", "1", 2);
    data.setDefaultValue("1", "0", 3);
    data.setDefaultValue("1", "1", 4);

    data.addPairing("0", "0", {1}, 9);
    data.addPairing("0", "0", {2}, 5);
    data.addPairing("0", "0", {'*'}, 2);

    data.addPairing("0", "1", {1, 2}, 7);
    data.addPairing("0", "1", {'*', 2}, 3);
    data.addPairing("0", "1", {'*', '*'}, 0);

    data.addPairing("1", "0", {1}, 6);
    data.addPairing("1", "0", {'*'}, 1);

    data.addPairing("1", "1", {1, '*'}, 8);
    data.addPairing("1", "1", {'*', 1}, 4);
    data.addPairing("1", "1", {'*', '*'}, 0);
  

    EXPECT_FALSE(InputValidation::validate_names(data));

}

// The name of a row or a column in the pairings does not match with any row or column name
// Hence is invalid.
TEST(ValidateAllNames, NotMatchingNamesinPairing){
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);
    data.addColumn("0", 1);
    data.addColumn("1", 2);

    data.setTarget("0", 10);
    data.setTarget("1", 8);

    data.setDefaultValue("0", "0", 1);
    data.setDefaultValue("0", "1", 2);
    data.setDefaultValue("1", "0", 3);
    data.setDefaultValue("1", "1", 4);

    data.addPairing("5", "0", {1}, 9);
    data.addPairing("5", "0", {2}, 5);
    data.addPairing("5", "0", {'*'}, 2);

    data.addPairing("5", "1", {1, 2}, 7);
    data.addPairing("5", "1", {'*', 2}, 3);
    data.addPairing("5", "1", {'*', '*'}, 0);

    data.addPairing("1", "0", {1}, 6);
    data.addPairing("1", "0", {'*'}, 1);

    data.addPairing("1", "1", {1, '*'}, 8);
    data.addPairing("1", "1", {'*', 1}, 4);
    data.addPairing("1", "1", {'*', '*'}, 0);
  

    EXPECT_FALSE(InputValidation::validate_names(data));

}

// In Pairings the row name in place of column name or vice versa
TEST(ValidateAllNames, RowColumnNameMismatch){
    InputData data;
    data.addRow("R1", 2);
    data.addRow("R2", 3);
    data.addRow("R3", 4);
    data.addColumn("C1", 3);

    data.setTarget("R1", 6);
    data.setTarget("R2", 4);
    data.setTarget("R3", 7);

    data.setDefaultValue("R1", "C1", 0);
    data.setDefaultValue("R2", "C1", 0);
    data.setDefaultValue("R3", "C1", 0);

    data.addPairing("R1", "C1", {1, 2, '*'}, 5);
    data.addPairing("R1", "C1", {'*', 2, 1}, 1);

    data.addPairing("C1", "R2", {2, 3, 1}, 3);
    data.addPairing("C1", "R2", {'*', 1, '*'}, 1);

    data.addPairing("R3", "C1", {1, 4, 3}, 6);
    data.addPairing("R3", "C1", {3, '*', 1}, 2);
    data.addPairing("R3", "C1", {'*', 2, 3}, 1);

    EXPECT_FALSE(InputValidation::validate_names(data));
}

// Size of Row and Target are not same, Hence there is not one target for every row
TEST(ValidateTargetMatchesRow, SizeNotSame){
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);
    data.addColumn("0", 1);
    data.addColumn("1", 2);

    data.setTarget("0", 10);
    data.setTarget("1", 8);
    data.setTarget("2", 1);

    data.setDefaultValue("0", "0", 1);
    data.setDefaultValue("0", "1", 2);
    data.setDefaultValue("1", "0", 3);
    data.setDefaultValue("1", "1", 4);

    data.addPairing("0", "0", {1}, 9);
    data.addPairing("0", "0", {2}, 5);
    data.addPairing("0", "0", {'*'}, 2);

    data.addPairing("0", "1", {1, 2}, 7);
    data.addPairing("0", "1", {'*', 2}, 3);
    data.addPairing("0", "1", {'*', '*'}, 0);

    data.addPairing("1", "0", {1}, 6);
    data.addPairing("1", "0", {'*'}, 1);

    data.addPairing("1", "1", {1, '*'}, 8);
    data.addPairing("1", "1", {'*', 1}, 4);
    data.addPairing("1", "1", {'*', '*'}, 0);
    EXPECT_FALSE(InputValidation::validate_unique_targets(data));
}

// The Target is empty
TEST(ValidateTargetMatchesRow, EmptyTarget){
    InputData data;
    data.addRow("A", 2);
    data.addColumn("X", 2);

    data.setDefaultValue("A", "X", 4);

    data.addPairing("A", "X", {1, 2}, 5);
    data.addPairing("A", "X", {1, '*'}, 2);
    data.addPairing("A", "X", {2, 1}, 3);
    data.addPairing("A", "X", {'*', 2}, 1);
    data.addPairing("A", "X", {'*', '*'}, 8);
    EXPECT_FALSE(InputValidation::validate_unique_targets(data));
}

// Same row has more than one target
TEST(ValidateTargetMatchesRow, RowWithMoreThanOneTarget){
    InputData data;
    data.addRow("R1", 2);
    data.addRow("R2", 3);
    data.addRow("R3", 4);
    data.addColumn("C1", 3);

    data.setTarget("R1", 6);
    data.setTarget("R2", 4);
    data.setTarget("R1", 7);

    data.setDefaultValue("R1", "C1", 0);
    data.setDefaultValue("R2", "C1", 0);
    data.setDefaultValue("R3", "C1", 0);

    data.addPairing("R1", "C1", {1, 2, '*'}, 5);
    data.addPairing("R1", "C1", {'*', 2, 1}, 1);

    data.addPairing("R2", "C1", {2, 3, 1}, 3);
    data.addPairing("R2", "C1", {'*', 1, '*'}, 1);

    data.addPairing("R3", "C1", {1, 4, 3}, 6);
    data.addPairing("R3", "C1", {3, '*', 1}, 2);
    data.addPairing("R3", "C1", {'*', 2, 3}, 1);
    EXPECT_FALSE(InputValidation::validate_unique_targets(data));
}

// The number of parameters describing entry does not match the number of number of data describing column
TEST(ValidatePairings, NotEqualToNumberOfDataInColumn){
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);
    data.addColumn("0", 1);
    data.addColumn("1", 2);

    data.setTarget("0", 10);
    data.setTarget("1", 8);

    data.setDefaultValue("0", "0", 1);
    data.setDefaultValue("0", "1", 2);
    data.setDefaultValue("1", "0", 3);
    data.setDefaultValue("1", "1", 4);

    data.addPairing("0", "0", {1}, 9);
    data.addPairing("0", "0", {2}, 5);
    data.addPairing("0", "0", {'*'}, 2);

    data.addPairing("0", "1", {1, 2}, 7);
    data.addPairing("0", "1", {'*'}, 3);
    data.addPairing("0", "1", {'*', '*'}, 0);

    data.addPairing("1", "0", {1, '*'}, 6);
    data.addPairing("1", "0", {'*', 5}, 1);

    data.addPairing("1", "1", {1, '*'}, 8);
    data.addPairing("1", "1", {'*', 1}, 4);
    data.addPairing("1", "1", {'*', '*'}, 0);

    EXPECT_FALSE(InputValidation::validate_pairings(data));
}

// Value in pattern is not in range [1,number of data describing row]
TEST(ValidatePairings, OutOfRange){
    InputData data;
    data.addRow("R1", 2);
    data.addRow("R2", 3);
    data.addRow("R3", 4);
    
    data.addColumn("C1", 3);

    data.setTarget("R1", 6);
    data.setTarget("R2", 4);
    data.setTarget("R3", 7);

    data.setDefaultValue("R1", "C1", 0);
    data.setDefaultValue("R2", "C1", 0);
    data.setDefaultValue("R3", "C1", 0);

    data.addPairing("R1", "C1", {1, 3, '*'}, 5);
    data.addPairing("R1", "C1", {'*', 2, 1}, 1);

    data.addPairing("R2", "C1", {2, 5, 1}, 3);
    data.addPairing("R2", "C1", {'*', 1, '*'}, 1);

    data.addPairing("R3", "C1", {1, 4, 3}, 6);
    data.addPairing("R3", "C1", {3, '*', 1}, 2);
    data.addPairing("R3", "C1", {'*', 2, 3}, 1);

    EXPECT_FALSE(InputValidation::validate_pairings(data));
}


// Main Function
// Runs all the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}