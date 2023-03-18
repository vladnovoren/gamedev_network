#pragma once

#include <type_traits>

template<typename BaseT>
class TypeID {
 public:
  template <typename T>
  static int GetID() {
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
