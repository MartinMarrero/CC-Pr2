#include "Tape.h"

#include <algorithm>
#include <stdexcept>

/** @brief Creates an empty tape with its head at position zero. */
Tape::Tape(char blankSymbol, bool infiniteBothDirections)
    : headPosition_(0), blankSymbol_(blankSymbol),
      infiniteBothDirections_(infiniteBothDirections) {}

/** @brief Reads the symbol under the head. */
char Tape::read() const { return getSymbolAt(headPosition_); }

/** @brief Writes a symbol in the current head position. */
void Tape::write(char symbol) { cells_[headPosition_] = symbol; }

/** @brief Moves the head one position to the left. */
void Tape::moveLeft() {
    if (!infiniteBothDirections_ && headPosition_ == 0) {
        throw std::out_of_range("Tape head cannot move left of position zero");
    }
    --headPosition_;
}

/** @brief Moves the head one position to the right. */
void Tape::moveRight() { ++headPosition_; }

/** @brief Applies L, R, or S to the tape head. */
void Tape::move(char direction) {
    if (direction == 'L') {
        moveLeft();
    } else if (direction == 'R') {
        moveRight();
    } else if (direction != 'S') {
        throw std::invalid_argument("Invalid tape movement: " + std::string(1, direction));
    }
}

/** @brief Loads an input string starting at position zero. */
void Tape::initialize(const std::string& input) {
    cells_.clear();
    headPosition_ = 0;
    for (std::size_t i = 0; i < input.size(); ++i) {
        cells_[static_cast<long long>(i)] = input[i];
    }
}

/** @brief Returns the current head position. */
long long Tape::getHeadPosition() const { return headPosition_; }

/** @brief Reads a position without moving the head. */
char Tape::getSymbolAt(long long position) const {
    const auto it = cells_.find(position);
    return it == cells_.end() ? blankSymbol_ : it->second;
}

/** @brief Returns the finite range of cells relevant to the current result. */
std::string Tape::getVisibleContents() const {
    if (cells_.empty()) {
        return std::string(1, blankSymbol_);
    }
    long long first = cells_.begin()->first;
    long long last = cells_.rbegin()->first;
    first = std::min(first, headPosition_);
    last = std::max(last, headPosition_);
    std::string result;
    for (long long position = first; position <= last; ++position) {
        result += getSymbolAt(position);
    }
    return result;
}
