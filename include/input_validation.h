#pragma once

#include "input_data.h"

class InputValidation {
public:
    static bool check_input_validity(const InputData& input_data);

private:
//  static bool validate_general_structure_of_the_file(); 
    static bool validate_if_orbit_names_are_unique(const std::vector<InputData::RowOrCol> rows, const std::vector<InputData::RowOrCol> columns);
    static bool validate_if_number_of_orbits_are_non_negative(const std::vector<InputData::RowOrCol> rows, const std::vector<InputData::RowOrCol> columns);
    static bool validate_if_names_in_descriptions_of_target_and_matrix_are_defined(const InputData& input_data);
    static bool validate_if_for_every_row_target_is_uniquely_defined(const std::vector<InputData::RowOrCol> rows, const std::map<std::string, int> target);
    static bool validate_unique_targets(const std::vector<InputData::RowOrCol> rows, const std::map<std::string, int> target);
    static bool validate_if_default_values_are_defined_uniquely(); // It is a map so it is always true
    static bool validate_if_matrix_entires_are_defined_uniquely();     // It is a map so it is always true
    static bool validate_if_all_matrix_entry_are_correct(const InputData& input_data);
    static bool validate_if_single_matrix_entry_is_correct_column_size(int col_size, std::vector<PatternKey::PatternElement>pattern, const std::string& col_name);
    static bool validate_if_single_matrix_entry_is_correct_row_range(int row_size, std::vector<PatternKey::PatternElement>pattern, const std::string& row_name);


    // Defning friend classes only for testing purposes (Google Tests)
    friend class ValidateIfOrbitNamesAreUniqueTest_AllInputsValid_Test;
    friend class ValidateIfOrbitNamesAreUniqueTest_BlankName_Test;
    friend class ValidateIfOrbitNamesAreUniqueTest_NoRow_Test;
    friend class ValidateIfOrbitNamesAreUniqueTest_NoColumn_Test;
    friend class ValidateIfOrbitNamesAreUniqueTest_DetectsDuplicateRows_Test;
    friend class ValidateIfOrbitNamesAreUniqueTest_DetectsDuplicateColumn_Test;
    friend class ValidateIfOrbitNamesAreUniqueTest_DetectsOverlapBetweenRowsAndColumns_Test;

    friend class ValidateIfNumberOfOrbitsAreNonNegative_NegativeOrbitRow_Test;
    friend class ValidateIfNumberOfOrbitsAreNonNegative_NegativeOrbitColumn_Test;

    friend class ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined_NotMatchingNamesinTarget_Test;
    friend class ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined_NotMatchingNamesinDefaultValue_Test;
    friend class ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined_NotMatchingNamesinPairing_Test;
    friend class ValidateIfNamesInDescriptionsOfTargetAndMatrixAreDefined_RowColumnNameMismatch_Test;

    friend class ValidateTargetMatchesRow_NoTargetForARow_Test;
    friend class ValidateTargetMatchesRow_SizeNotSame_Test;
    friend class ValidateTargetMatchesRow_EmptyTarget_Test;
    friend class ValidateTargetMatchesRow_RowWithMoreThanOneTarget_Test;

    friend class ValidateSetDuplicateValue_DefaultValueIsNotUnique_Test;

    friend class ValidateAddPairing_PairingIsDuplicated_Test;

    friend class ValidateIfAllMatrixEntryAreCorrect_NotEqualToNumberOfDataInColumn_Test;
    friend class ValidateIfAllMatrixEntryAreCorrect_OutOfRange_Test;
};


