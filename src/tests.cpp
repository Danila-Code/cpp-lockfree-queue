#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "../include/lock-free-queue.h"

TEST_CASE("Test empty", "[empty]") {
    LockFreeQueue<int> q;
    REQUIRE(q.empty());

    q.push(1);
    REQUIRE(!q.empty());

    q.pop();
    REQUIRE(q.empty());
}
