#include "input.h"
#include<vector>


const InputData input_data_1 = [] {
    InputData data;
    data.rows = {{"A", 2}};
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
    return data;
}();

const InputData input_data_2 = [] {
    InputData data;
    data.rows = {{"A", 2}, {"B", 1}};
    data.cols = {{"X", 1}, {"Y", 2}};
    data.target = {
        {"A", 4},
        {"B", 7}
    };
    data.default_values = {
        {{"A", "X"}, 0},
        {{"A", "Y"}, 0},
        {{"B", "X"}, 0},
        {{"B", "Y"}, 0}
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
    return data;
}();


const InputData input_data_3 = [] {
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
    return data;
}();

const InputData input_data_4 = [] {
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

        { {"R2", "C1", {2, 3, 1}}, 3 },
        { {"R2", "C1", {'*', 1, '*'}}, 1 },

        { {"R3", "C1", {1, 4, 3}}, 6 },
        { {"R3", "C1", {3, '*', 1}}, 2 },
        { {"R3", "C1", {'*', 2, 3}}, 1 }
    };
    return data;
}();

vector<InputData> input_data_set = {
    input_data_1,
    input_data_2,
    input_data_3,
    input_data_4
};