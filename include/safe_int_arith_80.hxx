// Copyright (C) 2020-2024 Valentin Nechayev.
// In public domain.

#pragma once

#include <cstring>
#include <limits>
#include <stdexcept>
#include <type_traits>

#define SIA80_UNLIKELY(x) __builtin_expect(long(x), 0)

// cx_xxx: checked versions - generate exception on error.
// cf_xxx: checked-with-flag versions - set flag to 1 on error,
//   the main result as for truncating.
// tr_xxx: truncating (wrapping) versions - maximum tolerance,
//   return truncated version of infinitely precise value.
// sr_xxx: saturating versions - maximum tolerance,
//   return the closest value to the infinitely precise one.

// xx_add: addition of two values.
// xx_sub: subtraction of two values.
// xx_mul: multiplication of two values.
// xx_div: T-division of two values, quotient of the result.
// xx_rem: T-division of two values, remainder of the result.
// xx_shl: left shift of the first value.
//   Overflow depends on the first value type (signed or unsigned).
// xx_shr: right shift of the first value.
//   Shift type depends on the first value type (signed or unsigned).
// xx_conv: conversion to the target type.
//   Example: tr_conv<int8_t>(val)
// xx_ufit: fitting to the specified number of bits as unsigned.
// xx_sfit: fitting to the specified number of bits as signed.

namespace sia80 {

  // TODO: Add cf_xxx 2-argument variants with a thread-local flag.
  // TODO: Add sf_xxx Saturation with flag setting (explicit or thread-local).
  // TODO: Add xx_shrx Exact division interpretation.

  // Got from cppreference.com.
  // Requires C++17. Not needed since C++20.
  // TODO Use conditional include on C++ version.
  template<class To, class From>
  std::enable_if_t<
      sizeof(To) == sizeof(From) &&
      std::is_trivially_copyable_v<From> &&
      std::is_trivially_copyable_v<To>,
      To>
  // constexpr support needs compiler magic
  inline ia_bit_cast(const From& src) noexcept
  {
      static_assert(std::is_trivially_constructible_v<To>,
          "This implementation additionally requires "
          "destination type to be trivially constructible");
      To dst;
      std::memcpy(&dst, &src, sizeof(To));
      return dst;
  }

