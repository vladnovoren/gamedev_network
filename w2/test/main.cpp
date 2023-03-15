#include <iostream>
#include <utility>

struct A {
    int data[4] = {};
};

int main() {
    A a;
    for (int i = 0; i < 4; ++i)
        a.data[i] = i;
    A b = std::move(a);
    printf("data[1]: %d\n", b.data[1]);
    return 0;
};
