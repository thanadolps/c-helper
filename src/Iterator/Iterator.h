#ifndef THANADOL_HELPER_ITER_RANGE_H
#define THANADOL_HELPER_ITER_RANGE_H

class Range {
private:
    int start, stop, step;
public:
    typedef int Output;
    struct Iterator {
        int value, step;
        bool operator!=(Iterator& other) {return other.value > this->value;}
        void operator++() {this->value += this->step;}
        int operator*() {return value;}
    };

    Range(int start, int stop, int step=1):
        start(start),
        stop(stop),
        step(step) 
    {}

    Iterator begin() { return {start, step}; }
    Iterator end() { return {stop, step}; }
};

#endif