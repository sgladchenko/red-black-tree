#pragma once

#include "bst.h"

namespace sg
{
    template <typename Tvalue>
    class rbt_node
    {
        using pnode_t = std::shared_ptr<sg::rbt_node<Tvalue>>;
        friend class sg::rbt<Tvalue>;

    public:
        enum class color_t { red, black };

        rbt_node() = delete;

        rbt_node(const Tvalue& __value, color_t __color, pnode_t __parent = nullptr) :
            value {__value},
            parent {__parent}
        {}

        rbt_node(Tvalue&& __value, color_t __color, pnode_t __parent = nullptr) :
            value {std::move(__value)},
            parent {__parent}
        {}

        bool is_leaf() const
        {
            // Here is_leaf means() = true means that all its children are NIL.
            return (!lchild && !rchild);
        }

    private:
        Tvalue value;
        pnode_t parent;
        pnode_t lchild = nullptr;
        pnode_t rchild = nullptr;
        color_t color;
    };

    template <typename Tvalue>
    class rbt : public sg::bst<Tvalue, sg::rbt_node<Tvalue>>
    {
        using node_t = sg::rbt_node<Tvalue>;
        using pnode_t = std::shared_ptr<node_t>;
    };
}
