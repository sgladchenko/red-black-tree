CXX = g++

.PHONY: simple_tests
simple_tests: source/bst.h tests/simple_tests.cpp
	g++ -std=c++17 -I source/ tests/simple_tests.cpp -o build/simple_tests
	./build/simple_tests