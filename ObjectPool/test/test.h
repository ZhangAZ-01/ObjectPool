#pragma once

#include<stdio.h>

#include"D:\Cpp_Project\ObjectPool\include\ObjectPool.h"
#include"D:\Cpp_Project\ObjectPool\src\HeapAllocator.h"

const int MAX_SIZE = 4;

class Test
{
private:
    typedef ObjectPool<Test, HeapAllocator<Test, MAX_SIZE>> ObjectPool;
    static ObjectPool POOL;
public:
    Test() = delete;
    explicit Test(int data) : data_(data)
    {
        printf("Test Construct\n");
    }
    ~Test()
    {
        printf("Test Destruct\n");
    }

    void * operator new(size_t n)
    {
        printf("Test new from object pool\n");
        return POOL.allocate(n);
    }
    void operator delete(void * p)
    {
        printf("Test delete from object pool\n");
        POOL.deallocate(p);
    }
private:
    int data_;
};
Test::ObjectPool Test::POOL;