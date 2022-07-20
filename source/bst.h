#pragma once

#include <memory>

namespace sg
{
    template <typename Tvalue> class bst_node;
    template <typename Tvalue, typename Tnode = sg::bst_node<Tvalue>> class bst;
}

namespace sg
{
    template <typename Tvalue>
    class bst_node
    {
    private:
        using pnode_t = std::shared_ptr<sg::bst_node<Tvalue>>;
        friend class sg::bst<Tvalue, sg::bst_node<Tvalue>>;

    public:
        bst_node() = delete;

        bst_node(const Tvalue& __value, pnode_t __parent = nullptr) :
            value {__value},
            parent {__parent}
        {}

        bst_node(Tvalue&& __value, pnode_t __parent = nullptr) :
            value {std::move(__value)},
            parent {__parent}
        {}

        enum class side { left, right, null };

    protected:
        Tvalue value;
        pnode_t parent;
        pnode_t lchild = nullptr;
        pnode_t rchild = nullptr;

        static sg::bst_node<Tvalue>::side detach(pnode_t& __child, pnode_t& __parent);
        static void attach_left(pnode_t& __child, pnode_t& __parent);
        static void attach_right(pnode_t& __child, pnode_t& __parent);
        static void attach(pnode_t& __child, pnode_t& __parent, sg::bst_node<Tvalue>::side __side);
    };
}

namespace sg
{
    template <typename Tvalue, typename Tnode>
    class bst
    {
    private:
        using node_t = Tnode;
        using pnode_t = std::shared_ptr<Tnode>;

    public:
        virtual bool exists(const Tvalue& __value) const;
        virtual void insert(Tvalue __value);
        virtual void remove(const Tvalue& __value);

        template <typename... Args>
        void emplace(Args&&... args)
        {
            Tvalue __value {std::forward<Args>(args)...};
            insert(std::move(__value));
        }

        std::size_t size() const { return tree_size; }

    protected:
        pnode_t root = nullptr;
        std::size_t tree_size = 0; // Number of nodes

    protected:
        pnode_t find(const Tvalue& __value) const;
        pnode_t successor(pnode_t __pnode) const;
        pnode_t predecessor(pnode_t __pnode) const;

        // Gives the minimal node from the right subtree
        pnode_t min_rsubtree(pnode_t __pnode) const;
        // Gives the maximal node from the left subtree
        pnode_t max_lsubtree(pnode_t __pnode) const;
    };
}

template <typename Tvalue>
inline typename sg::bst_node<Tvalue>::side
sg::bst_node<Tvalue>::detach(pnode_t& __child, pnode_t& __parent)
{
    // If __child is actually the root node or it's invalid, nothing to detach
    if (!__parent || !__child) { return sg::bst_node<Tvalue>::side::null; }

    if (__parent->lchild == __child)
    {
        __parent->lchild = nullptr;
        __child->parent = nullptr;
        return sg::bst_node<Tvalue>::side::left;
    }
    else if (__parent->rchild == __child)
    {
        __parent->rchild = nullptr;
        __child->parent = nullptr;
        return sg::bst_node<Tvalue>::side::right;
    }
    // If they aren't connected, nothing to detach
    return sg::bst_node<Tvalue>::side::null;
}

template <typename Tvalue>
inline void
sg::bst_node<Tvalue>::attach_left(pnode_t& __child, pnode_t& __parent)
{
    // Pointers must be valid
    if (!__parent || !__child) { return; }
    // They must be detached
    if (__parent->lchild ||  __child->parent) { return; /* Better to raise some exception*/ }

    __parent->lchild = __child;
    __child->parent = __parent;
}

template <typename Tvalue>
inline void
sg::bst_node<Tvalue>::attach_right(pnode_t& __child, pnode_t& __parent)
{
    // Pointers must be valid
    if (!__parent || !__child) { return; }
    // They must be detached
    if (__parent->rchild ||  __child->parent) { return; /* Better to raise some exception*/ }

    __parent->rchild = __child;
    __child->parent = __parent;
}

