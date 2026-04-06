#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <thread>
#include <vector>

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

TEST_CASE("multithreads", "[multithreads]") {
    const int NUM_THREADS = 10;
    const int ITEMS = 1000;

    LockFreeQueue<int> q;
    std::atomic<int> counter(0);

    auto consume = [&q, &counter]() {
        int received = 0;
        while (received < NUM_THREADS * ITEMS) {
            std::shared_ptr<int> res = q.pop();
            if (res) {
                ++received;
                counter.fetch_add(*res, std::memory_order_relaxed);
            }
        }
        REQUIRE(q.empty());
    };

    auto produce = [&q]() {
        for (int i = 0; i < ITEMS; ++i) {
            q.push(i);
        }
    };

    SECTION("multiple producers, single consumer") {
        std::vector<std::thread> producers;
        for (int i = 0; i < NUM_THREADS; ++i) {
            producers.emplace_back(produce);
        }

        std::thread consumer(consume);

        for (auto& t : producers) {
            t.join();
        }

        consumer.join();

        REQUIRE(counter.load() == NUM_THREADS * (ITEMS - 1) * ITEMS / 2);
    }
}
