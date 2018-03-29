#include <iostream>
#include "my_allocator.h"

struct test {
    int a;
    double b;
};
struct bigTest {
    int a;
    double b;
    int c;
};
struct veryBigTest {
    int a;
    double b;
    int c;
    double d;
};


int main() {
    my_allocator alloc = my_allocator();
    vector<void *> pointers;
    for (int i = 0; i < 5; ++i) {
        pointers.push_back(alloc.alloc(sizeof(test)));
        alloc.Dump();
    }

    auto last = pointers[1];
        alloc.free(last);
        alloc.Dump();

    last = pointers[2];
    alloc.free(last);
    alloc.Dump();

    pointers.push_back(alloc.alloc(sizeof(veryBigTest)));
    alloc.Dump();

}