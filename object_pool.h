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
};
} // namespace object_pool
