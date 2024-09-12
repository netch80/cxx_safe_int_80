#pragma once

#include <safe_int_arith_80.hxx>
#include <stdexcept>

#if WITH_VOLATILE
#define INPUT volatile
#else
#define INPUT
#endif

#define ASSERT_ALWAYS(expr) \
  do { if (!(expr)) { throw std::runtime_error(#expr); } } while(0)

void test_cx_add_signed();
void test_sr_add_signed();
void test_cx_div_signed();
void test_cx_ufit_signed();
void test_cx_ufit_unsigned();
void test_cx_sfit_signed();
void test_cx_sfit_unsigned();
