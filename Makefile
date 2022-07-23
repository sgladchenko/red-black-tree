CXX = g++
BUILD = build
TESTS = tests
CXXSTD = -std=c++14
GTEST = -lgtest
INCLUDE = -I source/

.PHONY: tests
tests: $(TESTS)/bstTests.cpp
	$(CXX) $(CXXSTD) $(TESTS)/bstTests.cpp $(INCLUDE) $(GTEST) -o $(BUILD)/tests