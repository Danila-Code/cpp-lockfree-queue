#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "../include/lock-free-queue.h"

TEST_CASE("test empty method", "[empty]") {
    LockFreeQueue<int> q;
    REQUIRE(q.empty());

    for (int i = 0; i < 10; ++i) {
        q.push(i);
    }
    REQUIRE(!q.empty());

    SECTION("push element") {
        q.push(1);
        REQUIRE(!q.empty());
    }

    SECTION("pop element") {
        q.pop();
        REQUIRE(!q.empty());
    }

    SECTION("pop all elements") {
        for (int i = 0; i < 10; ++i) {
            q.pop();
        }
        REQUIRE(q.empty());
    }
}

TEST_CASE("test push method", "[push]") {
    LockFreeQueue<int> q;
    REQUIRE(q.empty());

    int push_element(10);
    q.push(push_element);

    REQUIRE(!q.empty());

    auto pop_element = q.pop();
    REQUIRE(q.empty());

    REQUIRE(push_element == *pop_element);
}

TEST_CASE("test pop method", "[pop]") {
    LockFreeQueue<int> q;
    REQUIRE(q.empty());

    SECTION("pop element") {
        int push_element(10);
        q.push(push_element);

        REQUIRE(!q.empty());

        auto pop_element = q.pop();
        REQUIRE(q.empty());

        REQUIRE(push_element == *pop_element);
    }

    SECTION("throw exception") {
        std::shared_ptr<int> empty_ptr;
        bool error = false;
        try {
            empty_ptr = q.pop();
        } catch (const std::runtime_error& e) {
            error = true;
        }

        REQUIRE(!empty_ptr);
        REQUIRE(error);
    }
}
