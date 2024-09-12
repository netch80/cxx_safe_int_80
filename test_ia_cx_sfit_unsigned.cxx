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
        std::cerr << "test_cx_sfit_unsigned: " << exc_label
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
        std::cerr << "test_cx_sfit_unsigned: " << exc_label
                << ": want_fail: not failed for: ivalue=" << ivalue
                << "; width=" << width
                << "\n";
        throw std::runtime_error("Assertion failed: not excepted");
    }
}

//--------------------------------------------------------------

void test_cx_sfit_unsigned_int()
{
    const char *exc_label = "cx_sfit unsigned int";
    using UINT = unsigned int;
    // We assume 1 bit is really minimum for any value. Otherwise
    // it starts getting illogical: fitting to zero width has no sense,
    // and, it discords with the idea of reducing redundant bits
    // identical to sign one.
    constexpr unsigned uidigits = std::numeric_limits<UINT>::digits;
    constexpr UINT uimax = std::numeric_limits<UINT>::max();

    want_fail(UINT(0), 0, exc_label);
    want_ok(UINT(0), 1, exc_label);
    want_ok(UINT(0), 2, exc_label);
    want_ok(UINT(0), uidigits - 1, exc_label);
    want_ok(UINT(0), uidigits, exc_label);
    want_ok(UINT(0), uidigits * 2, exc_label);

    want_fail(UINT(1), 0, exc_label);
    want_fail(UINT(1), 1, exc_label);
    want_ok(UINT(1), 2, exc_label);

    want_fail(UINT(9), 3, exc_label);
    want_fail(UINT(9), 4, exc_label); // 1001 is not 01001
    want_ok(UINT(9), 5, exc_label);
    want_fail(UINT(15), 4, exc_label);
    want_ok(UINT(15), 5, exc_label);
    want_fail(UINT(16), 5, exc_label);
    want_ok(UINT(16), 6, exc_label);

    want_fail(uimax, uidigits - 1, exc_label);
    want_fail(uimax, uidigits, exc_label);
    want_ok(uimax, uidigits + 1, exc_label);
}

void test_cx_sfit_unsigned_long()
{
    const char *exc_label = "cx_sfit unsigned long";
    using ULONG = unsigned long;
    // See notes for "int" case.
    constexpr unsigned uldigits = std::numeric_limits<ULONG>::digits;
    constexpr ULONG ulmax = std::numeric_limits<ULONG>::max();

    want_fail(ULONG(0), 0, exc_label);
    want_ok(ULONG(0), 1, exc_label);
    want_ok(ULONG(0), 31, exc_label);
    want_ok(ULONG(0), 32, exc_label);
    want_ok(ULONG(0), 33, exc_label);
    want_fail(ULONG(1), 0, exc_label);
    want_fail(ULONG(1), 1, exc_label);
    want_ok(ULONG(1), 2, exc_label);
    want_ok(ULONG(1), 31, exc_label);
    want_ok(ULONG(1), 32, exc_label);
    want_ok(ULONG(1), 33, exc_label);

    want_fail(ULONG(9), 3, exc_label);
    want_fail(ULONG(9), 4, exc_label); // 1001 is not 01001
    want_ok(ULONG(9), 5, exc_label);
    want_fail(ULONG(15), 4, exc_label);
    want_ok(ULONG(15), 5, exc_label);
    want_fail(ULONG(16), 5, exc_label);
    want_ok(ULONG(16), 6, exc_label);

    want_fail(ulmax, uldigits - 1, exc_label);
    want_fail(ulmax, uldigits, exc_label);
    want_ok(ulmax, uldigits + 1, exc_label);
}

void test_cx_sfit_unsigned()
{
  test_cx_sfit_unsigned_int();

  test_cx_sfit_unsigned_long();
}
