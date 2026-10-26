#pragma once

#include "matching/order.h"

namespace exchange::matching{

struct Level {
    core::Price price{0};
    core::Quantity total_qty{0};
    std::uint32_t order_count{0};
    Order* head{nullptr};
    Order* tail{nullptr};

    void add_order(Order* order) noexcept{
        order->prev = tail; // the new order points to the tail
        (*order).next = nullptr;
        order->Parent_level = this;

        if (tail != nullptr) { // not empty
            tail->next = order; // the tail points back to the new order 
        } else { // empty
            head = order;
        }

        tail = order; // the new order becomes the tail
        total_qty += order->display_qty;
        ++order_count;

    }

    void remove_order(Order* order) noexcept {
        // if there is order before it
        if (order->prev != nullptr){
            order->prev->next = order->next;
        } else {
            head = order->next;
        }

        // if there is order after it
        if (order-> next != nullptr){
            order->next->prev = order->prev;
        } else {
            tail = order->prev;
        }

        total_qty -= order->display_qty;
        --order_count;

        // the order removed safely points to nothing
        order->next = nullptr;
        order->prev = nullptr;
        order->Parent_level = nullptr;

    }

    [[nodiscard]] Order* front() const noexcept {return head;}
    [[nodiscard]] bool is_empty() const noexcept {return head == nullptr;}
};

}