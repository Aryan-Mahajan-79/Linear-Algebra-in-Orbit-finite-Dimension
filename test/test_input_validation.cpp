#include <gtest/gtest.h>
#include "input.h"

// Assume these validation functions are declared in some header you include
bool validate_orbit_names(const InputData& input_data);
bool check_input_validity(const InputData& input_data);
bool validate_names(const InputData& input_data);
bool validate_unique_targets(const InputData& input_data);
bool validate_pairings(const InputData& input_data);

// All valid inputs from input.cpp are true
TEST(ValidateOrbitNamesTest, AllInputsValid) {
    for (size_t i = 0; i < input_data_set.size(); ++i) {
        EXPECT_TRUE(validate_orbit_names(input_data_set[i])) << "InputData " << (i + 1) << " failed";
    }
}

// Row name or Column name is blank
TEST(ValidateOrbitNamesTest, BlankName){
    InputData data;
    data.rows = {{"A", 2}, {"B", 1}};
    data.cols = {{"", 1}, {"Y", 2}};
    data.target = {
        {"A", 4},
        {"A", 7}
    };
    data.default_values = {
        {{"A", "X"}, 0},
        {{"A", "Y"}, 0},
        {{"A", "X"}, 0},
        {{"A", "Y"}, 0}
    };
    data.pairings = {
        { {"A", "X", {1}}, 6 },
        { {"A", "X", {2}}, 3 },
        { {"A", "X", {'*'}}, 1 },

        { {"A", "Y", {1, 2}}, 4 },
        { {"A", "Y", {'*', 2}}, 2 },
        { {"A", "Y", {'*', '*'}}, 0 },

        { {"B", "X", {1}}, 7 },
        { {"B", "X", {'*'}}, 3 },

        { {"B", "Y", {1, '*'}}, 5 },
        { {"B", "Y", {'*', 1}}, 2 },
        { {"B", "Y", {'*', '*'}}, 0 }
    };
    
    
    EXPECT_FALSE(validate_orbit_names(data));
}

// Input does not have any Row
TEST(ValidateOrbitNamesTest, NoRow){
    InputData data;
    data.rows = {};
    data.cols = {{"X", 2}};
    data.target = {{"A", 4}};
    data.default_values = {
        {{"A", "X"}, 4}
    };
    data.pairings = {
        { {"A", "X", {1, 2}}, 5 },
        { {"A", "X", {1, '*'}}, 2 },
        { {"A", "X", {2, 1}}, 3 },
        { {"A", "X", {'*', 2}}, 1 },
        { {"A", "X", {'*', '*'}}, 8 }
    };    
    
    EXPECT_FALSE(validate_orbit_names(data));
}

// Input does not have any Column
TEST(ValidateOrbitNamesTest, NoColumn){
    InputData data;
    data.rows = {{"A", 2}};
    data.cols = {};
    data.target = {{"A", 4}};
    data.default_values = {
        {{"A", "X"}, 4}
    };
    data.pairings = {
        { {"A", "X", {1, 2}}, 5 },
        { {"A", "X", {1, '*'}}, 2 },
        { {"A", "X", {2, 1}}, 3 },
        { {"A", "X", {'*', 2}}, 1 },
        { {"A", "X", {'*', '*'}}, 8 }
    };    
    
    EXPECT_FALSE(validate_orbit_names(data));
}

