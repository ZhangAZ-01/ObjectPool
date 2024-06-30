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

 public:
  test_object() { ++construct_count; }
  ~test_object() { ++destruct_count; }
};

int test_object::construct_count = 0;
int test_object::destruct_count = 0;

void test_object_reuse() {
  object_pool<test_object> pool;
  EXPECT_TRUE(pool.init(3), "Initialization failed.");

  auto obj1 = pool.get();
  auto obj2 = pool.get();
  auto obj3 = pool.get();

  EXPECT_TRUE(test_object::construct_count == 3, "Construct count is not 3.");

  obj1.reset();

  auto obj4 = pool.get();
  EXPECT_TRUE(test_object::construct_count == 3, "Construct count is not still 3.");

  obj2.reset();
  obj3.reset();
  obj4.reset();

  EXPECT_TRUE(test_object::destruct_count == 0, "Destruct count is not 0.");

  EXPECT_TRUE(pool.init(3), "Initialization failed.");

  EXPECT_TRUE(test_object::destruct_count == 3, "Destruct count after re-initialization error.");
  EXPECT_TRUE(test_object::construct_count == 6, "Construct count after re-initialization error.");
}

}  // namespace object_pool

int main() {
  object_pool::test_object_reuse();
  return 0;
}
