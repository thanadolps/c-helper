#ifndef THANADOL_HELPER_ITER_ADAPT_H
#define THANADOL_HELPER_ITER_ADAPT_H

#include <stddef.h>

namespace Adaptor {
/**
 * Iterator Adaptor that yield the transformed `inner iterator`'s yield
 * 
 * Note:
 * U: transformed yield type (type that this Map yield)
*/
template <typename U, typename InnerIterable>
class Map {

    typename InnerIterable::Iterator inner_begin;
    typename InnerIterable::Iterator inner_end;
    U(*transformer)(typename InnerIterable::Output);
public:
    typedef U Output;
    struct Iterator {
        typename InnerIterable::Iterator inner_iterator;
        U(*transformer)(typename InnerIterable::Output);

        bool operator!=(Iterator& other) { return this->inner_iterator != other.inner_iterator; }
        void operator++() { ++(this->inner_iterator); }
        U operator*() { return transformer(*this->inner_iterator); }
    };

    Map(InnerIterable& iterable, U(*transformer)(typename InnerIterable::Output)):
        inner_begin(iterable.begin()),
        inner_end(iterable.end()),
        transformer(transformer)
    {
    }
    
    Iterator begin() { return {inner_begin, transformer}; }
    Iterator end() { return {inner_end, transformer}; }
};

template <typename U, typename InnerIterable>
Map<U, InnerIterable> map(InnerIterable& iterable, U(*transformer)(typename InnerIterable::Output)) {
    return Map<U, InnerIterable>(iterable, transformer);
}





template <typename InnerIterable>
class Take {
    typename InnerIterable::Iterator inner_begin;
    typename InnerIterable::Iterator inner_end;
    size_t limit;
public:
    typedef typename InnerIterable::Output Output;
    struct Iterator {
        typename InnerIterable::Iterator inner_iterator;
        size_t index;

        bool operator!=(Iterator& other) { 
            return this->inner_iterator != other.inner_iterator && this->index < other.index; 
        }
        void operator++() { ++(this->inner_iterator); ++(this->index); }
        typename InnerIterable::Output operator*() { return *this->inner_iterator; }
    };

    Take(InnerIterable& iterable, size_t limit):
        inner_begin(iterable.begin()),
        inner_end(iterable.end()),
        limit(limit)
    {}
    
    Iterator begin() { return {inner_begin, 0}; }
    Iterator end() { return {inner_end, limit}; }
};

template <typename InnerIterable>
Take<InnerIterable> take(InnerIterable& iterable, size_t amount) {
    return Take<InnerIterable>(iterable, amount);
}





template <typename InnerIterable>
class Filter {
    typename InnerIterable::Iterator inner_begin;
    typename InnerIterable::Iterator inner_end;
    bool (*predicate)(typename InnerIterable::Output&);
public:
    typedef typename InnerIterable::Output Output;
    struct Iterator {
        typename InnerIterable::Iterator inner_iterator;
        bool (*predicate)(typename InnerIterable::Output&);
        
        bool operator!=(Iterator& other) {
            while(true)
            {
                // halt if inner iterator already halt
                if(!(this->inner_iterator != other.inner_iterator)) {
                    return false;
                }

                auto val = (this->inner_iterator).operator*();
                if (predicate(val)) {
                    // predicate statify, wait to yield value
                    return true;
                }
                else {
                    // predicate fail, iterate inner iterator and continue the cycle
                    ++(this->inner_iterator);
                    continue;
                }
            }
        }
        void operator++() {
            // very useless with != doing most of the increment
            ++(this->inner_iterator);
        }
        typename InnerIterable::Output operator*() { return *this->inner_iterator; }
    };

    Filter(InnerIterable& iterable, bool (*predicate)(typename InnerIterable::Output&)):
        inner_begin(iterable.begin()),
        inner_end(iterable.end()),
        predicate(predicate)
    {}
    
    Iterator begin() { return {inner_begin, predicate}; }
    Iterator end() { return {inner_end, predicate}; }
};

template <typename InnerIterable>
Filter<InnerIterable> filter(InnerIterable& iterable, bool (*predicate)(typename InnerIterable::Output&)) {
    return Filter<InnerIterable>(iterable, predicate);
}

}
#endif