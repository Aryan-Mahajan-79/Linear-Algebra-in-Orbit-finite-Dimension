#include "input_data.h"
#include <vector>

using std::vector;

const InputData input_data_1 = [] {
    InputData data;
    data.addRow("A", 2);
    data.addColumn("X", 2);
    data.setTarget("A", 4);
    data.setDefaultValue("A", "X", 4);

    data.addPairing("A", "X", {1, 2}, 5);
    data.addPairing("A", "X", {1, '*'}, 2);
    data.addPairing("A", "X", {2, 1}, 3);
    data.addPairing("A", "X", {'*', 2}, 1);
    data.addPairing("A", "X", {'*', '*'}, 8);

    return data;
}();

const InputData input_data_2 = [] {
    InputData data;
    data.addRow("A", 2);
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

    return data;
}();

const InputData input_data_3 = [] {
    InputData data;
    data.addRow("0", 2);
    data.addRow("1", 1);
    data.addColumn("2", 1);
    data.addColumn("3", 2);

    data.setTarget("0", 10);
    data.setTarget("1", 8);

    data.setDefaultValue("0", "2", 1);
    data.setDefaultValue("0", "3", 2);
    data.setDefaultValue("1", "2", 3);
    data.setDefaultValue("1", "3", 4);

    data.addPairing("0", "2", {1}, 9);
    data.addPairing("0", "2", {2}, 5);
    data.addPairing("0", "2", {'*'}, 2);

    data.addPairing("0", "3", {1, 2}, 7);
    data.addPairing("0", "3", {'*', 2}, 3);
    data.addPairing("0", "3", {'*', '*'}, 0);

    data.addPairing("1", "2", {1}, 6);
    data.addPairing("1", "2", {'*'}, 1);

    data.addPairing("1", "3", {1, '*'}, 8);
    data.addPairing("1", "3", {'*', 1}, 4);
    data.addPairing("1", "3", {'*', '*'}, 0);

    return data;
}();

const InputData input_data_4 = [] {
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

    data.addPairing("R2", "C1", {2, 3, 1}, 3);
    data.addPairing("R2", "C1", {'*', 1, '*'}, 1);

    data.addPairing("R3", "C1", {1, 4, 3}, 6);
    data.addPairing("R3", "C1", {3, '*', 1}, 2);
    data.addPairing("R3", "C1", {'*', 2, 3}, 1);

    return data;
}();

vector<InputData> input_data_set = {
    input_data_1,
    input_data_2,
    input_data_3,
    input_data_4
};
