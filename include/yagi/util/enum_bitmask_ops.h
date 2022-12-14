#ifndef YAGI_UTIL_ENUM_BITMASK_OPS_H
#define YAGI_UTIL_ENUM_BITMASK_OPS_H

#include <type_traits>

#define ENABLE_BITMASK_OPERATORS(x)             \
  template<> struct EnableBitMaskOperators<x> { \
    static const bool enable = true;            \
  };

template<typename Enum>
struct EnableBitMaskOperators {
  static const bool enable = false;
};

template<typename Enum>
typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
operator|(Enum lhs, Enum rhs) {
  using underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum>(
      static_cast<underlying>(lhs) |
      static_cast<underlying>(rhs)
  );
}

template<typename Enum>
typename std::enable_if_t<EnableBitMaskOperators<Enum>::enable, Enum>
operator&(Enum lhs, Enum rhs) {
  using underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum>(
      static_cast<underlying>(lhs) &
      static_cast<underlying>(rhs)
  );
}

template<typename Enum>
typename std::underlying_type<Enum>::type
unwrap(Enum e) {
  using underlying = typename std::underlying_type<Enum>::type;
  return static_cast<underlying>(e);
}

template<typename Enum>
typename std::underlying_type<Enum>::type
set(Enum e, Enum mask) {
  return (e & mask) == mask;
}

#endif //YAGI_UTIL_ENUM_BITMASK_OPS_H
