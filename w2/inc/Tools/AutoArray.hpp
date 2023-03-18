#pragma once

#include <vector>
#include <cstddef>

template<typename T>
class AutoArray {
 public:
  [[nodiscard]] T& operator[](size_t id) {
    if (id >= data_.size())
      Expand(id);
    return data_[id];
  }

 private:
  void Expand(size_t id) {
    size_t new_size = 2 * data_.size() <= id ? id + 1 : 2 * data_.size();
    data_.resize(new_size);
  }

  std::vector<T> data_;
};