// Duplicate Row name
TEST(ValidateOrbitNamesTest, DetectsDuplicateRows) {
    InputData data;
    data.rows = {{"A", 2}, {"A", 1}};
    data.cols = {{"X", 1}, {"Y", 2}};
    data.target = {
        {"A", 4},
        {"A", 7}
    };
    data.default_values = {
        {{"A", "X"}, 0},
        {{"A", "Y"}, 0},
        {{"A", "X"}, 0},
        {{"A", "Y"}, 0}
    };
    data.pairings = {
        { {"A", "X", {1}}, 6 },
        { {"A", "X", {2}}, 3 },
        { {"A", "X", {'*'}}, 1 },

        { {"A", "Y", {1, 2}}, 4 },
        { {"A", "Y", {'*', 2}}, 2 },
        { {"A", "Y", {'*', '*'}}, 0 },

        { {"A", "X", {1}}, 7 },
        { {"A", "X", {'*'}}, 3 },

        { {"A", "Y", {1, '*'}}, 5 },
        { {"A", "Y", {'*', 1}}, 2 },
        { {"A", "Y", {'*', '*'}}, 0 }
    };
    
    
    EXPECT_FALSE(validate_orbit_names(data));
}

// Duplicate Column name
TEST(ValidateOrbitNamesTest, DetectsDuplicateColumn) {
    InputData data;

    data.rows = {{"A", 2}, {"B", 1}};
    data.cols = {{"X", 1}, {"X", 2}};
    data.target = {
        {"A", 4},
        {"B", 7}
    };
    data.default_values = {
        {{"A", "X"}, 0},
        {{"A", "X"}, 0},
        {{"B", "X"}, 0},
        {{"B", "X"}, 0}
    };
    data.pairings = {
        { {"A", "X", {1}}, 6 },
        { {"A", "X", {2}}, 3 },
        { {"A", "X", {'*'}}, 1 },

        { {"A", "X", {1, 2}}, 4 },
        { {"A", "X", {'*', 2}}, 2 },
        { {"A", "X", {'*', '*'}}, 0 },

        { {"B", "X", {1}}, 7 },
        { {"B", "X", {'*'}}, 3 },

        { {"B", "X", {1, '*'}}, 5 },
        { {"B", "X", {'*', 1}}, 2 },
        { {"B", "X", {'*', '*'}}, 0 }
    };

    EXPECT_FALSE(validate_orbit_names(data));
}

// For some matrices it makes sense to have same row and column name eg. symmetric matrices
TEST(ValidateOrbitNamesTest, DetectsOverlapBetweenRowsAndCols) {
    InputData data;
    data.rows = {{"A", 2}, {"X", 1}};
    data.cols = {{"X", 1}, {"Y", 2}};
    data.target = {
        {"A", 4},
        {"X", 7}
    };
    data.default_values = {
        {{"A", "X"}, 0},
        {{"A", "Y"}, 0},
        {{"X", "X"}, 0},
        {{"X", "Y"}, 0}
    };
    data.pairings = {
        { {"A", "X", {1}}, 6 },
        { {"A", "X", {2}}, 3 },
        { {"A", "X", {'*'}}, 1 },

        { {"A", "Y", {1, 2}}, 4 },
        { {"A", "Y", {'*', 2}}, 2 },
        { {"A", "Y", {'*', '*'}}, 0 },

        { {"X", "X", {1}}, 7 },
        { {"X", "X", {'*'}}, 3 },

        { {"X", "Y", {1, '*'}}, 5 },
        { {"X", "Y", {'*', 1}}, 2 },
        { {"X", "Y", {'*', '*'}}, 0 }
    };
    
    EXPECT_TRUE(validate_orbit_names(data));
}

// All valid inputs from input.cpp are true
TEST(ValidateAllNames, AllInputsValid){
    for (size_t i = 0; i < input_data_set.size(); ++i) {
        EXPECT_TRUE(validate_names(input_data_set[i])) << "InputData " << (i + 1) << " failed";
    }
}

