#include <iostream>

#include "bst.h"

void test1()
{
    std::cout << "test1\n";

    sg::bst<int> bst_example;
    int N = 8;
    int array[] = {12, 1, 3, 10, 5, 11, 2, 0};

    for (int i = 0; i < N; ++i) { bst_example.insert(i); }

    for (int i = 0; i < N; ++i)
    {
        std::cout << "bst_example.exists(" << array[i] << "): "
                  << bst_example.exists(array[i]) << std::endl;
    }
}

void test2()
{
    std::cout << "test2\n";

    sg::bst<int> bst_example;
    int N = 3;
    int array[] = {12, 1, 3};

    for (int i = 0; i < N; ++i) { bst_example.insert(i); }

    for (int i = 0; i < N; ++i)
    {
        std::cout << "bst_example.exists(" << array[i] << "): "
                  << bst_example.exists(array[i]) << std::endl;
    }
}

int main()
{
    test2();
    return 0;
}