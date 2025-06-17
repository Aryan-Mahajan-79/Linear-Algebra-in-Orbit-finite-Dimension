#include <gtest/gtest.h>
#include "input_data.h"
#include "input_validation.h"
#include "data_examples.h"

bool check_input_validity(const InputData& input_data);

// All valid inputs from input.cpp are true
TEST(ValidateIfOrbitNamesAreUniqueTest, AllInputsValid) {
    for (size_t i = 0; i < input_data_set.size(); ++i) {
        EXPECT_TRUE(InputValidation::check_input_validity(input_data_set[i])) << "InputData " << (i + 1) << " failed";
    }
}

// numbers of data supporting rows should be nonnegative 
TEST(ValidateIfNumberOfOrbitsAreNonNegative, NegativeOrbitRow){
    InputData data;
    data.addRow("A", 0);
    data.addRow("B", -1);
    
    EXPECT_FALSE(InputValidation::validate_if_number_of_orbits_are_non_negative(data.getRows(), data.getColumns()));
}

// numbers of data supporting columns should be nonnegative 
TEST(ValidateIfNumberOfOrbitsAreNonNegative, NegativeOrbitColumn){
    InputData data;

    data.addColumn("X", 1);
    data.addColumn("Y", -2);

    EXPECT_FALSE(InputValidation::validate_if_number_of_orbits_are_non_negative(data.getRows(), data.getColumns()));
}

// Row name or Column name is blank
TEST(ValidateIfOrbitNamesAreUniqueTest, BlankName){
    
    //rows
    InputData data;
    data.addRow("A", 2);
    data.addRow("", 1);

    EXPECT_FALSE(InputValidation::validate_if_orbit_names_are_unique(data.getRows(), data.getColumns()));

    //columns
    InputData data1;

    data1.addColumn("X", 1);
    data1.addColumn("", 2);

    EXPECT_FALSE(InputValidation::validate_if_orbit_names_are_unique(data1.getRows(), data1.getColumns()));
}

// Input does not have any Row
TEST(ValidateIfOrbitNamesAreUniqueTest, NoRow){
    InputData data;
    data.addColumn("X", 2);
    data.setTarget("A", 4);
    data.setDefaultValue("A", "X", 4);

    data.addPairing("A", "X", {1, 2}, 5);
    data.addPairing("A", "X", {1, '*'}, 2);
    data.addPairing("A", "X", {2, 1}, 3);
    data.addPairing("A", "X", {'*', 2}, 1);
    data.addPairing("A", "X", {'*', '*'}, 8);
    
    EXPECT_FALSE(InputValidation::validate_if_orbit_names_are_unique(data.getRows(), data.getColumns()));
}

// Input does not have any Column
TEST(ValidateIfOrbitNamesAreUniqueTest, NoColumn){
    InputData data;
    data.addRow("A", 2);
    data.setTarget("A", 4);
    data.setDefaultValue("A", "X", 4);

    data.addPairing("A", "X", {1, 2}, 5);
    data.addPairing("A", "X", {1, '*'}, 2);
    data.addPairing("A", "X", {2, 1}, 3);
    data.addPairing("A", "X", {'*', 2}, 1);
    data.addPairing("A", "X", {'*', '*'}, 8);   
    
    EXPECT_FALSE(InputValidation::validate_if_orbit_names_are_unique(data.getRows(), data.getColumns()));
}

// Duplicate Row name
TEST(ValidateIfOrbitNamesAreUniqueTest, DetectsDuplicateRows) {
    InputData data;
    data.addRow("A", 2);
    data.addRow("A", 1);
    data.addColumn("X", 1);
    data.addColumn("Y", 2);
    
    EXPECT_FALSE(InputValidation::validate_if_orbit_names_are_unique(data.getRows(), data.getColumns()));
}

// Duplicate Column name
TEST(ValidateIfOrbitNamesAreUniqueTest, DetectsDuplicateColumn) {
    InputData data;
    data.addRow("A", 2);
    data.addRow("B", 1);
    data.addColumn("X", 1);
    data.addColumn("X", 2);

    EXPECT_FALSE(InputValidation::validate_if_orbit_names_are_unique(data.getRows(), data.getColumns()));
}

// Some Row name and Column name are same. Hence invalid
TEST(ValidateIfOrbitNamesAreUniqueTest, DetectsOverlapBetweenRowsAndColumns) {
    InputData data;
    data.addRow("A", 2);
    data.addRow("X", 1);
    data.addColumn("X", 1);
    data.addColumn("Y", 2);
    
    EXPECT_FALSE(InputValidation::validate_if_orbit_names_are_unique(data.getRows(), data.getColumns()));
}

