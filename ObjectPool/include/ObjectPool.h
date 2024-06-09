#pragma once

#include <stdio.h>

template<typename T, typename Allocator>
class ObjectPool
{
private:
    Allocator allocator_;
    
public:
    ObjectPool() = default;
    ~ObjectPool() = default;

public:
    void * allocate(size_t n)
    {
        if (sizeof(T) != n)
        {
            printf("Error");
            return nullptr;
        }
        return allocator_.allocate();
    }
    void deallocate(void * p)
    {
        allocator_.deallocate(static_cast<T *>(p));
    }
};
