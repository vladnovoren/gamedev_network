#include "TypeID.hpp"
#include <iostream>

class A {};

class B : public A {};

class C {};

class D : public C {};

int main() {
  std::cout << std::is_base_of_v<B, B> << '\n';
  std::cout << TypeID<A>::GetId<A>() << '\n';
  std::cout << TypeID<A>::GetId<B>() << '\n';
  std::cout << TypeID<C>::GetId<C>() << '\n';
  std::cout << TypeID<C>::GetId<D>() << '\n';

  return 0;
}