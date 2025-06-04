#include <gtest/gtest.h>
#include "input_example.h"

// Assume these validation functions are declared in some header you include
bool validate_orbit_names(const InputData& input_data);
bool check_input_validity(const InputData& input_data);

TEST(ValidateOrbitNamesTest, InputData1IsValid) {
    EXPECT_TRUE(validate_orbit_names(input_data_1));
}

TEST(ValidateOrbitNamesTest, InputData2IsValid) {
    EXPECT_TRUE(validate_orbit_names(input_data_2));
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

TEST(ValidateOrbitNamesTest, DetectsDuplicateCols) {
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
    
    
    EXPECT_FALSE(validate_orbit_names(data));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}