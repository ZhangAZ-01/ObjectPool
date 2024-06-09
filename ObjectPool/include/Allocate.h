#pragma once

// Allocates and deallocates objects of type T.
template<typename T>
class Allocator
{
public:
    Allocator() = default;
    virtual T * allocate() = 0;
    virtual void deallocate(T * p) = 0;
};