#include <iostream>

#include "../include/lock-free-queue.h"

int main() {
    LockFreeQueue<int> q;

    const size_t count = 10;

    for (size_t i = 0; i < count; ++i) {
        q.push(i);
    }

    std::cout << "Hello" << std::endl;
}
