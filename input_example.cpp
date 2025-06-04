#include "input_example.h"

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

