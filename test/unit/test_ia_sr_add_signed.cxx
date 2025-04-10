#include "test_common.hxx"
#include <iostream>

template <class T1, class T2, class T3>
static void want_ok(T1 i_arg1, T2 i_arg2, T3 i_expected_result,
        const char *exc_label)
{
    INPUT T1 arg1 = i_arg1;
    INPUT T2 arg2 = i_arg2;
    INPUT T3 er = i_expected_result;
    try {
        ASSERT_ALWAYS(sia80::sr_add(arg1, arg2) == er);
    }
    catch(std::exception& exc) {
        std::cerr << "test_cx_add_signed: "
                << "want_ok: failed for: " << exc_label
                << ": arg1=" << (arg1+0)
                << "; arg2=" << (arg2+0)
                << "; er=" << (er+0)
                << "\n";
        throw;
    }
}

//--------------------------------------------------------------

void test_sr_add_signed_int()
{
  const char *exc_label = "cx_add signed int";
  constexpr int imin = std::numeric_limits<int>::min();
  constexpr int imax = std::numeric_limits<int>::max();
  want_ok(int(0), int(0), int(0), exc_label);
  want_ok(int(1), int(1), int(2), exc_label);
  want_ok(imin, imax, -1, exc_label);
  want_ok(imax - 1, 1, imax, exc_label);
  want_ok(imax - 1, 2, imax, exc_label);
  want_ok(imax, 1, imax, exc_label);
  want_ok(imin, int(-1), imin, exc_label);
}

void test_sr_add_signed()
{
  test_sr_add_signed_int();
  // TODO More cases
}
