#pragma once

#include "bst.h"

namespace sg
{
    template <typename Tvalue>
    class rbt_node : public sg::bst_node<Tvalue>
    {};

    template <typename Tvalue>
    class rbt : public sg::bst<Tvalue, sg::rbt_node<Tvalue>>
    {};
}