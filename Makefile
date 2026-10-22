CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

TARGET := turing_simulator
BUILD_DIR := build

SOURCES := $(wildcard src/*.cc)
OBJECTS := $(patsubst src/%.cc,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all clean run-machine1 run-machine2

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-machine1: $(TARGET)
	./$(TARGET) -config config/machine1.txt -in examples/machine1.txt

run-machine2: $(TARGET)
	./$(TARGET) -config config/machine2.txt -in examples/machine2.txt

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TARGET).exe
