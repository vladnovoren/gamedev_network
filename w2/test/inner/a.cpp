#include "a.hpp"
#include "inner.hpp"

void A::F() {
    Inner inner;
    inner.x = 2;
    std::cout << "F\n";
}
