PROG = test_ia
OBJS = test_ia_main.o \
	test_ia_cx_add_signed.o \
	test_ia_sr_add_signed.o \
	test_ia_cx_div_signed.o \
	test_ia_cx_ufit_signed.o \
	test_ia_cx_ufit_unsigned.o \
	test_ia_cx_sfit_signed.o \
	test_ia_cx_sfit_unsigned.o
CXXFLAGS = -Wall -W -g -I. -std=c++17
WITH_VOLATILE?= 1
ifneq "$(WITH_VOLATILE)" ""
CXXFLAGS += -DWITH_VOLATILE
endif
OPTLEVEL ?= g
CXXFLAGS += -O$(OPTLEVEL)
#- CXXFLAGS += -fno-omit-frame-pointer

prog: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $(PROG) $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.cxx
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(CXXOPTS)

*.o: safe_int_arith_80.hxx

clean:
	rm -f $(PROG) $(OBJS)

.PHONY: clean