  //-- add -----------------------------------------------------

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cx_add(T1 v1, T2 v2) -> decltype(v1+v2)
  {
    using TR = decltype(v1+v2);
    TR result;
    bool ovf = __builtin_add_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      throw std::overflow_error("cx_add");
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cf_add(T1 v1, T2 v2, int *flag) -> decltype(v1+v2)
  {
    using TR = decltype(v1+v2);
    TR result;
    bool ovf = __builtin_add_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      *flag = 1;
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto tr_add(T1 v1, T2 v2) -> decltype(v1+v2)
  {
    using TR = decltype(v1+v2);
    TR result;
    __builtin_add_overflow(v1, v2, &result);
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto sr_add(T1 v1, T2 v2) -> decltype(v1+v2)
  {
    using TR = decltype(v1+v2);
    TR result;
    bool ovf = __builtin_add_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      if (v1 < 0) {
        return std::numeric_limits<TR>::min();
      }
      return std::numeric_limits<TR>::max();
    }
    return result;
  }

  //-- sub -----------------------------------------------------

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cx_sub(T1 v1, T2 v2) -> decltype(v1-v2)
  {
    using TR = decltype(v1-v2);
    TR result;
    bool ovf = __builtin_sub_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      throw std::overflow_error("cx_sub");
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cf_sub(T1 v1, T2 v2, int *flag) -> decltype(v1-v2)
  {
    using TR = decltype(v1-v2);
    TR result;
    bool ovf = __builtin_sub_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      *flag = 1;
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto tr_sub(T1 v1, T2 v2) -> decltype(v1-v2)
  {
    using TR = decltype(v1-v2);
    TR result;
    __builtin_sub_overflow(v1, v2, &result);
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto sr_sub(T1 v1, T2 v2) -> decltype(v1-v2)
  {
    using TR = decltype(v1-v2);
    TR result;
    bool ovf = __builtin_sub_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      if (v1 < 0) {
        return std::numeric_limits<TR>::min();
      }
      return std::numeric_limits<TR>::max();
    }
    return result;
  }

  //-- mul -----------------------------------------------------

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cx_mul(T1 v1, T2 v2) -> decltype(v1*v2)
  {
    using TR = decltype(v1*v2);
    TR result;
    bool ovf = __builtin_mul_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      throw std::overflow_error("cx_mul");
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cf_mul(T1 v1, T2 v2, int *flag) -> decltype(v1*v2)
  {
    using TR = decltype(v1*v2);
    TR result;
    bool ovf = __builtin_mul_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      *flag = 1;
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto tr_mul(T1 v1, T2 v2) -> decltype(v1*v2)
  {
    using TR = decltype(v1*v2);
    TR result;
    __builtin_mul_overflow(v1, v2, &result);
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto sr_mul(T1 v1, T2 v2) -> decltype(v1*v2)
  {
    using TR = decltype(v1*v2);
    TR result;
    bool ovf = __builtin_mul_overflow(v1, v2, &result);
    if (SIA80_UNLIKELY(ovf)) {
      if ((v1 < 0 && v2 >= 0) || (v1 >= 0 && v2 < 0)) {
        return std::numeric_limits<TR>::min();
      }
      return std::numeric_limits<TR>::max();
    }
    return result;
  }

  //-- div -----------------------------------------------------

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cx_div(T1 ddnd, T2 dvsr) -> decltype(ddnd/dvsr)
  {
    using TR = decltype(ddnd/dvsr);
    if (SIA80_UNLIKELY(dvsr == 0)) {
      throw std::domain_error("cx_div divisor 0");
    }
    if constexpr(std::is_signed<T2>::value) {
      constexpr TR rvmin = std::numeric_limits<TR>::min();
      if (SIA80_UNLIKELY(dvsr == -1 && ddnd == rvmin)) {
        throw std::overflow_error("cx_div min neg");
      }
    }
    return ddnd / dvsr;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cf_div(T1 ddnd, T2 dvsr, int *flag) -> decltype(ddnd/dvsr)
  {
    using TR = decltype(ddnd/dvsr);
    if (SIA80_UNLIKELY(dvsr == 0)) {
      *flag = 1;
      return ~TR(0);
    }
    if constexpr(std::is_signed<T2>::value) {
      constexpr TR rvmin = std::numeric_limits<TR>::min();
      if (SIA80_UNLIKELY(dvsr == -1 && ddnd == rvmin)) {
        *flag = 1;
        return rvmin;
      }
    }
    return ddnd / dvsr;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto tr_div(T1 ddnd, T2 dvsr) -> decltype(ddnd/dvsr)
  {
    using TR = decltype(ddnd/dvsr);
    if (SIA80_UNLIKELY(dvsr == 0)) {
      return ~TR(0);
    }
    if constexpr(std::is_signed<T2>::value) {
      const TR rvmin = std::numeric_limits<TR>::min();
      if (SIA80_UNLIKELY(dvsr == -1 && ddnd == rvmin)) {
        return rvmin;
      }
    }
    return ddnd / dvsr;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto sr_div(T1 ddnd, T2 dvsr) -> decltype(ddnd/dvsr)
  {
    using TR = decltype(ddnd/dvsr);
    const TR rvmax = std::numeric_limits<TR>::max();
    const TR rvmin = std::numeric_limits<TR>::min();
    if (SIA80_UNLIKELY(dvsr == 0)) {
      if (ddnd < 0) {
        return rvmin;
      }
      return rvmax;
    }
    if constexpr(std::is_signed<T2>::value) {
      if (SIA80_UNLIKELY(dvsr == -1 && ddnd == rvmin)) {
        return rvmax;
      }
    }
    return ddnd / dvsr;
  }

  //-- rem -----------------------------------------------------

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cx_rem(T1 ddnd, T2 dvsr) -> decltype(ddnd%dvsr)
  {
    // We avoid the division operation even if formally
    // there is no overflow for the remainder itself.
    // This pertains to both special cases (x/0 and MIN/-1).
    using TR = decltype(ddnd%dvsr);
    if (SIA80_UNLIKELY(dvsr == 0)) {
      throw std::domain_error("cx_rem divisor 0");
    }
    if constexpr(std::is_signed<T2>::value) {
      const TR rvmin = std::numeric_limits<TR>::min();
      if (SIA80_UNLIKELY(dvsr == -1 && ddnd == rvmin)) {
        throw std::overflow_error("cx_rem min neg");
      }
    }
    return ddnd % dvsr;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cf_rem(T1 ddnd, T2 dvsr, int *flag) -> decltype(ddnd%dvsr)
  {
    // We report the division operation even if formally
    // there is no overflow for the remainder itself.
    // This pertains to both special cases (x/0 and MIN/-1).
    using TR = decltype(ddnd%dvsr);
    if (SIA80_UNLIKELY(dvsr == 0)) {
      *flag = 1;
      return 0;
    }
    if constexpr(std::is_signed<T2>::value) {
      const TR rvmin = std::numeric_limits<TR>::min();
      if (SIA80_UNLIKELY(dvsr == -1 && ddnd == rvmin)) {
        *flag = 1;
        return 0;
      }
    }
    return ddnd % dvsr;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto tr_rem(T1 ddnd, T2 dvsr) -> decltype(ddnd%dvsr)
  {
    using TR = decltype(ddnd%dvsr);
    if (SIA80_UNLIKELY(dvsr == 0)) {
      return 0;
    }
    if constexpr(std::is_signed<T2>::value) {
      const TR rvmin = std::numeric_limits<TR>::min();
      if (SIA80_UNLIKELY(dvsr == -1 && ddnd == rvmin)) {
        return 0;
      }
    }
    return ddnd % dvsr;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto sr_rem(T1 ddnd, T2 dvsr) -> decltype(ddnd%dvsr)
  {
    return tr_rem(ddnd, dvsr);
  }

  //-- shl

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cx_shl(T1 v1, T2 shcnt) -> decltype(v1 << shcnt)
  {
    // Notice: integral promotion can widen the value to width of TR.
    // We check against TR, not T1.
    using TR = decltype(v1 << shcnt);
    if (SIA80_UNLIKELY(shcnt < 0 || shcnt >= std::numeric_limits<TR>::digits)) {
      throw std::out_of_range("cx_shl shift count");
    }
    // Criterion for the check: value correctly shifts back to
    // the original one. But the first shift left shall be done
    // with guarantee for unsigned, to preclude overflow effects.
    // So, convert to unsigned form for left shift and back to
    // original (if differ) for right shift.
    // As integral promotion is in effect before any real shift,
    // operate in its types (TR, UTR).
    using UTR = std::make_unsigned_t<TR>;
    UTR xv1 = v1;
    UTR uresult = xv1 << shcnt;
    TR result = ia_bit_cast<TR>(uresult);
    TR checkback = result >> shcnt;
    if (SIA80_UNLIKELY(v1 != checkback)) {
      throw std::overflow_error("cx_shl overflow");
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cf_shl(T1 v1, T2 shcnt, int *flag) -> decltype(v1 << shcnt)
  {
    // Notice: integral promotion can widen the value to width of TR.
    // We check against TR, not T1.
    using TR = decltype(v1 << shcnt);
    if (SIA80_UNLIKELY(shcnt < 0 || shcnt >= std::numeric_limits<TR>::digits)) {
      *flag = 1;
      return 0;
    }
    // See note for cx_shl().
    using UTR = std::make_unsigned_t<TR>;
    UTR xv1 = v1;
    UTR uresult = xv1 << shcnt;
    TR result = ia_bit_cast<TR>(uresult);
    TR checkback = result >> shcnt;
    if (SIA80_UNLIKELY(v1 != checkback)) {
      *flag = 1;
    }
    return result;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto tr_shl(T1 v1, T2 shcnt) -> decltype(v1 << shcnt)
  {
    using TR = decltype(v1 << shcnt);
    if (SIA80_UNLIKELY(shcnt < 0 || shcnt >= std::numeric_limits<TR>::digits)) {
      return 0;
    }
    // See note for cx_shl().
    using UTR = std::make_unsigned_t<TR>;
    UTR xv1 = ia_bit_cast<UTR>(v1);
    UTR uresult = xv1 << shcnt;
    return ia_bit_cast<TR>(uresult);
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto sr_shl(T1 v1, T2 shcnt) -> decltype(v1 << shcnt)
  {
    // Notice: integral promotion can widen the value to width of TR.
    // We check against TR, not T1.
    using TR = decltype(v1 << shcnt);
    const TR rvmax = std::numeric_limits<TR>::max();
    const TR rvmin = std::numeric_limits<TR>::min();
    if (SIA80_UNLIKELY(shcnt < 0 || shcnt >= std::numeric_limits<TR>::digits)) {
      if (v1 < 0) {
        return rvmin;
      }
      else if (v1 == 0) {
        return 0;
      }
      else {
        return rvmax;
      }
    }
    // See note for cx_shl().
    using UTR = std::make_unsigned_t<TR>;
    UTR xv1 = v1;
    UTR uresult = xv1 << shcnt;
    TR result = ia_bit_cast<TR>(uresult);
    TR checkback = result >> shcnt;
    if (SIA80_UNLIKELY(v1 != checkback)) {
      if (v1 < 0) {
        return rvmin;
      }
      return rvmax;
    }
    return result;
  }

  //-- shr

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cx_shr(T1 v1, T2 shcnt) -> decltype(v1 >> shcnt)
  {
    // Notice: integral promotion can widen the value to width of TR.
    // We check against TR, not T1.
    using TR = decltype(v1 >> shcnt);
    if (SIA80_UNLIKELY(shcnt < 0 || shcnt >= std::numeric_limits<TR>::digits)) {
      throw std::out_of_range("cx_shr shift count");
    }
    return v1 >> shcnt;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto cf_shr(T1 v1, T2 shcnt, int *flag) -> decltype(v1 >> shcnt)
  {
    // Notice: integral promotion can widen the value to width of TR.
    // We check against TR, not T1.
    using TR = decltype(v1 >> shcnt);
    if (SIA80_UNLIKELY(shcnt < 0 || shcnt >= std::numeric_limits<TR>::digits)) {
      *flag = 1;
      if (v1 < 0) {
        return ~TR(0);
      }
      return 0;
    }
    return v1 >> shcnt;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto tr_shr(T1 v1, T2 shcnt) -> decltype(v1 >> shcnt)
  {
    using TR = decltype(v1 >> shcnt);
    if (SIA80_UNLIKELY(shcnt < 0 || shcnt >= std::numeric_limits<TR>::digits)) {
      if (v1 < 0) {
        return ~TR(0);
      }
      return 0;
    }
    return v1 >> shcnt;
  }

  template <typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline auto sr_shr(T1 v1, T2 shcnt) -> decltype(v1 >> shcnt)
  {
    return tr_shr(v1, shcnt);
  }

  //-- conv

  template<typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline T1 cx_conv(T2 ival)
  {
    T1 result;
    bool ovf = __builtin_add_overflow(ival, 0, &result);
    if (SIA80_UNLIKELY(ovf)) {
      throw std::range_error("cx_conv");
    }
    return result;
  }

  template<typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline T1 cf_conv(T2 ival, int *flag)
  {
    T1 result;
    bool ovf = __builtin_add_overflow(ival, 0, &result);
    if (SIA80_UNLIKELY(ovf)) {
      *flag = 1;
    }
    return result;
  }

  template<typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline T1 tr_conv(T2 ival)
  {
    T1 result;
    __builtin_add_overflow(ival, 0, &result);
    return result;
  }

  template<typename T1, typename T2,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_integral<T2>::value, bool> = true>
  inline T1 sr_conv(T2 ival)
  {
    T1 result;
    const T1 rvmax = std::numeric_limits<T1>::max();
    const T1 rvmin = std::numeric_limits<T1>::min();
    bool ovf = __builtin_add_overflow(ival, 0, &result);
    if (SIA80_UNLIKELY(ovf)) {
      if (ival < 0) {
        return rvmin;
      }
      return rvmax;
    }
    return result;
  }

  //-- ufit ----------------------------------------------------

  template<typename T1,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true>
  inline T1 cx_ufit(T1 ival, unsigned nbits)
  {
    if (SIA80_UNLIKELY(ival < 0)) {
      throw std::range_error("cx_ufit: negative");
    }
    // FFR: With C++20, use consteval.
    constexpr unsigned tbits = std::numeric_limits<T1>::digits;
    // NB We check in T1 size. For example, tbits is 32 for uint32_t and
    // 31 for int32_t. We can always shift 1 left by value strictly less
    // than tbits. If nbits >= tbits, value fits except T1_min case, but
    // we excluded negative inputs earlier.
    if (nbits >= tbits) {
      return ival;
    }
    // NB For unsigned, 0 fits into 0 bits.
    using T1X = decltype(ival + 0); // integral promotion
    const T1X limit = T1X(1) << nbits;
    if (SIA80_UNLIKELY(ival >= limit)) {
      throw std::range_error("cx_ufit: too big");
    }
    return ival;
  }

  template<typename T1,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true>
  inline T1 cf_ufit(T1 ival, unsigned nbits, int *flag)
  {
    if (SIA80_UNLIKELY(ival < 0)) {
      *flag = 1;
      // NB We don't exit here. Future masking will extract only
      // needed bits.
    }
    // FFR: With C++20, use consteval.
    constexpr unsigned tbits = std::numeric_limits<T1>::digits;
    if (nbits >= tbits) {
      return ival;
    }
    using T1X = decltype(ival + 0); // integral promotion
    const T1X mask = (T1X(1) << nbits) - 1;
    T1X ret = T1X(ival) & mask;
    if (ret != ival) {
      *flag = 1;
    }
    return ret;
  }

  template<typename T1,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true>
  inline T1 tr_ufit(T1 ival, unsigned nbits)
  {
    // FFR: With C++20, use consteval.
    constexpr unsigned tbits = std::numeric_limits<T1>::digits;
    if (nbits >= tbits) {
      return ival;
    }
    using T1X = decltype(ival + 0); // integral promotion
    const T1X mask = (T1X(1u) << nbits) - 1;
    return T1X(ival) & mask;
  }
  // TODO sr_ufit

  //-- sfit ----------------------------------------------------

  template<typename T1,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_signed<T1>::value, bool> = true>
  inline T1 cx_sfit(T1 ival, unsigned nbits)
  {
    // The logic to detect bounds is:
    // Example:
    // 0x7fffffff fits in 32 bits but not in 31.
    // So, if input type is 32 bits full (digits+1) and request
    // is nbits >= 32, all is satisfied.
    // If nbits < 31, we can form limits with trivial shifts.
    // Example: to fit in 4 bits, value shall be in [-8..7], so, in [~7..7].
    // FFR: With C++20, use consteval.
    constexpr unsigned ftbits = std::numeric_limits<T1>::digits + 1;
    if (nbits >= ftbits) {
      return ival;
    }
    // NB For signed, nothing fits in 0 bits unless we artificially
    // define a special case (but we don't need to).
    if (SIA80_UNLIKELY(nbits == 0)) {
      throw std::range_error("cx_sfit: nbits==0");
    }
    using T1X = decltype(ival + 0); // integral promotion
    const T1X tmax = (T1X(1) << (nbits - 1)) - 1;
    if (SIA80_UNLIKELY(ival > tmax)) {
      throw std::range_error("cx_sfit: too big");
    }
    if (SIA80_UNLIKELY(ival < ~tmax)) {
      throw std::range_error("cx_sfit: too small");
    }
    return ival;
  }

  // Difference from signed T1: promotion to unsigned;
  // no check for negative input.
  template<typename T1,
      std::enable_if_t<std::is_integral<T1>::value, bool> = true,
      std::enable_if_t<std::is_unsigned<T1>::value, bool> = true>
  inline T1 cx_sfit(T1 ival, unsigned nbits)
  {
    // A strange case of request to fit an unsigned value into
    // a signed field...
    // We need at least one bit more than for full value.
    constexpr unsigned ubits = std::numeric_limits<T1>::digits;
    if (nbits >= ubits + 1) {
      return ival;
    }
    if (SIA80_UNLIKELY(nbits == 0)) {
      throw std::range_error("cx_sfit: nbits==0");
    }
    // Now shift by nbits guaranteedly fits into T1.
    using T1X = decltype(ival + 0u); // integral promotion
    const T1X tmax = (T1X(1u) << (nbits - 1)) - 1;
    if (SIA80_UNLIKELY(ival > tmax)) {
      throw std::range_error("cx_sfit: too big");
    }
    return ival;
  }

  // TODO cf_sfit
  // TODO tr_sfit
  // TODO sr_sfit

} // namespace sia80
// vim: ts=2 sts=2 sw=2 et :
