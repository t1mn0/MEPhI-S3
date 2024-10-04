#include "../../include/Associative/Map.hpp"



namespace tmn_associative {

// RBTreeNode implementation :
template <class Key, class Value>
Map<Key, Value>::RBTreeNode::RBTreeNode(const tmn::Pair<const Key, Value>& other_pair) noexcept :
pair(other_pair) { }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator implementation :

template <class Key, class Value>
template <bool isConst>
Map<Key, Value>::common_iterator<isConst>& 
Map<Key, Value>::common_iterator<isConst>::operator=(const common_iterator<isConst>& other){
    ptr = other.ptr;
    return *this;
}

template <class Key, class Value>
template <bool isConst>
typename Map<Key, Value>::common_iterator<isConst>::conditional_ref
Map<Key, Value>::common_iterator<isConst>::operator*() const {
    return ptr->pair;
}

template <class Key, class Value>
template <bool isConst>
typename Map<Key, Value>::common_iterator<isConst>::conditional_ptr
Map<Key, Value>::common_iterator<isConst>::operator->() const {
    return &(ptr->pair);
}

template <class Key, class Value>
template <bool isConst>
bool Map<Key, Value>::common_iterator<isConst>::operator==(const common_iterator<isConst>& other) const {
    return ptr == other.ptr;
}

template <class Key, class Value>
template <bool isConst>
bool Map<Key, Value>::common_iterator<isConst>::operator!=(const common_iterator<isConst>& other) const {
    return ptr != other.ptr;
}

template <class Key, class Value>
template <bool isConst>
Map<Key, Value>::common_iterator<isConst>& 
Map<Key, Value>::common_iterator<isConst>::operator++() {
    // TODO :
    // // ptr = ptr->next;
    return *this;
}

template <class Key, class Value>
template <bool isConst>
Map<Key, Value>::common_iterator<isConst> 
Map<Key, Value>::common_iterator<isConst>::operator++(int) {
   common_iterator<isConst> tmp(*this);
    // TODO :
   // // ptr = ptr->next;
   return tmp;
}

template <class Key, class Value>
template <bool isConst>
Map<Key, Value>::common_iterator<isConst>& 
Map<Key, Value>::common_iterator<isConst>::operator--() {
    // TODO :
    // // ptr = ptr->prev;
    return *this;
}

template <class Key, class Value>
template <bool isConst>
Map<Key, Value>::common_iterator<isConst> 
Map<Key, Value>::common_iterator<isConst>::operator--(int) {
   common_iterator<isConst> tmp(*this);
    // TODO :
    // // ptr = ptr->prev;
   return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

// template <class Key, class Value>
// Map<Key, Value>::Map(const Map<Key, Value>& other) {
//     for (const auto& pair: other){
//         insert(pair);
//     }
// }

template <class Key, class Value>
Map<Key, Value>::Map(Map<Key, Value>&& other) : 
    _root(other._root),
    _size(other._size),
    _alloc_node(_alloc_node)
{
    other._root = nullptr;
    other._size = 0;
}

template <class Key, class Value>
Map<Key, Value>::Map(std::initializer_list<tmn::Pair<const Key, Value>> lst) {
    for (const auto& pair : lst) {
        insert(pair);
    }
}

template <class Key, class Value>
void Map<Key, Value>::swap(Map<Key, Value>& other) {
    std::swap(_size, other._size);
    std::swap(_root, other._root);
    std::swap(_alloc_node, other._alloc_node);
}

// template <class Key, class Value>
// Map<Key, Value>& Map<Key, Value>::operator=(const Map<Key, Value>& other) {
//     clear();

//     for (const auto& pair : other) {
//         insert(pair);
//     }

//     return *this;
// }

template <class Key, class Value>
Map<Key, Value>& Map<Key, Value>::operator=(Map<Key, Value>&& other) noexcept {
    clear();

    _root = other._root;
    _size = other._size;
    _alloc_node = other._alloc_node;

    other._root = nullptr;
    other._size = 0;

    return *this;
}

template <class Key, class Value>
Map<Key, Value>::~Map() {
    clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Finders:

template <class Key, class Value>
typename Map<Key, Value>::RBTreeNode* Map<Key, Value>::leftmost(typename Map<Key, Value>::RBTreeNode* root) {
    RBTreeNode* current = root;
    while (current && current->left /*&& !current->left_is_thread*/) {
        current = current->left;
    }
    return current;
}

template <class Key, class Value>
typename Map<Key, Value>::RBTreeNode* Map<Key, Value>::rightmost(typename Map<Key, Value>::RBTreeNode* root) {
    RBTreeNode* current = root;
    while (current && current->right /*&& !current->right_is_thread*/) {
        current = current->left;
    }
    return current;
}

template <class Key, class Value>
typename Map<Key, Value>::RBTreeNode* Map<Key, Value>::find_node(const Key& key) {
    RBTreeNode* current = _root;
    while (current != nullptr) {
        if (key < current->pair.first){
            current = current->left;
        }
        else if (key > current->pair.first){
            current = current->right;
        }
        else {
            return current;
        }
    }
    
    return nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifier <INSERT> (rotate & inner insert) :


template <class Key, class Value>
void Map<Key, Value>::rotate_left(typename Map<Key, Value>::RBTreeNode* pivot_node) {
    RBTreeNode* right_child = pivot_node->right;
    pivot_node->right = right_child->left;

    if (right_child->left) {
        right_child->left->parent = pivot_node;
    }

    right_child->parent = pivot_node->parent;

    if (!pivot_node->parent) {
        _root = right_child;
    }
    else if (pivot_node == pivot_node->parent->left) {
        pivot_node->parent->left = right_child;
    }
    else {
        pivot_node->parent->right = right_child;
    }

    right_child->left = pivot_node;
    pivot_node->parent = right_child;
}

template <class Key, class Value>
void Map<Key, Value>::rotate_right(typename Map<Key, Value>::RBTreeNode* pivot_node) {
    RBTreeNode* left_child = pivot_node->left;
    pivot_node->left = left_child->right;

    if (left_child->right) {
        left_child->right->parent = pivot_node;
    }

    left_child->parent = pivot_node->parent;
    
    if (!pivot_node->parent) {
        _root = left_child;
    }
    else if (pivot_node == pivot_node->parent->right) {
        pivot_node->parent->right = left_child;
    }
    else {
        pivot_node->parent->left = left_child;
    }

    left_child->right = pivot_node;
    pivot_node->parent = left_child;
}

template <class Key, class Value>
void Map<Key, Value>::inner_insert(typename Map<Key, Value>::RBTreeNode* new_node) {
    // Восстановление свойств КЧ дерева для узла с родительским красным узлом
    while (new_node->parent && new_node->parent->is_red) {

        if (new_node->parent == new_node->parent->parent->left) {
            
            RBTreeNode* uncle_node = new_node->parent->parent->right;

            if (uncle_node && uncle_node->is_red) {
                new_node->parent->is_red = false;
                uncle_node->is_red = false;
                new_node->parent->parent->is_red = true;

                new_node = new_node->parent->parent;
            } 
            else {
                if (new_node == new_node->parent->right) {
                    new_node = new_node->parent;
                    rotate_left(new_node);
                }
                new_node->parent->is_red = false;
                new_node->parent->parent->is_red = true;

                rotate_right(new_node->parent->parent);
            }
        } 
        else {

            RBTreeNode* uncle_node = new_node->parent->parent->left;

            if (uncle_node && uncle_node->is_red) {
                new_node->parent->is_red = false;
                uncle_node->is_red = false;
                new_node->parent->parent->is_red = true;

                new_node = new_node->parent->parent;
            } 
            else {
                if (new_node == new_node->parent->left) {
                    new_node = new_node->parent;
                    rotate_right(new_node);
                }
                new_node->parent->is_red = false;
                new_node->parent->parent->is_red = true;

                rotate_left(new_node->parent->parent);
            }
        }
    }
    _root->is_red = false;
}

template <class Key, class Value>
bool Map<Key, Value>::insert(const tmn::Pair<const Key, Value>& pair) {
    
    RBTreeNode* new_node = allocator_traits_node::allocate( _alloc_node, 1);
    allocator_traits_node::construct(_alloc_node, new_node, pair);

    if (_root == nullptr) {
        _root = new_node;
        ++_size;
        return true;
    }

    RBTreeNode* current = _root;
    RBTreeNode* parent_node = nullptr;

    while (current) {
        parent_node = current;
        if (pair.first < current->pair.first) {
            current = current->left;
        }
        else if (pair.first > current->pair.first){
            current = current->right;
        }
        else{
            allocator_traits_node::destroy(_alloc_node, new_node);
            allocator_traits_node::deallocate(_alloc_node, new_node, 1);
            return false;
        }
    }

    new_node->parent = parent_node;
    
    if(pair.first < parent_node->pair.first) {
        parent_node->left = new_node;
    }
    else {
        parent_node->right = new_node;
    }

    new_node->is_red = true;
    inner_insert(new_node);

    ++_size;
    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifier <ERASE> (transplant & inner erase) :

template <class Key, class Value>
void Map<Key, Value>::transplant(RBTreeNode* u, RBTreeNode* v) {
    if (!u->parent) {
        _root = v;
    }
    else if (u == u->parent->left){
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }

    if (v) {
        v->parent = u->parent;
    }
}

template <class Key, class Value>
void Map<Key, Value>::inner_erase(RBTreeNode* node_to_remove) {
    while (node_to_remove != _root && (!node_to_remove->is_red)) {

        if (node_to_remove == node_to_remove->parent->left) {

            RBTreeNode* w = node_to_remove->parent->right;

            if (w->is_red) {
                w->is_red = false;

                node_to_remove->parent->is_red = true;

                rotate_left(node_to_remove->parent);

                w = node_to_remove->parent->right;
            }

            if ((!w->left->is_red) && (!w->right->is_red)) {
                w->is_red = true;
                node_to_remove = node_to_remove->parent;
            } 
            else {
                if (!w->right->is_red) {
                    w->left->is_red = false;
                    w->is_red = true;

                    rotate_right(w);
                    w = node_to_remove->parent->right;
                }

                w->is_red = node_to_remove->parent->is_red;

                node_to_remove->parent->is_red = false;
                w->right->is_red = false;

                rotate_left(node_to_remove->parent);
                node_to_remove = _root;
            }
        } 
        else {
            RBTreeNode* w = node_to_remove->parent->left;
            if (w->is_red) {
                w->is_red = false;
                node_to_remove->parent->is_red = true;

                rotate_right(_root, node_to_remove->parent);
                w = node_to_remove->parent->left;
            }

            if ((!w->right->is_red) && (!w->left->is_red)) {
                w->is_red = true;
                node_to_remove = node_to_remove->parent;
            } 
            else {
                if (!w->left->is_red) {

                    w->right->is_red = false;
                    w->is_red = true;

                    rotate_left(_root, w);
                    w = node_to_remove->parent->left;
                }
                w->is_red = node_to_remove->parent->is_red;

                node_to_remove->parent->is_red = false;
                w->left->is_red = false;

                rotate_right(_root, node_to_remove->parent);
                node_to_remove = _root;
            }
        }
    }
    
    node_to_remove->is_red = false;
}

template <class Key, class Value>
bool Map<Key, Value>::erase(const Key& key) {
    RBTreeNode* node = find_node(key); 

    if (!node){
        return false;
    } 

    RBTreeNode* y = node;

    bool y_original_color = y->is_red;

    RBTreeNode* x;

    if (!node->left) {
        x = node->right;
        transplant(node, node->right);
    } 
    else if (!node->right) {
        x = node->left;
        transplant(node, node->left);
    } 
    else {
        y = leftmost(node->right);
        y_original_color = y->is_red;
        x = y->right;
        if (y->parent == node) {
            x->parent = y;
        } 
        else {
            transplant(y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        transplant(node, y);
        y->left = node->left;
        y->left->parent = y;
        y->is_red = node->is_red;
    }

    allocator_traits_node::destroy(_alloc_node, node);
    allocator_traits_node::deallocate(_alloc_node, node, 1);

    --_size;
    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifier <CLEAR> :

template <class Key, class Value>
void Map<Key, Value>::clear_tree(RBTreeNode* node) {
    if (node) {
        clear_tree(node->left);
        clear_tree(node->right);
        allocator_traits_node::destroy(_alloc_node, node);
        allocator_traits_node::deallocate(_alloc_node, node, 1);
        node = nullptr;
    }
}

template <class Key, class Value>
void Map<Key, Value>::clear() {
    clear_tree(_root);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size & get fields:

template <class Key, class Value>
std::size_t Map<Key, Value>::size() const noexcept {
    return _size;
}

template <class Key, class Value>
bool Map<Key, Value>::empty() const noexcept {
    return false;
}

}