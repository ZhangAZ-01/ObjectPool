// Copyright 2024 Vipshop Inc. All Rights Reserved.
// Author: Aizhen Zhang (869723386@qq.com)
//

#include "object_pool.h"

class TestObject {
private:
  int id_;

public:
  TestObject(int id) : id_(id) {
    printf("Creating TestObject with id: %d\n", id_);
  }
  ~TestObject() { printf("Destroying TestObject with id: %d\n", id_); }

public:
  int getId() const { return id_; }
};

void test_object_reuse(objectpool::object_pool<TestObject> &pool) {
  printf("Testing object reuse:\n");
  auto obj1 = pool.get<int>();
  if (obj1) {
    printf("Got object with id: %d\n", obj1->getId());
  }
  auto obj2 = pool.get<int>();
  if (obj2) {
    printf("Got object with id: %d\n", obj2->getId());
  }
  // Return objects to the pool
  obj1.reset();
  obj2.reset();
  auto obj3 = pool.get<int>();
  if (obj3) {
    printf("Got object with id: %d\n", obj3->getId());
  }
  auto obj4 = pool.get<int>();
  if (obj4) {
    printf("Got object with id: %d\n", obj4->getId());
  }
}

void test_max_object_limit(objectpool::object_pool<TestObject> &pool) {
  printf("Testing maximum object limit:\n");
  std::vector<std::shared_ptr<TestObject>> objects;
  for (int i = 0; i < objectpool::MAX_OBJECT_NUM + 1; ++i) {
    auto obj = pool.get<int>();
    if (obj) {
      printf("Got object with id: %d\n", obj->getId());
      objects.push_back(obj);
    } else {
      printf("No more objects available in pool.\n");
    }
  }
}

int main() {
  objectpool::object_pool<TestObject> pool;
  pool.init(10, 0);

  test_object_reuse(pool);
  test_max_object_limit(pool);

  return 0;
}