template <typename Tvalue>
inline void
sg::bst_node<Tvalue>::attach(pnode_t& __child, pnode_t& __parent, sg::bst_node<Tvalue>::side __side)
{
    switch (__side)
    {
        case (sg::bst_node<Tvalue>::side::left):
            sg::bst_node<Tvalue>::attach_left(__child, __parent);
            break;
        case (sg::bst_node<Tvalue>::side::right):
            sg::bst_node<Tvalue>::attach_right(__child, __parent);
            break;
    }
}

template <typename Tvalue, typename Tnode>
bool
sg::bst<Tvalue, Tnode>::exists(const Tvalue& __value) const
{
    return find(__value) != nullptr;
}

template <typename Tvalue, typename Tnode>
void
sg::bst<Tvalue, Tnode>::insert(Tvalue __value)
{
    if (!root)
    {
        // If the tree is empty, put __value to the root node
        root = std::make_shared<node_t>(std::move(__value));
        return;
    }
    // If the tree isn't empty, make normal search to see the place
    // where the new node should reside
    pnode_t previous = nullptr;
    pnode_t current = root;
    while (current)
    {
        if (__value == current->value)
        {
            // If the object already exists, do nothing
            return;
        }

        if (__value < current->value)
        {
            previous = current;
            current = current->lchild;
        }
        else
        {
            previous = current;
            current = current->rchild;
        }
    }
    // Now, when current is NIL, previous is a parent for the new value (and one
    // can think of that found NIL as a position where to store then new value __value)
    if (__value < previous->value)
    {
        previous->lchild = std::make_shared<node_t>(std::move(__value), previous);
    }
    else
    {
        previous->rchild = std::make_shared<node_t>(std::move(__value), previous);
    }
    tree_size++;
}

template <typename Tvalue, typename Tnode>
typename sg::bst<Tvalue, Tnode>::pnode_t
sg::bst<Tvalue, Tnode>::find(const Tvalue& __value) const
{
    pnode_t current = root;
    while (current)
    {
        if (__value == current->value)
        {
            return current;
        }

        if (__value < current->value)
        {
            current = current->lchild;
        }
        else
        {
            current = current->rchild;
        }
    }
    return nullptr;
}

template <typename Tvalue, typename Tnode>
typename sg::bst<Tvalue, Tnode>::pnode_t
sg::bst<Tvalue, Tnode>::successor(pnode_t __pnode) const
{
    if (__pnode->rchild) { return min_rsubtree(__pnode); }
    // If there's no right subtree, the successor of pnode may be found upwards
    // where the tree goes left (then the parent of that left child is the successor)
    pnode_t current = __pnode;
    pnode_t parent = __pnode->parent;
    while (parent)
    {
        if (current == parent->rchild)
        {
            // Still move upwards and wait until we see the place
            // where the tree branches left
            current = parent;
            parent = current->parent;
        }
        else
        {
            return parent;
        }
    }
    // When parent == nullptr, we reached the root node going through right edges from a leaf,
    // so there's no element greater than __pnode and there's no successor of it.
    return nullptr;
}

template <typename Tvalue, typename Tnode>
typename sg::bst<Tvalue, Tnode>::pnode_t
sg::bst<Tvalue, Tnode>::predecessor(pnode_t __pnode) const
{
    if (__pnode->lchild) { return max_lsubtree(__pnode); }
    // Similarly to finding successor, if there's no left subtree, predecessor can be found
    // above where the tree goes right; parent of that right child is our predecessor.
    pnode_t current = __pnode;
    pnode_t parent = __pnode->parent;
    while (parent)
    {
        if (current == parent->lchild)
        {
            // Still need to move upwards
            current = parent;
            parent = current->parent;
        }
        else
        {
            return parent;
        }
    }
    // If we reached the root here, all that time we were going by left edges from a leaf,
    // so that means __pnode was the minimal element in the tree (and there's no predecessor)
    return nullptr;
}

