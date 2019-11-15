#ifndef THANADOL_HELPER_ITER_CONSUME_H
#define THANADOL_HELPER_ITER_CONSUME_H

namespace Iter {

template <typename State, typename Iterable>
State reduce(Iterable iterable, State inital_state, State (*reducer)(State, typename Iterable::Output)) {
    for(auto i : iterable) {
        inital_state = reducer(inital_state, i);
    }
    return inital_state;
}


template <typename State, typename Iterable>
State reduce(Iterable iterable, State inital_state, void (*reducer)(State&, typename Iterable::Output)) {
    for(auto i : iterable) {
        reducer(inital_state, i);
    }
    return inital_state;
}

template <typename State, typename Iterable>
State sum(Iterable iterable, State inital_state) {
    for(auto i : iterable) {
        inital_state += i;
    }
    return inital_state;
}


template <typename State, typename Iterable>
State product(Iterable iterable, State inital_state) {
    for(auto i : iterable) {
        inital_state *= i;
    }
    return inital_state;
}


template <typename Iterable>
void for_each(Iterable iterable, void(*fn)(typename Iterable::Output)) {
    for(auto i : iterable) {
        fn(i);
    }
}


}

#endif