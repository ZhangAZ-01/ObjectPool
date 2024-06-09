#pragma once

#include <algorithm>
#include <memory>

#include "D:\Cpp_Project\ObjectPool\include\Allocate.h"

template <typename T, int N> class HeapAllocator : public Allocator<T> {
private:
  enum STATE { STATE_USED = 0, STATE_FREE = 1 };
  unsigned char data_[sizeof(T) * N];
  int availble_ = N;

public:
  struct Entry {
    STATE state;
    T *p;

    bool operator<(const Entry &other) const { return state < other.state; }
  };

private:
  Entry entry[N];

public:
  HeapAllocator() {
    for (int i = 0; i < N; ++i) {
      entry[i].state = STATE_FREE;
      entry[i].p = reinterpret_cast<T *>(&data_[sizeof(T) * i]);
    }
    std::make_heap(entry, entry + N);
  }
  ~HeapAllocator() = default;

public:
  virtual T *allocate() {
    if (availble_ <= 0) {
      return nullptr;
    }
    Entry e = entry[0];
    std::pop_heap(entry, entry + N);
    --availble_;
    entry[availble_].state = STATE_USED;
    entry[availble_].p = nullptr;
    return e.p;
  }
  virtual void deallocate(T *p) {
    if (p == nullptr || availble_ >= N) {
      return;
    }
    entry[availble_].state = STATE_FREE;
    entry[availble_].p = reinterpret_cast<T *>(p);
    ++availble_;
    std::push_heap(entry, entry + N);
  }
};