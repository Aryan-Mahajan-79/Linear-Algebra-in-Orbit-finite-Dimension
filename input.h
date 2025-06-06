#pragma once

#include <string>
#include <vector>
#include <map>
#include <variant>

using namespace std;

using PatternElement = variant<int, char>; // '*' is represented as char

struct PatternKey {
    string row;
    string col;
    vector<PatternElement> pattern;

    // Constructor for convenience
    PatternKey(string r, string c, vector<PatternElement> p)
        : row(move(r)), col(move(c)), pattern(move(p)) {}

    bool operator<(const PatternKey& other) const {
        if (row != other.row) return row < other.row;
        if (col != other.col) return col < other.col;
        if (pattern.size() != other.pattern.size()) return pattern.size() < other.pattern.size();
        for (size_t i = 0; i < pattern.size(); ++i) {
            if (pattern[i] != other.pattern[i]) return pattern[i] < other.pattern[i];
        }
        return false;
    }
};

struct InputData {
    vector<pair<string, int>> rows;
    vector<pair<string, int>> cols;
    map<string, int> target;
    map<pair<string, string>, int> default_values;
    map<PatternKey, int> pairings;
};

extern const InputData input_data_1;
extern const InputData input_data_2;
extern const InputData input_data_3;
extern const InputData input_data_4;

extern std::vector<InputData> input_data_set;