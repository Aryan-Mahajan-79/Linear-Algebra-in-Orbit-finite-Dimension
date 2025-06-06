#pragma once
#include "input.h"

bool check_input_validity(const InputData& input_data);
bool validate_orbit_names(const InputData& input_data);
bool validate_names(const InputData& inout_data);
bool is_only_spaces(const std::string& s);
bool validate_unique_targets(const InputData& input_data);