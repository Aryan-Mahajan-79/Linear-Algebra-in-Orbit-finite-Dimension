#pragma once

#include <string>
#include <vector>
#include <variant>
#include <functional>


class PatternKey {
public:
    using PatternElement = std::variant<int, char>;

    PatternKey(std::string row, std::string column, std::vector<PatternElement> pattern);

    bool operator<(const PatternKey& other) const;
    bool operator==(const PatternKey& other) const;

    const std::string& getRow() const;
    const std::string& getColumn() const;
    const std::vector<PatternElement>& getPattern() const;

    struct Hasher {
        std::size_t operator()(const PatternKey& key) const;
    };

private:
    std::string row_;
    std::string column_;
    std::vector<PatternElement> pattern_;

    static bool compareElement(const PatternElement& a, const PatternElement& b);
    static void hash_combine(std::size_t& seed, std::size_t value);
};