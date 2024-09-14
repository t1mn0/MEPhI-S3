#pragma once

namespace tmn_iterator {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

struct forward_iterator_tag;

struct bidirectional_iterator_tag;

struct random_access_iterator_tag;

struct contiguous_iterator_tag;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator>
struct iterator_traits;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator, typename IteratorCategory>
void advance_helper(Iterator& iter, int n, IteratorCategory);

template <typename Iterator>
void advance_helper(Iterator& iter, int n, random_access_iterator_tag);

template <typename Iterator>
void advance(Iterator& iter, int n);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator>
typename iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

template <typename Iterator>
Iterator next(Iterator it);

template <typename Iterator>
Iterator prev(Iterator it);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename Iterator>
class reverse_iterator;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}

#include "../../src/Iterator/Iterator.tpp"