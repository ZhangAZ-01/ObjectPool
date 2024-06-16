// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//

#include "object_pool.h"
#include <assert.h>
#include <iostream>

namespace object_pool {
#define MY_ASSERT(cond, msg)                                            \
  if (!(cond)) {                                                        \
    std::cerr << msg << std::endl;                                      \
    assert(cond);                                                       \
  }

class test_object {
public:
  test_object() {}
  ~test_object() {}
};

void test_default_creation() {
  object_pool<test_object> pool;
  MY_ASSERT(pool.init(5), "Init failed.");

  auto obj1 = pool.get();
  MY_ASSERT(obj1 != nullptr, "Get failed.");
  auto obj2 = pool.get_shared();
  MY_ASSERT(obj2 != nullptr, "Get_shared failed.");

  pool.release(obj1.release());
  pool.release(obj2.get());

  MY_ASSERT(pool.get() != nullptr, "Get after release failed.");
  MY_ASSERT(pool.get_shared() != nullptr, "Get_shared after release failed.");
}
void test_custom_creation() {
  object_pool<test_object> pool;
  auto creator = []() { return new test_object(); };
  MY_ASSERT(pool.init(5, creator), "Custom init failed.");

  auto obj1 = pool.get();
  MY_ASSERT(obj1 != nullptr, "Custom get failed.");
  auto obj2 = pool.get_shared();
  MY_ASSERT(obj2 != nullptr, "Custom get_shared failed.");

  pool.release(obj1.release());
  pool.release(obj2.get());

  MY_ASSERT(pool.get() != nullptr, "Custom get after release failed.");
  MY_ASSERT(pool.get_shared() != nullptr,
            "Custom get_shared after release failed.");
}

void test_boundary_conditions() {
  object_pool<test_object> pool;
  MY_ASSERT(!pool.init(0), "Init size 0 failed.");

  auto creator = []() { return new test_object(); };
  MY_ASSERT(!pool.init(0, creator), "Custom init size 0 failed");
}

void test_max_size() {
  object_pool<test_object> pool;
  auto creator = []() { return new test_object(); };
  MY_ASSERT(pool.init(3, creator), "Init failed");

  std::vector<object_pool<test_object>::smarter_pointer> objects;
  for (size_t i = 0; i < 5; ++i) {
    auto obj = pool.get();
    MY_ASSERT(obj != nullptr, "Get exceeded max_size failed");
    objects.push_back(std::move(obj));
  }

  for (auto& obj : objects) {
    pool.release(obj.release());
  }
}
} // namespace object_pool

int main() {
  object_pool::test_default_creation();
  object_pool::test_custom_creation();
  object_pool::test_boundary_conditions();
  object_pool::test_max_size();
  return 0;
}
