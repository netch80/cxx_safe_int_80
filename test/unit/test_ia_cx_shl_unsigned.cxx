#include "test_common.hxx"
#include <iostream>

template <class T1, class T2, class T3>
static void want_ok(T1 i_arg1, T2 i_arg2, T3 i_expected_result,
        const char *exc_label)
{
    INPUT T1 arg1 = i_arg1;
    INPUT T2 arg2 = i_arg2;
    INPUT T3 er = i_expected_result;
    using TR = decltype(sia80::cx_shl(arg1, arg2));
    TR result{};
    try {
        result = sia80::cx_shl(arg1, arg2);
        ASSERT_ALWAYS(result == er);
    }
    catch(std::exception& exc) {
        std::cerr << "test_cx_shl_unsigned: "
                << "want_ok: failed for: " << exc_label
                << ": arg1=" << (arg1+0)
                << "; arg2=" << (arg2+0)
                << "; actual=" << (result+0)
                << "; expected=" << (er+0)
                << "\n";
        throw;
    }
}

template <class T1, class T2>
static void want_fail(T1 i_arg1, T2 i_arg2,
        const char *exc_label)
{
    bool excepted = false;
    INPUT T1 arg1 = i_arg1;
    INPUT T2 arg2 = i_arg2;
    using TR = decltype(i_arg1 + i_arg2);
    try {
        volatile TR result = sia80::cx_shl(arg1, arg2);
        (void) result;
    }
    catch(std::exception& exc) {
        excepted = true;
    }
    if (!excepted) {
        std::cerr << "test_cx_shl_unsigned: want_fail: " << exc_label
                << ": not failed for: arg1=" << (arg1+0)
                << "; arg2=" << (arg2+0) << "\n";
        throw std::runtime_error("Assertion failed: not excepted");
    }
}

//--------------------------------------------------------------

void test_cx_shl_unsigned_int()
{
  const char *exc_label = "cx_shl unsigned int";
  constexpr unsigned imax = std::numeric_limits<unsigned>::max();
  want_ok(unsigned(0), unsigned(0), unsigned(0), exc_label);
  want_ok(unsigned(0), unsigned(1), unsigned(0), exc_label);
  want_ok(unsigned(0), unsigned(16), unsigned(0), exc_label);
  want_ok(unsigned(0), unsigned(31), unsigned(0), exc_label);
  want_ok(unsigned(1u), unsigned(0), unsigned(1u), exc_label);
  want_ok(unsigned(1u), unsigned(1), unsigned(2u), exc_label);
  want_ok(unsigned(1u), unsigned(16), unsigned(0x10000u), exc_label);
  want_ok(unsigned(1u), unsigned(31), unsigned(0x80000000u), exc_label);
  want_fail(unsigned(1u), unsigned(32), exc_label);
  want_ok(unsigned(4u), unsigned(29), unsigned(0x80000000u), exc_label);
  want_fail(unsigned(4u), unsigned(30), exc_label);
  want_ok(imax, unsigned(0), imax, exc_label);
  want_fail(imax, unsigned(1u), exc_label);
}

void test_cx_shl_unsigned()
{
  test_cx_shl_unsigned_int();
}
