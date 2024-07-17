// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//

#include "object_pool.h"
#include <assert.h>
#include <iostream>
#include "gtest/gtest.h"

namespace object_pool {

class test_object {
 public:
  static size_t construct_count;
  static size_t destruct_count;

 public:
  test_object() { ++construct_count; }
  ~test_object() { ++destruct_count; }
};

size_t test_object::construct_count = 0;
size_t test_object::destruct_count = 0;

class object_pool_test : public ::testing::Test {
 protected:
  object_pool<test_object> pool;

 protected:
  void SetUp() override {
    test_object::construct_count = 0;
    test_object::destruct_count = 0;
    pool.init(3);
  }
};

TEST_F(object_pool_test, test_get) {
  EXPECT_EQ(test_object::construct_count, 3u);

  auto ptr1 = pool.get();
  auto ptr2 = pool.get();
  auto ptr3 = pool.get();

  EXPECT_EQ(test_object::construct_count, 3u);

  ptr1.reset();
  auto ptr4 = pool.get();

  EXPECT_EQ(test_object::construct_count, 3u);

  ptr2.reset();
  ptr3.reset();
  ptr4.reset();

  EXPECT_EQ(test_object::destruct_count, 0u);

  pool.init(3);

  EXPECT_EQ(test_object::destruct_count, 3u);
  EXPECT_EQ(test_object::construct_count, 6u);
}

TEST_F(object_pool_test, test_get_shared) {
  EXPECT_EQ(test_object::construct_count, 3u);

  auto shared_ptr1 = pool.get_shared();
  auto shared_ptr2 = pool.get_shared();
  auto shared_ptr3 = pool.get_shared();

  EXPECT_EQ(test_object::construct_count, 3u);

  {
    auto shared_ptr4 = pool.get_shared();
    auto shared_ptr5 = pool.get_shared();
    EXPECT_EQ(test_object::construct_count, 5u);
  }

  EXPECT_EQ(static_cast<size_t>(pool.size()), 2u);

  shared_ptr1.reset();
  EXPECT_EQ(static_cast<size_t>(pool.size()), 3u);

  auto shared_ptr6 = pool.get_shared();
  EXPECT_EQ(test_object::construct_count, 5u);

  shared_ptr2.reset();
  shared_ptr3.reset();
  shared_ptr6.reset();

  EXPECT_EQ(test_object::destruct_count, 2u);
}

}  // namespace object_pool

int main() {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
