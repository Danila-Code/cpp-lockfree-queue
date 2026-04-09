#pragma once

#include <atomic>
#include <memory>

// очередь на основе атомарных указателей (очередь Майкла и Скотта)
template <typename T>
class LockFreeQueue {
private:
    struct Node {
        Node() : next(nullptr) {}

        Node(const T& value) : data(std::make_shared<T>(value)), next(nullptr) {}

        std::shared_ptr<T> data;
        std::atomic<Node*> next;
    };

public:
    explicit LockFreeQueue() {
        Node* head_node = new Node();
        head_.store(head_node);
        tail_.store(head_node);
    }

    ~LockFreeQueue() noexcept {
        while (!empty()) {
            pop();
        }

        delete head_.load();
    }

    void push(T value) {
        // todo: add try catch for allocation failure
        Node* new_node = new Node(value);

        while (true) {
            Node* current_tail = tail_.load(std::memory_order_acquire);
            Node* null_node = nullptr;

            if (tail_.load(std::memory_order_acquire)
                    ->next.compare_exchange_strong(null_node,
                                                   new_node,
                                                   std::memory_order_release,
                                                   std::memory_order_relaxed)) {
                tail_.compare_exchange_strong(current_tail,
                                              new_node,
                                              std::memory_order_release,
                                              std::memory_order_relaxed);
                return;
            } else {
                tail_.compare_exchange_strong(current_tail,
                                              current_tail->next.load(std::memory_order_acquire),
                                              std::memory_order_release,
                                              std::memory_order_relaxed);
            }
        }
    }

    std::shared_ptr<T> pop() {
        std::shared_ptr<T> res;

        while (true) {
            Node* current_head = head_.load(std::memory_order_acquire);
            Node* current_tail = tail_.load(std::memory_order_acquire);
            Node* head_next = current_head->next.load(std::memory_order_acquire);

            if (current_head == head_.load(std::memory_order_acquire)) {
                if (current_head == current_tail) {
                    if (!head_next) {
                        return nullptr;
                    } else {
                        tail_.compare_exchange_strong(current_tail,
                                                      head_next,
                                                      std::memory_order_release,
                                                      std::memory_order_relaxed);
                    }
                } else {
                    res = head_next->data;
                    if (head_.compare_exchange_strong(current_head,
                                                      head_next,
                                                      std::memory_order_release,
                                                      std::memory_order_relaxed)) {
                        delete current_head;
                        break;
                    }
                }
            }
        }

        return res;
    }

    bool empty() {
        return head_.load() == tail_.load();
    }

private:
    std::atomic<Node*> head_;
    std::atomic<Node*> tail_;
};
