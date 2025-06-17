#pragma once

#include <string>
#include <vector>
#include <variant>

class PatternKey {
public:
    using PatternElement = std::variant<int, char>;

    PatternKey(std::string row, std::string column, std::vector<PatternElement> pattern);

    bool operator<(const PatternKey& other) const;
    bool operator==(const PatternKey& other) const;

    const std::string& getRow() const;
    const std::string& getColumn() const;
    const std::vector<PatternElement>& getPattern() const;

private:
    std::string row_;
    std::string column_;
    std::vector<PatternElement> pattern_;

    static bool compareElement(const PatternElement& a, const PatternElement& b);
};