// The name of row or column in target or default values or pairings does not match with any row or column name
// Hence is invalid.
TEST(ValidateAllNames, NotMatchingNames){
    InputData data;
    data.rows = {{"0", 2}, {"1", 1}};
    data.cols = {{"0", 1}, {"1", 2}};
    data.target = {
        {"0", 10},
        {"1", 8}
    };
    data.default_values = {
        {{"0", "0"}, 1},
        {{"0", "1"}, 2},
        {{"1", "0"}, 3},
        {{"1", "1"}, 4}
    };
    data.pairings = {
        { {"5", "0", {1}}, 9 },
        { {"5", "0", {2}}, 5 },
        { {"5", "0", {'*'}}, 2 },

        { {"0", "1", {1, 2}}, 7 },
        { {"0", "1", {'*', 2}}, 3 },
        { {"0", "1", {'*', '*'}}, 0 },

        { {"1", "0", {1}}, 6 },
        { {"1", "0", {'*'}}, 1 },

        { {"1", "1", {1, '*'}}, 8 },
        { {"1", "1", {'*', 1}}, 4 },
        { {"1", "1", {'*', '*'}}, 0 }
    };

    EXPECT_FALSE(validate_names(data));

}

// In Pairings the row name in place of column name or vice versa
TEST(ValidateAllNames, RowColNameMismatch){
    InputData data;
    data.rows = {{"R1", 2}, {"R2", 3}, {"R3", 4}};
    data.cols = {{"C1", 3}};  // Only one column
    data.target = {
        {"R1", 6},
        {"R2", 4},
        {"R3", 7}
    };
    data.default_values = {
        {{"R1", "C1"}, 0},
        {{"R2", "C1"}, 0},
        {{"R3", "C1"}, 0}
    };
    data.pairings = {
        { {"R1", "C1", {1, 2, '*'}}, 5 },
        { {"R1", "C1", {'*', 2, 1}}, 1 },

        { {"C1", "R2", {2, 3, 1}}, 3 },
        { {"C1", "R2", {'*', 1, '*'}}, 1 },

        { {"R3", "C1", {1, 4, 3}}, 6 },
        { {"R3", "C1", {3, '*', 1}}, 2 },
        { {"R3", "C1", {'*', 2, 3}}, 1 }
    };
    EXPECT_FALSE(validate_names(data));
}

// All valid inputs from input.cpp are true
TEST(ValidateTargetMatchesRow, AllInputsValid){
    for (size_t i = 0; i < input_data_set.size(); ++i) {
        EXPECT_TRUE(validate_unique_targets(input_data_set[i])) << "InputData " << (i + 1) << " failed";
    }
}

// Size of Row and Target are not same, Hence there is not one target for every row
TEST(ValidateTargetMatchesRow, SizeNotSame){
    InputData data;
    data.rows = {{"0", 2}, {"1", 1}};
    data.cols = {{"0", 1}, {"1", 2}};
    data.target = {
        {"0", 10},
        {"1", 8},
        {"2", 0}
    };
    data.default_values = {
        {{"0", "0"}, 1},
        {{"0", "1"}, 2},
        {{"1", "0"}, 3},
        {{"1", "1"}, 4}
    };
    data.pairings = {
        { {"0", "0", {1}}, 9 },
        { {"0", "0", {2}}, 5 },
        { {"0", "0", {'*'}}, 2 },

        { {"0", "1", {1, 2}}, 7 },
        { {"0", "1", {'*', 2}}, 3 },
        { {"0", "1", {'*', '*'}}, 0 },

        { {"1", "0", {1}}, 6 },
        { {"1", "0", {'*'}}, 1 },

        { {"1", "1", {1, '*'}}, 8 },
        { {"1", "1", {'*', 1}}, 4 },
        { {"1", "1", {'*', '*'}}, 0 }
    };
    EXPECT_FALSE(validate_unique_targets(data));
}

// The Target is empty
TEST(ValidateTargetMatchesRow, EmptyTarget){
    InputData data;
    data.rows = {{"A", 2}};
    data.cols = {{"X", 2}};
    data.target = {};
    data.default_values = {
        {{"A", "X"}, 4}
    };
    data.pairings = {
        { {"A", "X", {1, 2}}, 5 },
        { {"A", "X", {1, '*'}}, 2 },
        { {"A", "X", {2, 1}}, 3 },
        { {"A", "X", {'*', 2}}, 1 },
        { {"A", "X", {'*', '*'}}, 8 }
    };
    EXPECT_FALSE(validate_unique_targets(data));
}

