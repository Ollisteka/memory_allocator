#include <iostream>
#include "my_allocator.h"

struct test {
    int a;
    double b;
};


int main() {
    my_allocator alloc = my_allocator();
    vector<void *> pointers;
    for (int i = 0; i < 12; ++i) {
        pointers.push_back(alloc.alloc(sizeof(test)));
        alloc.Dump();
    }
    while (!pointers.empty()) {
        auto last = pointers[pointers.size() - 1];
        pointers.pop_back();
        alloc.free(last);
        alloc.Dump();
    }
}