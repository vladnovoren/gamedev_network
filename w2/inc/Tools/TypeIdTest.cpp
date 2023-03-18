#pragma once

#include <type_traits>

template<typename BaseT>
class TypeID {
 public:
  template<typename T>
  static int GetId() {
    static_assert(std::is_base_of_v<BaseT, T>);
    static bool is_registered = false;
    static int id = 0;
    if (!is_registered) {
      id = last_id_++;
      is_registered = true;
    }
    return id;
  }

 private:
  static int last_id_;
};

template<typename BaseT>
int TypeID<BaseT>::last_id_ = 0;


class BaseA {};

class DerivedA : public BaseA {};

class BaseB {};

class DerivedB : public BaseB {};

int main() {
  std::cout << std::is_base_of_v<B, B> << '\n';
  std::cout << TypeID<A>::GetId<A>() << '\n';
  std::cout << TypeID<A>::GetId<B>() << '\n';
  std::cout << TypeID<C>::GetId<C>() << '\n';
  std::cout << TypeID<C>::GetId<D>() << '\n';

  return 0;
}
