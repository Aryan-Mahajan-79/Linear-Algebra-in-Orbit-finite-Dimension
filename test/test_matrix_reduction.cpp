#include "gtest/gtest.h"
#include "input_data.h"
#include "matrix_reduction.h"

using PatternElement = PatternKey::PatternKey::PatternElement;

// Some of the default values are defined. Other possible default values should be added and set as zero
TEST(FillMissingDefaultValuesWithZero, SomeDefaultValuesDefined) {
    InputData data;
    data.addRow("R1", 2);
    data.addRow("R2", 3);
    data.addColumn("C1", 4);
    data.addColumn("C2", 5);

    data.setDefaultValue("R1", "C1", 8);

    MatrixReduction::fill_missing_default_values_with_zero(data);

    const auto& defaults = data.getDefaultValues();

    // Total expected default values = 2 rows * 2 cols = 4
    ASSERT_EQ(defaults.size(), 4u);

    EXPECT_EQ(data.getDefaultValue("R1", "C1"), 8);  // should not be overwritten
    EXPECT_EQ(data.getDefaultValue("R1", "C2"), 0);
    EXPECT_EQ(data.getDefaultValue("R2", "C1"), 0);
    EXPECT_EQ(data.getDefaultValue("R2", "C2"), 0);
}

// All  the default values are already defined. No changes should be made.
TEST(FillMissingDefaultValuesWithZero, AllDefaultValuesDefined) {
    InputData data;
    data.addRow("R1", 2);
    data.addColumn("C1", 2);
    data.setDefaultValue("R1", "C1", 5);

    MatrixReduction::fill_missing_default_values_with_zero(data);

    EXPECT_EQ(data.getDefaultValue("R1", "C1"), 5);
    EXPECT_EQ(data.getDefaultValues().size(), 1u);
}

// In definition of matrix, no default value is added so all must be set to zero
TEST(FillMissingDefaultValuesWithZero, NoDefaultValuesDefined) {
    InputData data;
    data.addRow("R1", 2);
    data.addColumn("C1", 2);

    MatrixReduction::fill_missing_default_values_with_zero(data);

    EXPECT_EQ(data.getDefaultValues().size(), 1u);
}

// For large matrix, final number of default values = number of rows * number of columns
TEST(FillMissingDefaultValuesWithZero, LargeMatrix) {
    InputData data;
    for (int i = 0; i < 100; ++i) {
        data.addRow("R" + std::to_string(i), 1);
        data.addColumn("C" + std::to_string(i), 1);
    }

    MatrixReduction::fill_missing_default_values_with_zero(data);
    EXPECT_EQ(data.getDefaultValues().size(), 10000u); // 100 × 100
}

// Only one pairing is defined explicitly. All other entries must be default value.
TEST(FillMissingPairingWithDefaultValues, SomePairingsDefined) {
    InputData data;
    data.addRow("R1", 2);
    data.addColumn("C1", 2);
    data.setDefaultValue("R1", "C1", 4);

    data.addPairing("R1", "C1", {1, 1}, 9);

    MatrixReduction::fill_missing_pairing_with_the_default_values(data);

    const auto& pairings = data.getPairings();
    // data.print();
    // Expect 9 patterns total for R1:2 and C1:2
    EXPECT_EQ(pairings.size(), 9u);

    std::vector<std::vector<PatternKey::PatternElement>> expected_patterns = {
        {1, 1}, {1, 2}, {2, 1}, {2, 2},
        {1, '*'}, {2, '*'}, {'*', 1}, {'*', 2}, {'*', '*'}
    };

    for (const auto& pattern : expected_patterns) {
        PatternKey key("R1", "C1", pattern);
        auto it = pairings.find(key);
        ASSERT_NE(it, pairings.end());
        if (pattern == std::vector<PatternKey::PatternElement>{1, 1}) {
            EXPECT_EQ(it->second, 9);
        } else {
            EXPECT_EQ(it->second, 4);
        }
    }
}

// No pairing is defined explicitly.
TEST(FillMissingPairingWithDefaultsValues, NoPairingsDefined) {
    InputData data;
    data.addRow("R1", 2);
    data.addColumn("C1", 2);
    data.setDefaultValue("R1", "C1", 5);

    MatrixReduction::fill_missing_pairing_with_the_default_values(data);

    EXPECT_EQ(data.getPairings().size(), 9u);
    for (const auto& [key, val] : data.getPairings()) {
        EXPECT_EQ(val, 5);
    }
}

