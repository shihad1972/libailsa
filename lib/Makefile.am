AM_CFLAGS = -ansi -std=c99 -D_POSIX_C_SOURCE=200809L
AM_CPPFLAGS = -I$(top_srcdir)/include/
AM_LDFLAGS =
lib_LTLIBRARIES = libailsa.la
libailsa_la_SOURCES = generic.c data.c networking.c sql.c logging.c ailsa_kv_s.c b64.c
include_HEADERS = $(top_srcdir)/include/ailsa.h $(top_srcdir)/include/ailsasql.h

if HAVE_LIBLDAP
AM_CFLAGS += -DLDAP_DEPRECATED
AM_CPPFLAGS += $(LDAP_CFLAGS)
AM_LDFLAGS +=  $(LDAP_LDFLAGS)
LIBS += $(LDAP_LIBS)
libailsa_la_SOURCES += ldap.c
endif

if HAVE_LIBUUID
AM_CPPFLAGS += $(UUID_CPPFLAGS)
AM_LDFLAGS += $(UUID_LDFLAGS)
LIBS += $(UUID_LIBS)
libailsa_la_SOURCES += uuid.c
endif
