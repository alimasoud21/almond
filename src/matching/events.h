#pragma once 

#include <cstddef>
#include <type_traits>
#include <variant>
#include "../core/types.h"

namespace exchange::matching{

enum class ReasonCode : std::uint16_t {
  NONE = 0,
  ZERO_QUANTITY,
  NEGATIVE_PRICE,
  BOOK_EMPTY,
  DUPLICATE_ORDER_ID,
  ORDER_NOT_FOUND,
  ORDER_NOT_ACTIVE,
  FOK_INSUFFICIENT_LIQUIDITY,
  POST_ONLY_WOULD_CROSS, // 
  INVALID_MODIFICATION,
  INVALID_ICEBERG_DISPLAY,
  ORDER_POOL_EXHAUSTED,
};

enum class EventType : std::uint8_t {
  ORDER_ACCEPTED,
  ORDER_RESTED,
  ORDER_REJECTED,
  ORDER_REDUCED, //
  ORDER_PARTIALLY_FILLED,
  ORDER_FILLED,
  ORDER_CANCELED,
  TRADE,
};

struct OrderAccepted {
  core::SequenceNumber sequence_number;
  core::Timestamp timestamp;
  core::OrderId order_id;
  core::Symbol symbol;
  core::Side side;
  core::Price price;
  core::Quantity qty;
  core::OrderType order_type;
};

struct OrderRested {
  core::SequenceNumber sequence_number;
  core::Timestamp timestamp;
  core::OrderId order_id;
  core::Symbol symbol;
  core::Side side;
  core::Price price;
  core::Quantity qty;
};

struct OrderRejected {
  core::SequenceNumber sequence_number;
  core::Timestamp timestamp;
  core::OrderId order_id;
  ReasonCode reason_code;
};

struct OrderReduced {
  core::SequenceNumber sequence_number;
  core::Timestamp timestamp;
  core::OrderId order_id;
  core::Quantity new_qty;
};

struct OrderPartiallyFilled {
  core::SequenceNumber sequence_number;
  core::Timestamp timestamp;
  core::OrderId order_id;
  core::Quantity filled_qty;
  core::Quantity remaining_qty;
  core::Price price;
};

struct OrderFilled {
  core::SequenceNumber sequence_number;
  core::Timestamp timestamp;
  core::OrderId order_id;
  core::Quantity filled_qty;
  core::Price price;
};

struct OrderCanceled {
  core::SequenceNumber sequence_number;
  core::Timestamp imestamp;
  core::OrderId order_id;
  core::Quantity canceled_qty;
};

struct Trade {
  core::SequenceNumber sequence_number;
  core::Timestamp timestamp;
  core::MatchId match_id;
  core::Symbol symbol;
  core::OrderId buy_order_id;
  core::OrderId sell_order_id;
  core::Price price;
  core::Quantity qty;
};

struct Event {
  std::variant<
    OrderAccepted, 
    OrderRested, 
    OrderRejected, 
    OrderReduced,
    OrderPartiallyFilled,
    OrderFilled,
    OrderCanceled,
    Trade
  > payload;
};

static_assert(std::is_trivially_copyable_v<ReasonCode>);
static_assert(std::is_trivially_copyable_v<EventType>);
static_assert(std::is_trivially_copyable_v<OrderAccepted>);
static_assert(std::is_trivially_copyable_v<OrderRested>);
static_assert(std::is_trivially_copyable_v<OrderRejected>);
static_assert(std::is_trivially_copyable_v<OrderReduced>);
static_assert(std::is_trivially_copyable_v<OrderPartiallyFilled>);
static_assert(std::is_trivially_copyable_v<OrderFilled>);
static_assert(std::is_trivially_copyable_v<OrderCanceled>);
static_assert(std::is_trivially_copyable_v<Trade>);
static_assert(std::is_trivially_copyable_v<Event>);

}