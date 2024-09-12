#include "test_common.hxx"
#include <iostream>

template <class T1>
static void want_ok(T1 arg_ivalue, unsigned arg_width,
        const char *exc_label)
{
    INPUT T1 ivalue = arg_ivalue;
    INPUT unsigned width = arg_width;
    try {
        ASSERT_ALWAYS(sia80::cx_ufit(ivalue, width) == ivalue);
    }
    catch(std::exception& exc) {
        std::cerr << "test_cx_ufit_signed: " << exc_label
                << ": want_ok: failed for: ivalue=" << ivalue
                << "; width=" << width
                << "\n";
        throw;
    }
}

template <class T1>
static void want_fail(T1 arg_ivalue, unsigned arg_width,
        const char *exc_label)
{
    bool excepted;
    INPUT T1 ivalue = arg_ivalue;
    INPUT unsigned width = arg_width;
    try {
        excepted = false;
        ASSERT_ALWAYS(sia80::cx_ufit(ivalue, width) == ivalue);
    }
    catch(std::exception& exc) {
        excepted = true;
    }
    if (!excepted) {
        std::cerr << "test_cx_ufit_signed: " << exc_label
                << ": want_fail: not failed for: ivalue=" << ivalue
                << "; width=" << width
                << "\n";
        throw std::runtime_error("Assertion failed: not excepted");
    }
}

static void test_cx_ufit_signed_int()
{
  constexpr int imax = std::numeric_limits<int>::max();
  constexpr unsigned idigits = std::numeric_limits<int>::digits; // normally 31
  const char *exc_label = "cx_ufit signed int";
  want_ok(int(0), 0, exc_label);
  want_ok(int(0), 1, exc_label);
  want_ok(int(0), idigits, exc_label);
  want_ok(int(0), idigits + 1, exc_label);
  want_ok(int(0), idigits + 10, exc_label);
  want_ok(int(0), idigits * 2, exc_label);
  want_ok(int(0), idigits * 2 + 1, exc_label);
  want_fail(int(1), 0, exc_label);
  want_fail(int(9), 3, exc_label);
  want_ok(int(9), 4, exc_label);
  want_fail(imax, idigits - 1, exc_label);
  want_ok(imax, idigits, exc_label);
  want_ok(imax, idigits + 1, exc_label);
}

static void test_cx_ufit_signed_long()
{
  const char *exc_label = "cx_ufit signed long";
  constexpr long lmax = std::numeric_limits<long>::max();
  constexpr unsigned ldigits = std::numeric_limits<long>::digits; // normally 31 or 63
  want_ok(long(0), 0, exc_label);
  want_ok(long(0), 1, exc_label);
  want_ok(long(0), ldigits - 1, exc_label);
  want_ok(long(0), ldigits, exc_label);
  want_ok(long(0), ldigits + 1, exc_label);
  want_fail(long(1), 0, exc_label);
  want_ok(long(1), 1, exc_label);
  want_fail(lmax, ldigits - 1, exc_label);
  want_ok(lmax, ldigits, exc_label);
  want_ok(lmax, ldigits + 1, exc_label);

}

void test_cx_ufit_signed()
{
  test_cx_ufit_signed_int();
  test_cx_ufit_signed_long();
}
