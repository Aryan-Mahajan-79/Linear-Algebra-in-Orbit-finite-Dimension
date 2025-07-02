#include "pattern_key.h"

PatternKey::PatternKey(std::string row, std::string column, std::vector<PatternElement> pattern)
    : row_(std::move(row)), column_(std::move(column)), pattern_(std::move(pattern)) {}

const std::string& PatternKey::getRow() const { return row_; }
const std::string& PatternKey::getColumn() const { return column_; }
const std::vector<PatternKey::PatternElement>& PatternKey::getPattern() const { return pattern_; }

bool PatternKey::compareElement(const PatternElement& a, const PatternElement& b) {
    if (a.index() != b.index())
        return a.index() < b.index();
    if (std::holds_alternative<int>(a))
        return std::get<int>(a) < std::get<int>(b);
    return std::get<char>(a) < std::get<char>(b);
}

bool PatternKey::operator<(const PatternKey& other) const {
    if (row_ != other.row_) return row_ < other.row_;
    if (column_ != other.column_) return column_ < other.column_;
    if (pattern_.size() != other.pattern_.size()) return pattern_.size() < other.pattern_.size();
    for (size_t i = 0; i < pattern_.size(); ++i) {
        if (pattern_[i] != other.pattern_[i])
            return compareElement(pattern_[i], other.pattern_[i]);
    }
    return false;
}

bool PatternKey::operator==(const PatternKey& other) const {
    return row_ == other.row_ &&
           column_ == other.column_ &&
           pattern_ == other.pattern_;
}

void PatternKey::hash_combine(std::size_t& seed, std::size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

std::size_t PatternKey::Hasher::operator()(const PatternKey& key) const {
    std::size_t seed = 0;

    PatternKey::hash_combine(seed, std::hash<std::string>{}(key.getRow()));
    PatternKey::hash_combine(seed, std::hash<std::string>{}(key.getColumn()));

    for (const auto& elem : key.getPattern()) {
        if (std::holds_alternative<int>(elem)) {
            PatternKey::hash_combine(seed, std::hash<int>{}(std::get<int>(elem)));
        } else {
            PatternKey::hash_combine(seed, std::hash<char>{}(std::get<char>(elem)));
        }
    }

    return seed;
}

int PatternKey::convert_to_int(const PatternKey::PatternElement& element) {
        if (std::holds_alternative<int>(element)) {
            return std::get<int>(element);
        } else if (std::holds_alternative<char>(element)) {
            return -1; // Wildcard represented by '*'
        }
        throw std::runtime_error("Invalid PatternElement type");
    }