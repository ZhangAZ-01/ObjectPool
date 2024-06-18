// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//

#include "object_pool.h"
#include <assert.h>
#include <iostream>

namespace object_pool {

#define EXPECT_TRUE(cond, msg)                                                 \
  if (!(cond)) {                                                               \
    std::cerr << msg << std::endl;                                             \
    assert(cond);                                                              \
  }

class test_object {
public:
  static int construct_count;
  static int destruct_count;
  int id;

public:
  test_object() : id(construct_count){
    ++construct_count;
  }

  ~test_object() {
    ++destruct_count;
  }
};

int test_object::construct_count = 0;
int test_object::destruct_count = 0;

void test_object_reuse(){
  object_pool<test_object> pool;
  EXPECT_TRUE(pool.init(2), "Initialization failed.");

  auto obj1 = pool.get();
  int id1 = obj1->id;
  obj1.reset();

  auto obj2 = pool.get();
  int id2 = obj2->id;
  EXPECT_TRUE(id1 == id2, "Object ID mismatch.");

  obj2.reset();
}

} // namespace object_pool

int main() {
  object_pool::test_object_reuse();
  return 0;
}