template <typename Tvalue, typename Tnode>
typename sg::bst<Tvalue, Tnode>::pnode_t
sg::bst<Tvalue, Tnode>::min_rsubtree(pnode_t __pnode) const
{
    pnode_t previous = __pnode;
    pnode_t current = __pnode->rchild;
    if (!current)
    {
        // If there's no right subtree of __pnode
        return nullptr;
    }
    while (current)
    {
        previous = current;
        current = previous->lchild;
    }
    return previous;
}

template <typename Tvalue, typename Tnode>
typename sg::bst<Tvalue, Tnode>::pnode_t
sg::bst<Tvalue, Tnode>::max_lsubtree(pnode_t __pnode) const
{
    pnode_t previous = __pnode;
    pnode_t current = __pnode->lchild;
    if (!current)
    {
        // If there's no left subtree of __pnode
        return nullptr;
    }
    while (current)
    {
        previous = current;
        current = previous->rchild;
    }
    return previous;
}

template <typename Tvalue, typename Tnode>
void
sg::bst<Tvalue, Tnode>::remove(const Tvalue& __value)
{
    pnode_t pnode = find(__value);
    if (!pnode)
    {
        // If there's no such element, do nothing
        return;
    }
    pnode_t parent = pnode->parent;
    pnode_t left = pnode->lchild;
    pnode_t right = pnode->rchild;

    if (!left && !right) // 1. If pnode is a leaf, just remove it
    {
        if (parent) { node_t::detach(pnode, parent); }
        else { root = nullptr; } // parent == nullptr means that pnode was root
    }
    else if (left && !right) // 2. If pnode has only left child, replace pnode by its left child in the tree
    {
        node_t::detach(left, pnode);
        typename node_t::side side = node_t::detach(pnode, parent);
        if (parent) { node_t::attach(left, parent, side); }
        else { root = left; } // parent == nullptr means that pnode was root
    }
    else if (!left && right) // 3. If pnode has only right child, replace pnode by its right child in the tree
    {
        node_t::detach(right, pnode);
        typename node_t::side side = node_t::detach(pnode, parent);
        if (parent) { node_t::attach(right, parent, side); }
        else { root = right; } // parent == nullptr means that pnode was root
    }
    else // 4. Last case is when pnode has both left and right children;
    {
        // First find the successor of pnode;
        // As pnode has right subtree (right != nullptr), its successor is somewhere below.
        pnode_t s = successor(pnode);
        // Just quick note: s doesn't have a left child (always!)

        // If s is pnode's immediate child
        if (s == right)
        {
            node_t::detach(left, pnode);
            node_t::attach_left(left, right);
            node_t::detach(right, pnode);

            typename node_t::side side = node_t::detach(pnode, parent);
            if (parent) { node_t::attach(right, parent, side); }
            else { root = right; } // parent == nullptr means that pnode was root
        }
        // If it's not an immediate child, we will still substitute pnode with its successor, but
        // we will need to re-link right child of the successor to the right child of pnode
        // (if the former exists)
        // Another quick note: successor is a left child to some other node (always)
        else
        {
            pnode_t s_rchild = s->rchild;
            pnode_t s_parent = s->parent;

            node_t::detach(s_rchild, s);             // Does nothing if s_rchild is nullptr
            node_t::detach(s, s_parent);
            node_t::attach_left(s_rchild, s_parent); // Does nothing if s_rchild is nullptr

            node_t::detach(left, pnode);
            node_t::detach(right, pnode);
            node_t::attach_left(left, s);
            node_t::attach_right(right, s);

            typename node_t::side side = node_t::detach(pnode, parent);
            if (parent) { node_t::attach(s, parent, side); }
            else { root = s; } // parent == nullptr means that pnode was root
        }
    }
    tree_size--;
}
