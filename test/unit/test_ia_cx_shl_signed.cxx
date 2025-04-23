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
        std::cerr << "test_cx_shl_signed: "
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
        std::cerr << "test_cx_shl_signed: want_fail: " << exc_label
                << ": not failed for: arg1=" << (arg1+0)
                << "; arg2=" << (arg2+0) << "\n";
        throw std::runtime_error("Assertion failed: not excepted");
    }
}

//--------------------------------------------------------------

void test_cx_shl_signed_int()
{
  const char *exc_label = "cx_shl signed int";
  if (std::numeric_limits<int>::digits != 31) {
    std::cerr << exc_label << ": not 32 bits, tests shall be remade\n";
    throw std::logic_error("int bits");
  }
  want_ok(int(0), int(0), int(0), exc_label);
  want_ok(int(0), int(1), int(0), exc_label);
  want_ok(int(0), int(16), int(0), exc_label);
  want_ok(int(0), int(30), int(0), exc_label);
  want_ok(int(0), int(31), int(0), exc_label);
  want_ok(int(1), int(1), int(2), exc_label);
  want_ok(int(1), int(30), int(0x40000000), exc_label);
  // NB Shift for 31 bit is treated impossible, this is signed.
  want_fail(int(1), int(31), exc_label);
  want_ok(int(0x40000000), int(0), int(0x40000000), exc_label);
  want_fail(int(0x40000000), int(1), exc_label);
  want_ok(int(-1), int(1), int(-2), exc_label);
  want_ok(int(-1), int(30), int(-0x40000000), exc_label);
  want_ok(int(-1), int(31), int(~0x7FFFFFFF), exc_label);
  want_fail(int(-0x55555555), int(1), exc_label);
}

void test_cx_shl_signed()
{
  test_cx_shl_signed_int();
}
