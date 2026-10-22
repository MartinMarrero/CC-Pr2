#pragma once

#include <map>
#include <string>

class Tape {
public:
    explicit Tape(char blankSymbol = '_', bool infiniteBothDirections = true);

    char read() const;
    void write(char symbol);
    void moveLeft();
    void moveRight();
    void move(char direction);
    void initialize(const std::string& input);
    long long getHeadPosition() const;
    char getSymbolAt(long long position) const;
    std::string getVisibleContents() const;

private:
    std::map<long long, char> cells_;
    long long headPosition_;
    char blankSymbol_;
    bool infiniteBothDirections_;
};