// Pairings with '*'
TEST(FillMissingPairingWithDefaultsValues, PartiallyFilledPairings) {
    InputData data;
    data.addRow("R1", 1); 
    data.addColumn("C1", 2); 
    data.setDefaultValue("R1", "C1", 9);

    data.addPairing("R1", "C1", {'*', '*'}, 8);

    MatrixReduction::fill_missing_pairing_with_the_default_values(data);

    std::map<std::vector<PatternElement>, int> expected = {
        {{1, 1}, 9},
        {{1, '*'}, 9},
        {{'*', 1}, 9},
        {{'*', '*'}, 8} 
    };

    const auto& pairings = data.getPairings();
    EXPECT_EQ(pairings.size(), expected.size());

    for (const auto& [key, val] : pairings) {
        const auto& pattern = key.getPattern();
        ASSERT_TRUE(expected.count(pattern)) << "Unexpected pattern found!";
        EXPECT_EQ(val, expected.at(pattern)) << "Wrong value for pattern.";
    }
}

TEST(FillMissingPairingWithDefaultsValues, MultipleRowsAndColumns) {
    InputData data;

    data.addRow("R1", 2);
    data.addRow("R2", 2);
    data.addColumn("C1", 2);
    data.addColumn("C2", 2);

    data.setDefaultValue("R1", "C1", 5);
    data.setDefaultValue("R1", "C2", 7);
    data.setDefaultValue("R2", "C1", 9);
    data.setDefaultValue("R2", "C2", 10);

    data.addPairing("R1", "C1", {'*', '*'}, 12);
    data.addPairing("R1", "C2", {1, '*'}, 15);
    data.addPairing("R2", "C1", {2, 1}, 2);

    MatrixReduction::fill_missing_pairing_with_the_default_values(data);

    const auto& pairings = data.getPairings();

    // 9 each for each pair of row and column
    EXPECT_EQ(pairings.size(), 36u);

    auto wildcard_key = PatternKey("R1", "C1", {'*', '*'});
    auto it = pairings.find(wildcard_key);
    ASSERT_NE(it, pairings.end());
    EXPECT_EQ(it->second, 12);

    auto p1 = PatternKey("R2", "C2", {1, 1});
    auto p2 = PatternKey("R2", "C2", {'*', 2});
    auto p3 = PatternKey("R1", "C2", {'*', 1});
    EXPECT_EQ(pairings.at(p1), 10);
    EXPECT_EQ(pairings.at(p2), 10);
    EXPECT_EQ(pairings.at(p3), 7);
}



TEST(GetMaxDimensionOfTheMatrix, SingleElement) {
    InputData data;

    data.addRow("R1", 5);
    EXPECT_EQ(MatrixReduction::get_maximum_dimension_of_the_matrix(data.getRows()), 5);
}

TEST(GetMaxDimensionOfTheMatrix, AllEqualSizes) {
    InputData data;
    data.addRow("R1", 3);
    data.addRow("R2", 3);
    data.addRow("R3", 3);

    EXPECT_EQ(MatrixReduction::get_maximum_dimension_of_the_matrix(data.getRows()), 3);
}

TEST(GetMaxDimensionOfTheMatrix, VaryingSizes) {
        InputData data;
    data.addRow("R1", 2);
    data.addRow("R2", 7);
    data.addRow("R3", 5);

    EXPECT_EQ(MatrixReduction::get_maximum_dimension_of_the_matrix(data.getRows()), 7);
}

// Row R1 is expanded and replaced by 4 rows of dimension 3
TEST(ReplaceRowByNMinus1Rows, BasicReplacement) {
    InputData data, new_data;
    data.addRow("R1", 4);

    MatrixReduction::add_new_rows_for_a_sigle_input_row_in_the_reduction_process(data, {"R1", 4}, new_data);

    const auto& rows = new_data.getRows();
    ASSERT_EQ(rows.size(), 3u);

    EXPECT_EQ(rows[0].first, "R1_1");
    EXPECT_EQ(rows[0].second, 3);
    EXPECT_EQ(rows[1].first, "R1_2");
    EXPECT_EQ(rows[1].second, 3);
    EXPECT_EQ(rows[2].first, "R1_3");
    EXPECT_EQ(rows[2].second, 3);
}