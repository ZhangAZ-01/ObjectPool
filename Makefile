# Copyright 2024 Vipshop Inc. All Rights Reserved.
# Author: Aizhen Zhang (869723386@qq.com)
#

CXX := g++
CPPFLAGS := -DNDEBUG=1 -I./gtest/googletest/include
CXXFLAGS := -g -O3 -std=c++17 -Wall -Wextra -Werror -pedantic
LDFLAGS := -L./gtest/build/lib -lgtest -lgtest_main

all: object_pool_test object_pool_test.o
.PHONY: all

object_pool_test: object_pool_test.o
	$(CXX) -o $@ $^ $(LDFLAGS)

object_pool_test.o: object_pool_test.cc object_pool.h
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -f object_pool_test object_pool_test.o

.PHONY: clean
