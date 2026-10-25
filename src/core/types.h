 #pragma once 

 #include <algorithm>
 #include <array>
 #include <cstdint>
 #include <string_view>
 #include <type_traits>

namespace exchange::core{

using Price = std::int64_t;

/*
could be written like this ??
std::int64_t Price;
*/

using Quantity = std::uint32_t;

using OrderId = std::uint64_t;

using SequenceNumber = std::uint64_t;

using Timestamp = std::uint64_t;

using ParticipantId = std::uint32_t;

using MatchId = uint64_t;

inline constexpr Price PRICE_SCALE = 10'000;

enum class Side : std::uint8_t{ // 4 byte by default minimized to 1 byte or 8 bits
    BUY,
    SELL,
};

//some busniss releated terms
enum class OrderType : std::uint8_t {
    LIMIT,
    MARKET,
    IOC,    // imediat or cancel
    FOK, // fil or kill
    GTC,    //good to can good till cancel
    STOP,
    STOP_LIMIT,
    ICEBERG, // show only slice order
    POST_ONLY, 
};

enum class OrderStatus : std::uint8_t{
    NEW,
    ACCEDPTED,
    PARTIALLY_FILLED,
    FILLED,
    CANCELED,
};

using Symbol = std::array<char, 8>; 

constexpr Symbol make_symbol(std::string_view text) noexcept{ 
    // 0 heap allocation and 0 copying 
    // constexpr tell the compiler to evaluate the funciton in compile time instead of run time (this is the compiler decison)
    // noexcept its a promes to the compiler that this functoin will never throw an error
    Symbol symbol{};
    const std::size_t length = std::min(symbol.size(), text.size());
    // copy n elements form source to destination
    std::copy_n(text.data(), length, symbol.begin());
    return symbol;
}

constexpr std::string_view symbol_view(const Symbol& symbol) noexcept{
    //c++ 20 features
    return {symbol.begin(), std::find(symbol.begin(), symbol.end(), '\0')};
}

// dont know what is this 
struct SymbolLess {
    constexpr bool operator()(const Symbol& lhs, const Symbol& rhs) const noexcept{
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
};

static_assert(std::is_trivially_copyable_v<Price>);
static_assert(std::is_trivially_copyable_v<Quantity>);
static_assert(std::is_trivially_copyable_v<OrderId>);
static_assert(std::is_trivially_copyable_v<SequenceNumber>);
static_assert(std::is_trivially_copyable_v<Timestamp>);
static_assert(std::is_trivially_copyable_v<ParticipantId>);
static_assert(std::is_trivially_copyable_v<MatchId>);
static_assert(std::is_trivially_copyable_v<Side>);
static_assert(std::is_trivially_copyable_v<OrderType>);
static_assert(std::is_trivially_copyable_v<OrderStatus>);
static_assert(std::is_trivially_copyable_v<Symbol>);
} 