// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//

#pragma once

#include <stdio.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace objectpool {

const int MAX_OBJECT_NUM = 10;

template <typename T> class object_pool {
public:
  template <typename... Args>
  using Constructor = std::function<std::shared_ptr<T>(Args...)>;

private:
  std::multimap<std::string, std::shared_ptr<T>> object_map_;
  bool need_clear_ = false;

public:
  object_pool(void) : need_clear_(false) {}
  virtual ~object_pool() { need_clear_ = true; }

public:
  template <typename... Args> void init(size_t num, Args &&...args) {
    if (num <= 0 || num > MAX_OBJECT_NUM) {
      printf("Object out of range.");
      return;
    }
    std::string construct_name = typeid(Constructor<Args...>).name();
    for (size_t i = 0; i < num; ++i) {
      object_map_.emplace(
          construct_name,
          std::shared_ptr<T>(
              new T(args...), [construct_name, this](T *t) {
                if (need_clear_) {
                  delete t;
                } else {
                  object_map_.emplace(construct_name, std::shared_ptr<T>(t));
                }
              }));
    }
  }
  template <typename... Args> std::shared_ptr<T> get() {
    std::string construct_name_ = typeid(Constructor<Args...>).name();
    auto range = object_map_.equal_range(construct_name_);
    for (auto it = range.first; it != range.second; ++it) {
      auto ptr = it->second;
      object_map_.erase(it);
      return ptr;
    }
    return nullptr;
  }
};
} // namespace objectpool