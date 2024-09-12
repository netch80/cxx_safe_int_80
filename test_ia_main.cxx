#include "test_common.hxx"
#include <cstdio>

void test_tr_shl_signed()
{
  INPUT int iia, sw;
  //
  iia = 0x7777777;
  sw = 1;
  ASSERT_ALWAYS(sia80::tr_shl(iia, sw) == 0xEEEEEEE);
  //
  sw = 2;
  ASSERT_ALWAYS(sia80::tr_shl(iia, sw) == 0x1DDDDDDC);
  //
  sw = 4;
  ASSERT_ALWAYS(sia80::tr_shl(iia, sw) == 0x77777770);
  //
  sw = 5;
  ASSERT_ALWAYS(sia80::tr_shl(iia, sw) == -0x11111120);
  //
  sw = 6;
  ASSERT_ALWAYS(sia80::tr_shl(iia, sw) == -0x22222240);
  //
  sw = 100;
  ASSERT_ALWAYS(sia80::tr_shl(iia, sw) == 0);
  //
  sw = -100;
  ASSERT_ALWAYS(sia80::tr_shl(iia, sw) == 0);
}

int main()
{
  test_cx_add_signed();
  // TODO test_cx_add_unsigned
  // TODO test_cf_add_signed
  // TODO test_cf_add_unsigned
  // TODO test_tr_add_signed
  // TODO test_tr_add_unsigned
  test_sr_add_signed();
  // TODO test_sr_add_unsigned
  // TODO sub
  // TODO mul
  test_cx_div_signed();
  // TODO test_cx_div_unsigned
  // TODO test_cf_div_signed
  // TODO test_cf_div_unsigned
  // TODO test_tr_div_signed
  // TODO test_tr_div_unsigned
  // TODO test_sf_div_signed
  // TODO test_sf_div_unsigned
  // TODO rem
  // TODO shl
  // TODO test_cx_shl_signed
  // TODO test_cx_shl_unsigned
  // TODO test_cf_shl_signed
  // TODO test_cf_shl_unsigned
  test_tr_shl_signed();
  // TODO test_tr_shl_unsigned
  // TODO test_sr_shl_signed
  // TODO test_sr_shl_unsigned
  // TODO shr
  // TODO shre
  // TODO conv
  // TODO test_cx_ufit_signed
  test_cx_ufit_signed();
  test_cx_ufit_unsigned();
  // TODO test_cx_ufit_unsigned
  // TODO test_cf_ufit_signed
  // TODO test_cf_ufit_unsigned
  // TODO test_tr_ufit_signed
  // TODO test_tr_ufit_unsigned
  // TODO test_sr_ufit_signed
  // TODO test_sr_ufit_unsigned
  // TODO test_cx_ufit_signed
  test_cx_sfit_signed();
  test_cx_sfit_unsigned();
  // TODO test_cf_sfit_signed
  // TODO test_cf_sfit_unsigned
  // TODO test_tr_sfit_signed
  // TODO test_tr_sfit_unsigned
  // TODO test_sr_sfit_signed
  // TODO test_sr_sfit_unsigned

#if 0
  volatile int numr1 = -2147483647-1;
  volatile int divr0 = 0;
  volatile int divr1 = -1;
  int tr_div0 = sia80::tr_div(numr1, divr0);
  printf("tr_div0=%d(%x)\n", tr_div0, tr_div0);
  int tr_div1 = sia80::tr_div(numr1, divr1);
  printf("tr_div1=%d(%x)\n", tr_div1, tr_div1);
  int sr_div0 = sia80::sr_div(numr1, divr0);
  printf("sr_div0=%d(%x)\n", sr_div0, sr_div0);
  int sr_div1 = sia80::sr_div(numr1, divr1);
  printf("sr_div1=%d(%x)\n", sr_div1, sr_div1);

  int8_t rconv1 = sia80::sr_conv<int8_t>(i31);
  printf("sr_conv to int8_t: %d\n", rconv1);

#endif
  printf("All tests passed\n");
}
