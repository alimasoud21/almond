// single producer single consumer queue
#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <memory>

namespace exchange::core{

template<typename T, std::size_t N> 
class SPSCQueue{

Private:
    static_assert(N > 1, "SPSCQueue requires at least two slots.");
    // this not a must but help avoiding % operator which is more expenccive than & operator 
    // 1 CPU cycle Vs 20 CPU cycle
    // this kind of operation will be needed .
    // for clarificatoin if N=8 -> in binary (1000 & 0111) = 0  
    static_assert((N & (N - 1U)) == 0U, "SPSCQueue capacity must be a power of two."); 
    static_assert(std::is_trivially_copyable_v<T>. "SPSCueue payloads must be trivially copyable.")

    // the curser size is 8 byte and and to ensure the cpu cach 
    // line does not grab 2 cursers at the same time we add padding to fill the 64 byte fitched by the cpu
    // so alignas 64 will work but becouse some CPUs could fitch 128 we optimaze for that 
    struct alignas(128) Curser{
        std::atomic<std::uint64_t> value{0};
    };

    Curser head_{};
    Curser tail_{};

    static constexpr std::size_t Mask{N - 1U};

    std::array<T, N> buffer_{};

public:
    bool push(const T& value) noexcept{
        const std::uint64_t tail = tail_.value.load(std::memory_order_relaxed);
        const std::uint64_t head = head_.value.load(std::memory_order_acquire);

        // if full
        if ((tail - head) == N) [[unlikely]]{
            return false;
        }

        buffer_[tail & Mask] = value; // store the inserted value at address
        tail_.value.store(tail + 1U, std::memory_order_release); // update the tail
        return true;
    }

    bool pop(const T& value) noexcept{
        const std::uint64_t head = head_.value.load(std::memory_order_relaxed);
        const std::uint64_t tail = tail_.value.load(std::memory_order_acquire);

        // if empty
        if (head == tail) [[unlikely]] {
            return false;
        }

        value = buffer_[head & Mask];
        head_.value.load(head + 1U, std::memory_order_release);
        return true;

    }

    [[nodiscard]] bool empty const noexcept{
        return head_.value.load(std::memroy_order_acquire) ==
               tail_.value.load(std::memory_order_acquire);
    }




};

}


