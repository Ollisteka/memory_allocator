#include <iostream>
#include "MyAllocator.h"

struct littleTest {
    int a;
};

struct test {
    int a;
    double b;
};
struct bigTest {
    int a;
    double b;
    int c;
};
struct middleTest {
    int a;
    double b;
    int c;
};
struct veryBigTest {
    int a;
    double b;
    int c;
    double d;
    int e;
};


int main() {
    MyAllocator alloc = MyAllocator();
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

    last = pointers[4];
    alloc.free(last);
    alloc.Dump();

    pointers.push_back(alloc.alloc(sizeof(veryBigTest)));
    alloc.Dump();
/*
    //вставить большой кусок, объединив два
    pointers.push_back(alloc.alloc(sizeof(middleTest)));
    alloc.Dump();
    //вставить маленький кусочек, объединить остаток
*/
    pointers.push_back(alloc.alloc(sizeof(littleTest)));
    alloc.Dump();


    pointers.push_back(alloc.alloc(sizeof(littleTest)));
    alloc.Dump();
    pointers.push_back(alloc.alloc(sizeof(test)));
    alloc.Dump();
    pointers.push_back(alloc.alloc(sizeof(test)));
    alloc.Dump();

    // ошибка
    /*
    for (int i = 0; i < 5; ++i) {
        pointers.push_back(alloc.alloc(sizeof(test)));
        alloc.Dump();
    }
     */

}