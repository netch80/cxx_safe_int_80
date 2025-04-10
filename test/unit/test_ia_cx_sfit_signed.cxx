#include "test_common.hxx"
#include <iostream>

template <class T1>
static void want_ok(T1 arg_ivalue, unsigned arg_width,
        const char *exc_label)
{
    INPUT T1 ivalue = arg_ivalue;
    INPUT unsigned width = arg_width;
    try {
        ASSERT_ALWAYS(sia80::cx_sfit(ivalue, width) == ivalue);
    }
    catch(std::exception& exc) {
        std::cerr << "test_cx_sfit_signed: " << exc_label
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
        ASSERT_ALWAYS(sia80::cx_sfit(ivalue, width) == ivalue);
    }
    catch(std::exception& exc) {
        excepted = true;
    }
    if (!excepted) {
        std::cerr << "test_cx_sfit_signed: " << exc_label
                << ": want_fail: not failed for: ivalue=" << ivalue
                << "; width=" << width
                << "\n";
        throw std::runtime_error("Assertion failed: not excepted");
    }
}

//--------------------------------------------------------------

void test_cx_sfit_signed_int()
{
    // We assume 1 bit is really minimum for any value. Otherwise
    // it starts getting illogical: fitting to zero width has no sense,
    // and, it discords with the idea of reducing redundant bits
    // identical to sign one.
    const char *exc_label = "cx_sfit signed int";
    constexpr unsigned fidigits = std::numeric_limits<int>::digits + 1;
    constexpr int imin = std::numeric_limits<int>::min();
    constexpr int imax = std::numeric_limits<int>::max();

    want_fail(int(0), 0, exc_label);
    want_ok(int(0), 1, exc_label);
    want_ok(int(0), 2, exc_label);
    want_ok(int(0), fidigits - 1, exc_label);
    want_ok(int(0), fidigits, exc_label);
    want_ok(int(0), fidigits * 2, exc_label);

    want_fail(int(1), 0, exc_label);
    want_fail(int(1), 1, exc_label);
    want_ok(int(1), 2, exc_label);

    want_fail(int(9), 3, exc_label);
    want_fail(int(9), 4, exc_label); // 1001 is not 01001
    want_ok(int(9), 5, exc_label);
    want_fail(int(15), 4, exc_label);
    want_ok(int(15), 5, exc_label);
    want_fail(int(16), 5, exc_label);
    want_ok(int(16), 6, exc_label);

    want_fail(imax, fidigits - 1, exc_label);
    want_ok(imax, fidigits, exc_label);

    want_fail(int(-1), 0, exc_label);
    want_ok(int(-1), 1, exc_label);
    want_ok(int(-1), 2, exc_label);
    want_ok(int(-1), fidigits - 1, exc_label);
    want_ok(int(-1), fidigits, exc_label);
    want_ok(int(-1), fidigits * 2, exc_label);

    want_fail(int(-2), 1, exc_label);
    want_ok(int(-2), 2, exc_label);

    want_fail(int(-9), 3, exc_label);
    want_fail(int(-9), 4, exc_label);
    want_ok(int(-9), 5, exc_label);
    want_fail(int(-15), 4, exc_label);
    want_ok(int(-15), 5, exc_label);
    want_fail(int(-16), 4, exc_label);
    want_ok(int(-16), 5, exc_label);
    want_fail(int(-17), 5, exc_label);
    want_ok(int(-17), 6, exc_label);

    want_fail(imin, fidigits - 1, exc_label);
    want_ok(imin, fidigits, exc_label);
}

void test_cx_sfit_signed_long()
{
    const char *exc_label = "cx_sfit signed long";
    // See notes for "int" case.
    constexpr unsigned fldigits = std::numeric_limits<long>::digits + 1;
    constexpr long lmin = std::numeric_limits<long>::min();
    constexpr long lmax = std::numeric_limits<long>::max();

    want_fail(long(0), 0, exc_label);
    want_ok(long(0), 1, exc_label);
    want_ok(long(0), 31, exc_label);
    want_ok(long(0), 32, exc_label);
    want_ok(long(0), 33, exc_label);
    want_fail(long(1), 0, exc_label);
    want_fail(long(1), 1, exc_label);
    want_ok(long(1), 2, exc_label);
    want_ok(long(1), 31, exc_label);
    want_ok(long(1), 32, exc_label);
    want_ok(long(1), 33, exc_label);

    want_fail(long(9), 3, exc_label);
    want_fail(long(9), 4, exc_label); // 1001 is not 01001
    want_ok(long(9), 5, exc_label);
    want_fail(long(15), 4, exc_label);
    want_ok(long(15), 5, exc_label);
    want_fail(long(16), 5, exc_label);
    want_ok(long(16), 6, exc_label);

    want_fail(lmax, fldigits - 1, exc_label);
    want_ok(lmax, fldigits, exc_label);
    want_ok(lmax, fldigits + 1, exc_label);

    want_fail(long(-1), 0, exc_label);
    want_ok(long(-1), 1, exc_label);
    want_ok(long(-1), 2, exc_label);
    want_ok(long(-1), fldigits - 1, exc_label);
    want_ok(long(-1), fldigits, exc_label);
    want_ok(long(-1), fldigits * 2, exc_label);

    want_fail(long(-9), 3, exc_label);
    want_fail(long(-9), 4, exc_label);
    want_ok(long(-9), 5, exc_label);
    want_fail(long(-15), 4, exc_label);
    want_ok(long(-15), 5, exc_label);
    want_fail(long(-16), 4, exc_label);
    want_ok(long(-16), 5, exc_label);
    want_fail(long(-17), 5, exc_label);
    want_ok(long(-17), 6, exc_label);

    want_fail(lmin, fldigits - 1, exc_label);
    want_ok(lmin, fldigits, exc_label);
    want_ok(lmin, fldigits + 1, exc_label);
}

void test_cx_sfit_signed()
{
  test_cx_sfit_signed_int();

  test_cx_sfit_signed_long();
}
