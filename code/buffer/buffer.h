#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>
#include <cassert>

class Buffer
{
public:
    Buffer();
    ~Buffer() = default;

private:
};

#endif