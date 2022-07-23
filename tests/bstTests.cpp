#include <iostream>
#include <random>
#include <set>

#include "bst.h"
#include "gtest/gtest.h"

int makeRandomInt(int left, int right)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<std::mt19937::result_type> distribution(left, right);

    return distribution(rng);
}

class bstTests :
    public ::testing::Test
{
public:
    bstTests()
    {
        for (int i = 0; i < N_elements; ++i)
        {
            int num = makeRandomInt(left_bound, right_bound);
            bstSample.insert(num);
            stdSet.insert(num);
        }
    }

    ~bstTests() override
    {}

public:
    const int N_elements = 10000;
    int left_bound = -3000;
    int right_bound = 3000;

    sg::bst<int> bstSample;
    std::set<int> stdSet;
};

// Look for every single and integer added to the structure and
// call exists() against it; as the number has been added to bstSample,
// exists() must return true.
TEST_F(bstTests, bstSearchAddedItems)
{
    for (auto it = stdSet.begin(); it != stdSet.end(); ++it)
    {
        bool test_val = bstSample.exists(*it);
        EXPECT_EQ(test_val, true);
    }
}

// Try to feed random numbers to exists() method and see
// if it returns true only on the elements that have been added
// to the structure.
TEST_F(bstTests, bstSearchRandomItems)
{
    for (int i = 0; i < N_elements; ++i)
    {
        int num = makeRandomInt(left_bound, right_bound);

        bool test_val = bstSample.exists(num);
        bool ref_val = stdSet.find(num) != stdSet.end();
        EXPECT_EQ(test_val, ref_val);
    }
}

// Try to delete random numbers from the structure, and once deleted an element,
// check if it was deleted (and check that other elements haven't been lost,
// for example) by comparing contents of bstSame and stdSet.
// Also check if the deleted items aren't actually found in the structure
// after their deletion.
TEST_F(bstTests, bstRemoveRandomItems)
{
    std::set<int> deletedItems;

    for (int i =  0; i < N_elements; ++i)
    {
        int num = makeRandomInt(left_bound, right_bound);

        bstSample.remove(num);
        stdSet.erase(num);
        deletedItems.insert(num);

        for (auto it_check = stdSet.begin(); it_check != stdSet.end(); ++it_check)
        {
            bool test_val = bstSample.exists(*it_check);
            bool ref_val = stdSet.find(*it_check) != stdSet.end();
            EXPECT_EQ(test_val, ref_val);
        }

        for (auto it_check = deletedItems.begin(); it_check != deletedItems.end(); ++it_check)
        {
            bool test_val = bstSample.exists(*it_check);
            EXPECT_EQ(test_val, false);
        }
    }
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