// Same row has more than one target
TEST(ValidateTargetMatchesRow, RowWithMoreThanOneTarget){
    InputData data;
    data.rows = {{"R1", 2}, {"R2", 3}, {"R3", 4}};
    data.cols = {{"C1", 3}};  // Only one column
    data.target = {
        {"R1", 6},
        {"R1", 4},
        {"R3", 7}
    };
    data.default_values = {
        {{"R1", "C1"}, 0},
        {{"R2", "C1"}, 0},
        {{"R3", "C1"}, 0}
    };
    data.pairings = {
        { {"R1", "C1", {1, 2, '*'}}, 5 },
        { {"R1", "C1", {'*', 2, 1}}, 1 },

        { {"R2", "C1", {2, 3, 1}}, 3 },
        { {"R2", "C1", {'*', 1, '*'}}, 1 },

        { {"R3", "C1", {1, 4, 3}}, 6 },
        { {"R3", "C1", {3, '*', 1}}, 2 },
        { {"R3", "C1", {'*', 2, 3}}, 1 }
    };
    EXPECT_FALSE(validate_unique_targets(data));
}



// All valid inputs from input.cpp are true
TEST(ValidatePairings, AllInputsValid){
    for (size_t i = 0; i < input_data_set.size(); ++i) {
        EXPECT_TRUE(validate_pairings(input_data_set[i])) << "InputData " << (i + 1) << " failed";
    }
}

// The number of parameters describing entry does not match the number of number of data describing column
TEST(ValidatePairings, NotEqualToNumberOfDataInColumn){
    InputData data;
    data.rows = {{"0", 2}, {"1", 1}};
    data.cols = {{"0", 1}, {"1", 2}};
    data.target = {
        {"0", 10},
        {"1", 8}
    };
    data.default_values = {
        {{"0", "0"}, 1},
        {{"0", "1"}, 2},
        {{"1", "0"}, 3},
        {{"1", "1"}, 4}
    };
    data.pairings = {
        { {"0", "0", {1, 3}}, 9 },
        { {"0", "0", {2 ,'*'}}, 5 },
        { {"0", "0", {'*'}}, 2 },

        { {"0", "1", {1, 2}}, 7 },
        { {"0", "1", {'*', 2}}, 3 },
        { {"0", "1", {'*', '*'}}, 0 },

        { {"1", "0", {1}}, 6 },
        { {"1", "0", {'*'}}, 1 },

        { {"1", "1", {1, '*'}}, 8 },
        { {"1", "1", {'*', 1}}, 4 },
        { {"1", "1", {'*', '*'}}, 0 }
    };

    EXPECT_FALSE(validate_pairings(data));
}

// Value in pattern is not in range [1,number of data describing row]
TEST(ValidatePairings, OutOfRange){
    InputData data;
    data.rows = {{"R1", 2}, {"R2", 3}, {"R3", 4}};
    data.cols = {{"C1", 3}};  // Only one column
    data.target = {
        {"R1", 6},
        {"R2", 4},
        {"R3", 7}
    };
    data.default_values = {
        {{"R1", "C1"}, 0},
        {{"R2", "C1"}, 0},
        {{"R3", "C1"}, 0}
    };
    data.pairings = {
        { {"R1", "C1", {1, 5, '*'}}, 5 },
        { {"R1", "C1", {'*', 2, 1}}, 1 },

        { {"R2", "C1", {2, 3, 1}}, 3 },
        { {"R2", "C1", {'*', 1, '*'}}, 1 },

        { {"R3", "C1", {1, 4, 3}}, 6 },
        { {"R3", "C1", {3, '*', 1}}, 2 },
        { {"R3", "C1", {'*', 2, 3}}, 1 }
    };

    EXPECT_FALSE(validate_pairings(data));
}


// Main Function
// Runs all the tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}