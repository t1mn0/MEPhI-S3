#pragma once

namespace tmn_strategy {
    
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Hash strategies :

template <class Key, class Value>
class SeparateChaining {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = tmn::Pair<const Key, Value>;

    int find_or_insert(/* */);
    bool contains(/* */);
};

template <class Key, class Value>
class OpenAddressing {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = tmn::Pair<const Key, Value>;

    int find_or_insert(/* */);
    bool contains(/* */);
};

template <class Strategy>
class collision_strategy_traits {
public:
    static int find_or_insert() {
        _.find_or_insert();
    }

    static bool contains() {
        _.contains();
    }
};


}