#pragma once

#include "pattern_key.h"
#include <string>
#include <vector>
#include <map>

class InputData {
public:
    using RowOrCol = std::pair<std::string, int>;

    void addRow(const std::string& name, int value);
    void addColumn(const std::string& name, int value);
    void setTarget(const std::string& key, int value);
    void setDefaultValue(const std::string& row, const std::string& col, int value);
    void addPairing(const PatternKey& key, int value);
    void addPairing(const std::string& row, const std::string& col,
                    const std::vector<PatternKey::PatternElement>& pattern, int value);

    const std::vector<RowOrCol>& getRows() const;
    const std::vector<RowOrCol>& getColumns() const;
    const std::map<std::string, int>& getTarget() const;
    const std::map<std::pair<std::string, std::string>, int>& getDefaultValues() const;
    const std::map<PatternKey, int>& getPairings() const;

private:
    std::vector<RowOrCol> rows_;
    std::vector<RowOrCol> columns_;
    std::map<std::string, int> target_;
    std::map<std::pair<std::string, std::string>, int> default_values_;
    std::map<PatternKey, int> pairings_;

};
