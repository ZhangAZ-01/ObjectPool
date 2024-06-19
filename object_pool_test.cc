// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//

#include "object_pool.h"
#include <assert.h>
#include <iostream>

namespace object_pool {

#define EXPECT_TRUE(cond, msg)     \
  if (!(cond)) {                   \
    std::cerr << msg << std::endl; \
    assert(cond);                  \
  }

class test_object {
 public:
  static int construct_count;
  static int destruct_count;

 private:
  int id_;

 public:
  test_object() { ++construct_count; }
  ~test_object() { ++destruct_count; }

 public:
  int get_id() const { return id_; }
};

int test_object::construct_count = 0;
int test_object::destruct_count = 0;

void test_object_reuse() {
  object_pool<test_object> pool;
  EXPECT_TRUE(pool.init(2), "Initialization failed.");
  int id1 = 0;
  int id2 = 0;

  {
    auto ptr1 = pool.get();
    id1 = ptr1->get_id();
    ptr1.reset();
  }
  {
    auto ptr2 = pool.get();
    id2 = ptr2->get_id();
    ptr2.reset();
    EXPECT_TRUE(id1 == id2, "Dismatch");
  }
}

void test_init_multiple_times() {
  object_pool<test_object> pool;
  size_t sizes[] = {2, 4, 8};
  for (size_t i = 0; i < 3; ++i) {
    EXPECT_TRUE(pool.init(sizes[i]), "Init failed.");
    for (size_t j = 0; j < sizes[i]; ++j) {
      auto ptr = pool.get();
      EXPECT_TRUE(ptr != nullptr,
                  "Get failed: size " + std::to_string(sizes[i]) + ", attempt " + std::to_string(j + 1));
      ptr.reset();
    }
  }
}

}  // namespace object_pool

int main() {
  object_pool::test_object_reuse();
  object_pool::test_init_multiple_times();
  return 0;
}
