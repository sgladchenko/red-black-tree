#include <iostream>

#include "bst.h"

class Tests
{
public:
    static void test1()
    {
        std::cout << "test1\n";

        sg::bst<int> bst_example;

        int N = 8;
        int array[] = {12, 1, 3, 10, 5, 11, 2, 0};

        int N1 = 6;
        int array1[] = {4, 6, 7, 9, 20, -7};

        for (int i = 0; i < N; ++i) { bst_example.insert(array[i]); }

        for (int i = 0; i < N; ++i)
        {
            std::cout << "bst_example.exists(" << array[N-1 - i] << "): "
                    << bst_example.exists(array[N-1 - i]) << std::endl;
        }

        std::cout << std::endl;

        for (int i = 0; i < N1; ++i)
        {
            std::cout << "bst_example.exists(" << array1[i] << "): "
                      <<  bst_example.exists(array1[i]) << std::endl;
        }

        /*
        std::cout << std::endl;

        for (int i = 0; i < N; ++i)
        {
            auto pnode = bst_example.successor(bst_example.find(array[i]));
            std::cout << "bst_example.successor of " << array[i] << ": ";
            if (pnode)
            {
                std::cout <<  pnode->value << std::endl;
            }
            else
            {
                std::cout << "(it's max)" << std::endl;
            }
        }
        */

        std::cout << std::endl;

        for (int i = 0; i < N; ++i)
        {
            bst_example.remove(array[i]);
            std::cout << "After deleting " << array[i] << std::endl;
            for (int j = 0; j < N; ++j)
            {
                std::cout << "\t" << "bst_example.exists(" << array[j] << "): ";
                std::cout << bst_example.exists(array[j]) << std::endl;
            }
        }
    }
};

int main()
{
    Tests::test1();
    return 0;
}