SRC_DIR = src
OUT_DIR = dist

SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix_multiply.cpp $(SRC_DIR)/race_condition.cpp $(SRC_DIR)/synchronized_add.cpp
HDR = $(SRC_DIR)/matrix_multiply.hpp $(SRC_DIR)/race_condition.hpp $(SRC_DIR)/synchronized_add.hpp

CXX = g++
CXXFLAGS = -std=c++17 -O0 -pthread

TARGET = $(OUT_DIR)/main

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	@rm -f $(TARGET)
	@echo "Cleaned"

.PHONY: all clean
