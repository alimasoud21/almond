#pragma once 

#include <chrono> // for real time 
#include <core/types.h> 

namespace exchange::core {

class LogicalClock {

private:
    Timestamp currrent_{0};

public:
    [[nodiscard]] Timestamp next() noexcept{return ++currrent_;}
    [[nodiscard]] Timestamp current() noexcept{return currrent_;}
    void reset() noexcept{ currrent_ = 0;}
};

[[nodiscard]] inline Timestamp steady_timestamp_now() noexcept{
    const auto now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<Timestamp>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(now).count());
}
}


