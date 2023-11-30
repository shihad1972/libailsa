if HAVE_CHECK
TESTS = check_kv check_ailsa_list
check_PROGRAMS = $(TESTS)
AM_CFLAGS = @CHECK_CFLAGS@
AM_LDFLAGS = $(top_builddir)/lib/libailsa.la @CHECK_LIBS@

check_kv = check_kv.c $(top_builddir)/include/ailsa.h
check_ailsa_list = check_ailsa_list.c
endif
