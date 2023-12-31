AM_CFLAGS = -ansi -std=c99 -D_POSIX_C_SOURCE=200809L
AM_CPPFLAGS = -I$(top_srcdir)/include/
lib_LTLIBRARIES = libailsa.la
libailsa_la_SOURCES = generic.c data.c networking.c sql.c logging.c ailsa_kv_s.c
include_HEADERS = $(top_srcdir)/include/ailsa.h $(top_srcdir)/include/ailsasql.h

if HAVE_LIBLDAP
AM_CFLAGS += -DLDAP_DEPRECATED
AM_CPPFLAGS += $(LDAP_CFLAGS)
AM_LDFLAGS +=  $(LDAP_LDFLAGS)
LIBS += $(LDAP_LIBS)
libailsa_la_SOURCES += ldap.c
endif
