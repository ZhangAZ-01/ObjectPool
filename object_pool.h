// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//

#pragma once

#include <functional>
#include <memory>
#include <stddef.h>
#include <vector>


namespace object_pool {

template <typename T> class object_pool {
public:
  using value_type = T;
  using pointer = T *;
  using creator_type = std::function<pointer()>;
  using deleter_type = std::function<void(pointer)>;
  using smarter_pointer = std::unique_ptr<value_type, deleter_type>;
  using shared_pointer = std::shared_ptr<value_type>;

public:
  object_pool() = default;
  ~object_pool() { release_all(); }

private:
  object_pool(const object_pool &) = delete;
  object_pool(object_pool &&) = delete;
  object_pool &operator=(const object_pool &) = delete;
  object_pool &operator=(object_pool &&) = delete;

public:
  [[nodiscard]] bool init(size_t max_size);
  [[nodiscard]] bool init(size_t max_size, const creator_type &creator,
                          const deleter_type &deleter = nullptr);
  [[nodiscard]] smarter_pointer get();
  [[nodiscard]] shared_pointer get_shared();

private:
  void release(pointer ptr);
  void release_all();

private:
  size_t max_size_ = 0;
  creator_type creator_;
  deleter_type deleter_;
  std::vector<pointer> pool_;
};

template <typename T> bool object_pool<T>::init(size_t max_size) {
  return init(max_size, []() { return new value_type(); });
}

template <typename T>
bool object_pool<T>::init(size_t max_size, const creator_type &creator,
                          const deleter_type &deleter) {
  release_all();
  max_size_ = max_size;
  creator_ = creator;
  deleter_ = deleter ? deleter : [](pointer ptr) { delete ptr; };
  pool_.reserve(max_size_);
  for (size_t i = 0; i < max_size_; ++i) {
    pool_.emplace_back(creator_());
  }
  return true;
}

template <typename T> auto object_pool<T>::get() -> smarter_pointer {
  if (!pool_.empty()) {
    pointer ptr = pool_.back();
    pool_.pop_back();
    return smarter_pointer(ptr, [this](pointer ptr) { release(ptr); });
  } else {
    return smarter_pointer(creator_(), [this](pointer ptr) { release(ptr); });
  }
}

template <typename T> auto object_pool<T>::get_shared() -> shared_pointer {
  if (!pool_.empty()) {
    pointer ptr = pool_.back();
    pool_.pop_back();
    return shared_pointer(ptr, [this](pointer ptr) { release(ptr); });
  } else {
    return shared_pointer(creator_(), [this](pointer ptr) { release(ptr); });
  }
}

template <typename T> void object_pool<T>::release(pointer ptr) {
  if (pool_.size() < max_size_) {
    pool_.emplace_back(ptr);
  } else {
    deleter_(ptr);
  }
}

template <typename T> void object_pool<T>::release_all() {
  for (pointer ptr : pool_) {
    deleter_(ptr);
  }
  pool_.clear();
}

} // namespace object_pool