// The name of a row or a column in the target does not match with any row or column name
// Hence is invalid.
TEST(ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined, NotMatchingNamesinTarget){
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
  

    EXPECT_FALSE(InputValidation::validate_if_names_in_descriptions_of_target_and_matrix_are_defined(data));

}

// The name of a row or a column in the default value does not match with any row or column name
// Hence is invalid.
TEST(ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined, NotMatchingNamesinDefaultValue){
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
  

    EXPECT_FALSE(InputValidation::validate_if_names_in_descriptions_of_target_and_matrix_are_defined(data));

}

// The name of a row or a column in the pairings does not match with any row or column name
// Hence is invalid.
TEST(ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined, NotMatchingNamesinPairing){
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
  

    EXPECT_FALSE(InputValidation::validate_if_names_in_descriptions_of_target_and_matrix_are_defined(data));

}

// In Pairings the row name in place of column name or vice versa
TEST(ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined, RowColumnNameMismatch){
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

    EXPECT_FALSE(InputValidation::validate_if_names_in_descriptions_of_target_and_matrix_are_defined(data));
}

// For some row, there is no target defined corresponding to that row. 
TEST(ValidateTargetMatchesRow, NoTargetForARow){
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);

    data.setTarget("0", 10);

    EXPECT_FALSE(InputValidation::validate_unique_targets(data.getRows(), data.getTarget()));
}

// Size of Row and Target are not same, Hence there is not one target for every row
TEST(ValidateTargetMatchesRow, SizeNotSame){
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);

    data.setTarget("0", 10);
    data.setTarget("1", 8);
    data.setTarget("2", 1);

    EXPECT_FALSE(InputValidation::validate_unique_targets(data.getRows(), data.getTarget()));
}

// The Target is empty
TEST(ValidateTargetMatchesRow, EmptyTarget){
    InputData data;
    data.addRow("A", 2);

    EXPECT_FALSE(InputValidation::validate_unique_targets(data.getRows(), data.getTarget()));
}

// Same row has more than one target
TEST(ValidateTargetMatchesRow, RowWithMoreThanOneTarget){
    InputData data;
    data.addRow("R1", 2);
    data.addRow("R2", 3);
    data.addRow("R3", 4);

    data.setTarget("R1", 6);
    data.setTarget("R2", 4);
    data.setTarget("R1", 7);

    EXPECT_FALSE(InputValidation::validate_unique_targets(data.getRows(), data.getTarget()));
}

// The default value for a row column pair is not uniquely defined so print a warning and ignore the second setDefaultvalue()
TEST(ValidateSetDuplicateValue, DefaultValueIsDuplicated) {
    InputData data;

    data.setDefaultValue("A", "X", 10);
    data.setDefaultValue("A", "X", 20);

    const auto& defaults = data.getDefaultValues();
    auto key = std::make_pair("A", "X");

    ASSERT_TRUE(defaults.count(key));
    EXPECT_EQ(defaults.at(key), 10);  // Should NOT be 20
}

// For same row, column and pattern, a pairing is defined more than once
TEST(ValidateAddPairing, PairingIsDuplicated) {
    InputData data;

    using PatternElement = std::variant<int, char>;
    PatternKey key1("A", "X", { PatternElement(1) });
    PatternKey key2("B", "Y", { PatternElement(2) });

    data.addPairing(key1, 10);
    data.addPairing(key2, 20);
    data.addPairing(key1, 99);

    const auto& pairings = data.getPairings();

    ASSERT_EQ(pairings.size(), 2u);
    EXPECT_EQ(pairings.at(key1), 10);
    EXPECT_EQ(pairings.at(key2), 20);
}

// The number of parameters describing entry does not match the number of number of data describing column
TEST(ValidateIfAllMatrixEntryAreCorrect, NotEqualToNumberOfDataInColumn){
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

    EXPECT_FALSE(InputValidation::validate_if_all_matrix_entry_are_correct(data));
}

// Value in pattern is not in range [1,number of data describing row]
TEST(ValidateIfAllMatrixEntryAreCorrect, OutOfRange){
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

    EXPECT_FALSE(InputValidation::validate_if_all_matrix_entry_are_correct(data));
}


// Main Function
// Runs all the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}