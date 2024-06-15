// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//
#pragma once

#include <functional>
#include <memory>
#include <stddef.h>

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

private:
  object_pool(const object_pool &) = delete;
  object_pool(object_pool &&) = delete;
  object_pool &operator=(const object_pool &) = delete;
  object_pool &operator=(object_pool &&) = delete;

public:
  bool init(size_t max_size);
  bool init(size_t max_size, const creator_type &creator);
  smarter_pointer get();
  shared_pointer get_shared();
  void release(pointer ptr);

private:
  size_t max_size_;
  std::vector<pointer> pool_;
  creator_type creator_;
  deleter_type deleter_;
};
template <typename T> bool object_pool<T>::init(size_t max_size) {
  if (max_size == 0)
    return false;
  max_size_ = max_size;
  creator_ = []() { return new value_type(); };
  deleter_ = [this](pointer ptr) { this->release(ptr); };
  for (size_t i = 0; i < max_size_; ++i) {
    pool_.push_back(creator_());
  }
  return true;
}
template <typename T>
bool object_pool<T>::init(size_t max_size, const creator_type &creator) {
  if (max_size == 0 || creator == nullptr)
    return false;
  max_size_ = max_size;
  creator_ = creator;
  deleter_ = [this](pointer ptr) { this->release(ptr); };
  for (size_t i = 0; i < max_size_; ++i) {
    pool_.push_back(creator_());
  }
  return true;
}
template <typename T> object_pool<T>::smarter_pointer object_pool<T>::get() {
  if (!pool_.empty()) {
    pointer obj = pool_.back();
    pool_.pop_back();
    return smarter_pointer(obj, deleter_);
  } else {
    return smarter_pointer(creator_(), deleter_);
  }
}
template <typename T>
object_pool<T>::shared_pointer object_pool<T>::get_shared() {
  if (!pool_.empty()) {
    pointer obj = pool_.back();
    pool_.pop_back();
    return shared_pointer(obj, deleter_);
  } else {
    return shared_pointer(creator_(), deleter_);
  }
}
template <typename T> void object_pool<T>::release(pointer ptr) {
  if (pool_.size() < max_size_) {
    pool_.push_back(ptr);
  } else {
    delete ptr;
  }
}
} // namespace object_pool
