#include "../../include/Iterator/Iterator.hpp"

namespace tmn {
namespace iterator {    

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

struct forward_iterator_tag {};

struct bidirectional_iterator_tag : forward_iterator_tag {};

struct random_access_iterator_tag : bidirectional_iterator_tag {};

struct contiguous_iterator_tag : random_access_iterator_tag {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator>
struct iterator_traits {
    using iterator_category	= typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using difference_type = typename Iterator::difference_type;
    using pointer = typename Iterator::pointer;
    using const_pointer = typename Iterator::const_pointer;
    using reference = typename Iterator::reference;
    using const_reference = typename Iterator::const_reference;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator, typename IteratorCategory>
void advance_helper(Iterator& iter, int n, IteratorCategory){
    for (std::size_t i = 0; i < n; ++i){
        ++iter;
    }
}

template <typename Iterator>
void advance_helper(Iterator& iter, int n, random_access_iterator_tag){
    iter += n;
}

template <typename Iterator>
void advance(Iterator& iter, int n){
    advance_helper(iter, n, typename iterator_traits<Iterator>::iterator_category());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator>
typename iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last){
    if constexpr (std::is_same_v<typename iterator_traits<Iterator>::iterator_category, random_access_iterator_tag>) {
        return last - first;
    }
    
    typename iterator_traits<Iterator>::difference_type i = 0; 
    for (; first != last; ++i){
        ++first;
    }
    
    return i;   
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator>
Iterator next(Iterator it){
    advance(it, 1);
}

template <typename Iterator>
Iterator prev(Iterator it){
    advance(it, -1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename Iterator>
class reverse_iterator {
private:
    Iterator iter;

public:
    explicit reverse_iterator(const Iterator& _iter) { 
        Iterator tmp(_iter);
        --tmp;
        iter = tmp;
    }
    
    reverse_iterator(const reverse_iterator& other) : iter(other.iter) {}

    reverse_iterator<Iterator>& operator=(const reverse_iterator& other) {
        iter = other.iter;
        return *this;
    }

    reverse_iterator<Iterator>& operator++() {
        --iter;
        return *this;
    }

    reverse_iterator<Iterator> operator++(int) {
        reverse_iterator tmp(*this);
        --iter;
        return tmp;
    }

    reverse_iterator<Iterator>& operator--() {
        ++iter;
        return *this;
    }

    reverse_iterator<Iterator> operator--(int) {
        reverse_iterator tmp(*this);
        ++iter;
        return tmp;
    }

    bool operator==(const reverse_iterator<Iterator>& other) const {
        return iter == other.iter;
    }

    bool operator!=(const reverse_iterator<Iterator>& other) const {
        return iter != other.iter;
    }

    Iterator base() const {
        return iter;
    }

    typename iterator_traits<Iterator>::reference operator*() const {
        return *iter;
    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}
}